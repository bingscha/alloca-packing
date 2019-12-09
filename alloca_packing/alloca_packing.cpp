#include "llvm/Pass.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/Format.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"
#include "llvm/Transforms/Scalar/DCE.h"

#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace llvm;

using std::numeric_limits;
using std::pair;
using std::unordered_map;
using std::unordered_set;
using std::vector;

namespace {
    struct SoftwareRegister {
        unordered_map<AllocaInst*, pair<int,int>> bit_ranges;
        int size = 0;
    };


    struct AllocaPackPass : public FunctionPass {
        static char ID;
        AllocaPackPass() : FunctionPass(ID) {}

        void getAnalysisUsage(AnalysisUsage &AU) const { 
            AU.setPreservesAll();
        }

        virtual bool runOnFunction(Function &F) {
            vector<AllocaInst*> all_allocas;
            unordered_map<AllocaInst*, unordered_map<AllocaInst*, int>> alloca_to_score;
            findAlloca(F, all_allocas);
            alloca_to_score = getSimilarUses(F, all_allocas);

            // for (auto& temp : alloca_to_score) {
            //     errs() << *(temp.first) << "\n";
            //     for (auto& val : temp.second) {
            //         errs() << "\t" << *(val.first) << " " << val.second <<  "\n";
            //     }
            // }

            unordered_map<AllocaInst*, SoftwareRegister> packed = packAlloca(F, all_allocas, alloca_to_score);
            int sum = 0;
            for (auto it = packed.begin(); it != packed.end(); ) {
                int num = it->second.bit_ranges.size();
                assert(num >= 1);
                if (num < 2) {
                    auto prev = it;
                    it++;
                    packed.erase(prev);
                } 
                else {
                    it++;
                }
                sum += num;
            }
            assert(sum == all_allocas.size());
            fixUpCode(F, packed);
            
            bool changed = true;
            while (changed) {
                changed = false;
                vector<Instruction*> to_delete;
                for (auto& BB : F) {
                    for (auto& I : BB) {
                        if (I.use_empty() && !dyn_cast<StoreInst>(&I) && !dyn_cast<BranchInst>(&I) && !dyn_cast<ReturnInst>(&I) && !dyn_cast<CallInst>(&I)) {
                            errs() << "DELETING" << I << "\n";
                            to_delete.push_back(&I);
                            changed = true;
                        }
                    }
                }

                for (Instruction* victim : to_delete) {
                    victim->eraseFromParent();
                }
            }

            return true;
        }

        void findAlloca(Function& F, vector<AllocaInst*>& all_allocas) {
            for (BasicBlock& bb : F) {
                for (Instruction& i : bb) {
                    // Get infomration on Alloca's
                    if (AllocaInst* alloc = dyn_cast<AllocaInst>(&i)) {
                        auto sz = alloc->getAllocationSizeInBits(F.getParent()->getDataLayout());
                        // Only get data that is less than 64 bits
                        if (sz.hasValue() && sz.getValue() < 64) {
                            all_allocas.push_back(alloc);
                        }
                    }
                }
            }

        }

        // For each alloca, find all similar uses (same BB)
        unordered_map<AllocaInst*, unordered_map<AllocaInst*, int>> getSimilarUses(Function& F, vector<AllocaInst*>& all_allocas) {
            // Matrix to determine which alloca's have the most in common uses with another alloca i.e. their uses are in the same BB
            unordered_map<AllocaInst*, unordered_map<AllocaInst*, int>> alloca_to_score;
            const DataLayout& dl = F.getParent()->getDataLayout();

            // Iterate through all the packable allocas
            for (int idx = 0; idx < all_allocas.size(); ++idx) {
                AllocaInst* alloc = all_allocas[idx];

                // Itearate through the current user
                for (auto i = alloc->use_begin(); i != alloc->use_end(); ++i) {
                    Instruction* user = dyn_cast<Instruction>(i->getUser());

                    // Iterate through the other allocas
                    for (int other_idx = idx + 1; other_idx < all_allocas.size(); ++other_idx) {
                        AllocaInst* other_alloc = all_allocas[other_idx];
                        if (other_alloc->getAllocationSizeInBits(dl).getValue() + alloc->getAllocationSizeInBits(dl).getValue() > 64) continue;

                        // Iterate through the other alloca's uses
                        for (auto j = other_alloc->use_begin(); j != other_alloc->use_end(); ++j) {
                            Instruction* other_user = dyn_cast<Instruction>(j->getUser());

                            // If they share a parent, add + 1 to the each other's score
                            if (other_user->getParent() == user->getParent()) {
                                alloca_to_score[alloc][other_alloc] += 1;
                                alloca_to_score[other_alloc][alloc] += 1;
                            }
                        }
                    }
                }
            }

            return alloca_to_score;
        }

        // Pack allocas that are closest in basic blocks
        unordered_map<AllocaInst*, SoftwareRegister> packAlloca(Function& F, vector<AllocaInst*>& all_allocas, unordered_map<AllocaInst*, unordered_map<AllocaInst*, int>>& alloca_to_score) {
            unordered_map<AllocaInst*, SoftwareRegister> packed_registers;
            const DataLayout& dl = F.getParent()->getDataLayout();

            for (AllocaInst* alloc : all_allocas) {
                auto size = alloc->getAllocationSizeInBits(dl).getValue();
                packed_registers[alloc].bit_ranges[alloc] = { 0, size };
                packed_registers[alloc].size = size;
            }
            
            while (alloca_to_score.size()) {
                auto largest = getLargestScore(alloca_to_score);
                
                // Add this alloca to the prev
                for (auto& adding_alloc : packed_registers[largest.second].bit_ranges) {
                    auto& curr_size = packed_registers[largest.first].size;
                    auto alloc_size = adding_alloc.first->getAllocationSizeInBits(dl).getValue();
                    packed_registers[largest.first].bit_ranges[adding_alloc.first] = { curr_size, curr_size + alloc_size };
                    curr_size += alloc_size;
                }

                //errs() << "combining\n" << *(largest.first) << "\n" << *(largest.second) << "\n" << packed_registers[largest.first].size << "\n";
                assert(packed_registers[largest.first].size <= 64);
                packed_registers.erase(largest.second);

                // iterate through the second one
                for (auto& to_remove : alloca_to_score[largest.second]) {
                    uint64_t size = packed_registers[to_remove.first].size;

                    // If this is less than, we can add to the score
                    if (size + packed_registers[largest.first].size <= 64 && to_remove.first != largest.first) {
                        alloca_to_score[largest.first][to_remove.first] += to_remove.second;
                    }
                }

                // Erase second
                alloca_to_score.erase(largest.second);

                // Check if second exists anywhere else and erase
                for (auto& vals : alloca_to_score) {
                    if (vals.second.count(largest.second)) {
                        vals.second.erase(largest.second);
                    }
                }

                // Iterate through first and remove if necessary
                for (auto it = alloca_to_score[largest.first].begin(); it != alloca_to_score[largest.first].end();) {
                    uint64_t size = packed_registers[it->first].size;
                    
                    if (size + packed_registers[largest.first].size > 64) {
                        auto prev = it;
                        it++;
                        alloca_to_score[largest.first].erase(prev);
                        alloca_to_score[prev->first].erase(largest.first);
                        if (alloca_to_score[prev->first].empty()) {
                            alloca_to_score.erase(prev->first);
                        }
                    }
                    else {
                        it++;
                    }
                }

                if (alloca_to_score[largest.first].empty()) {
                    alloca_to_score.erase(largest.first);
                }
            }

            for (auto& temp : packed_registers) {
                errs() << *(temp.first) << " " << temp.second.size << "\n";
                for (auto& val : temp.second.bit_ranges) {
                    errs() << "\t" << *(val.first) << " "  << val.second.first << " " << val.second.second << "\n";
                }
            }

            return packed_registers;
        }

        pair<AllocaInst*, AllocaInst*> getLargestScore(unordered_map<AllocaInst*, unordered_map<AllocaInst*, int>>& alloca_to_score) {
            int top_score = 0;
            AllocaInst* first, *second;

            for (auto& row : alloca_to_score) {
                for (auto& col : row.second) {
                    if (col.second > top_score) {
                        first = row.first;
                        second = col.first;
                        top_score = col.second;
                    }
                }
            }

            return { first, second };
        }

        void fixUpCode(Function& F, unordered_map<AllocaInst*, SoftwareRegister>& packed) {
            // 1. Create the new allocas
            static LLVMContext context_s;
            unordered_map<AllocaInst*, AllocaInst*> old_to_new_allocas;
            unordered_map<AllocaInst*, SoftwareRegister> new_to_packed;
            for (auto& elt : packed) {
                auto new_alloca = new AllocaInst(Type::getInt64Ty(context_s), 0, "cursed", elt.first);
                new_to_packed[new_alloca] = elt.second;
                for (auto& packing : elt.second.bit_ranges) {
                    old_to_new_allocas[packing.first] = new_alloca;
                }
            }

            // 2.
            // Iterate through each BB and load our new variable if necessary
            for (auto& BB : F) {
                unordered_map<AllocaInst*, unordered_set<AllocaInst*>> new_to_needed;

                // Determine which alloca inst are needed
                for (Instruction& I : BB) {
                    AllocaInst* isAlloca = nullptr;
                    if (LoadInst* load = dyn_cast<LoadInst>(&I)) {
                        isAlloca = dyn_cast<AllocaInst>(load->getPointerOperand());
                    }
                    if (StoreInst* store = dyn_cast<StoreInst>(&I)) {
                        isAlloca = dyn_cast<AllocaInst>(store->getPointerOperand());
                        // at this point, guaranteed to be dealing with our alloca boi
                        auto it = old_to_new_allocas.find(isAlloca);
                        if (it != old_to_new_allocas.end() && !new_to_needed.count(it->second)) {
                            new_to_needed[it->second] = unordered_set<AllocaInst*>();
                        }
                    }
                    if (!isAlloca) {
                        continue;
                    }
                    // at this point, guaranteed to be dealing with our alloca boi
                    auto it = old_to_new_allocas.find(isAlloca);
                    if (it != old_to_new_allocas.end()) {
                        new_to_needed[it->second].insert(isAlloca);
                    } 
                }

                //keep track of last value
                unordered_map<AllocaInst*, Value*> alloca_to_last_value;
                unordered_map<AllocaInst*, LoadInst*> new_alloca_to_load;
                //3.
                // add loads to the beginning of basic block and extract variables
                //to_load is <new, needed vector>
                for (auto& to_load : new_to_needed) {
                    Instruction* to_insert = &(BB.getInstList().front());
                    while(true) {
                        auto alloca = dyn_cast<AllocaInst>(to_insert);
                        if (alloca) {
                            to_insert = to_insert->getNextNode();
                            continue;
                        }
                        auto store_boi = dyn_cast<StoreInst>(to_insert);
                        if (!store_boi) {
                            break;
                        }
                        if (auto alloca = dyn_cast<AllocaInst>(store_boi->getPointerOperand())) {
                            if (new_to_packed.find(alloca) != new_to_packed.end()) {
                                to_insert = to_insert->getNextNode();
                                continue;
                            }
                        }
                        
                        break;
                    }

                    LoadInst* load = new LoadInst(to_load.first);
                    load->insertBefore(to_insert);
                    // save this load as the original value to do the final store
                    new_alloca_to_load[to_load.first] = load;

                    auto& bit_ranges = new_to_packed[to_load.first].bit_ranges;
                    //extract each needed
                    for (auto alloca_inst : to_load.second) {
                        int start = bit_ranges[alloca_inst].first, end = bit_ranges[alloca_inst].second;

                        // only shift right if you actually need to
                        Instruction* shr = nullptr;
                        if (start != 0) {
                            shr = BinaryOperator::CreateAShr(load, ConstantInt::get(Type::getInt64Ty(context_s), start));
                            shr->insertAfter(load);

                            errs() << "Shift RIGHT" << *shr << "\n";
                        }
                        int64_t mask_value = -1 + (1L << (end  - start));
                        // errs() << end << " " << start << "\n";
                        Instruction* and_instr = BinaryOperator::CreateAnd(start == 0 ? load : shr, ConstantInt::get(Type::getInt64Ty(context_s), mask_value));
                        and_instr->insertAfter(start == 0 ? load : shr);
                        errs() << "And with " << *(and_instr) << "\n";

                        auto opcode = CastInst::getCastOpcode(and_instr, true, alloca_inst->getAllocatedType(), (dyn_cast<IntegerType>(alloca_inst->getAllocatedType())) ? true : false);
                        Instruction* cast_instr = CastInst::Create(opcode, and_instr, alloca_inst->getAllocatedType());
                        cast_instr->insertAfter(and_instr);
                        errs() << "Cast instr " << *cast_instr << "\n";

                        alloca_to_last_value[alloca_inst] = cast_instr;                        
                    }
                    
                }

                // errs() << "Got here\n";
                for (auto bb_it = BB.begin(); bb_it != BB.end(); ) {
                    Instruction& I = *bb_it;
                    bb_it++;
                    AllocaInst* isAlloca = nullptr;
                    if (StoreInst* store = dyn_cast<StoreInst>(&I)) {
                        isAlloca = dyn_cast<AllocaInst>(store->getPointerOperand());
                        errs() << "STORE TO REMOVE: " << *store << "\n";
                        auto it = old_to_new_allocas.find(isAlloca);
                        if (it == old_to_new_allocas.end()) {
                            continue;
                        }

                        alloca_to_last_value[isAlloca] = store->getValueOperand();
                        store->eraseFromParent();
                    }
                    else if (LoadInst* load = dyn_cast<LoadInst>(&I)) {
                        isAlloca = dyn_cast<AllocaInst>(load->getPointerOperand());

                        auto it = old_to_new_allocas.find(isAlloca);
                        if (it == old_to_new_allocas.end()) {
                            continue;
                        }

                        for (auto us = load->use_begin(); us != load->use_end(); ++us) {
                            assert(dyn_cast<Instruction>(us->getUser())->getParent() == &BB);
                        }

                        load->replaceAllUsesWith(alloca_to_last_value[isAlloca]);
                        assert(load->use_begin() == load->use_end());
                        load->eraseFromParent();
                    }
                }

                //4. Issue final store for-each packed register
                for (auto& to_store : new_to_needed) {
                    //to_store.first is the new alloca
                    // start with all 1's
                    uint64_t and_mask = numeric_limits<uint64_t>::max();
                    bool used_in_bb = false;
                    for (auto& packed : new_to_packed[to_store.first].bit_ranges) {
                        if (alloca_to_last_value.find(packed.first) == alloca_to_last_value.end()) {
                            //this specific packed value in this register hasn't been used in this BB
                            continue;
                        }
                        used_in_bb = true;
                        //zero out in and_mask the bit range
                        // if number is 1111 1111 1111, we could subtract out 1111 0000
                        uint64_t num_to_subtract = -1 + (1L << (packed.second.second - packed.second.first));
                        num_to_subtract <<= packed.second.first;
                        and_mask -= num_to_subtract;

                        // errs() << "BITS: " << packed.second.first << " " << packed.second.second << "\n";
                    }
                    //if and_mask is still all 1's, then there was no use
                    if (!used_in_bb) {
                        continue;
                    }

                    // and the original load with this and_mask (keeps all the bits except the values used in the BB)

                    assert(new_alloca_to_load.find(to_store.first) != new_alloca_to_load.end());
                    Instruction* and_instr = BinaryOperator::CreateAnd(new_alloca_to_load[to_store.first], ConstantInt::get(Type::getInt64Ty(context_s), and_mask));
                    and_instr->insertBefore(BB.getTerminator());

                    auto result = and_instr;
                    for (auto& packed : new_to_packed[to_store.first].bit_ranges) {
                        auto alloca_it = alloca_to_last_value.find(packed.first);
                        if (alloca_it == alloca_to_last_value.end()) {
                            //this specific packed value in this register hasn't been used in this BB
                            continue;
                        }

                        // actually use this fucking alloca_to_last_value
                        // cast, shift, then or on the result of our previous op
                        auto& val = alloca_it->second;
                        auto opcode = CastInst::getCastOpcode(val, (dyn_cast<IntegerType>(alloca_it->first->getAllocatedType())) ? true : false, Type::getInt64Ty(context_s), true);
                        
                        Instruction* cast_instr = CastInst::Create(opcode, val, Type::getInt64Ty(context_s));
                        cast_instr->insertAfter(result);
                        
                        Instruction* shl = BinaryOperator::CreateShl(cast_instr, ConstantInt::get(Type::getInt64Ty(context_s), packed.second.first));
                        shl->insertAfter(cast_instr);

                        Instruction* or_inst = BinaryOperator::CreateOr(shl, result);
                        or_inst->insertAfter(shl);
                        result = or_inst;
                    }
                    //store result
                    StoreInst* store = new StoreInst(result, to_store.first);
                    store->insertAfter(result);
                }
            }
            
        }
    };
}

char AllocaPackPass::ID = 0;

static RegisterPass<AllocaPackPass> X("AllocaPack", "Alloca Pack Pass");

static void registerStatisticsPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
            
    PM.add(new AllocaPackPass());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerStatisticsPass);
