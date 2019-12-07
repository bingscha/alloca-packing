#include "llvm/Pass.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/Format.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"

#include <unordered_map>
#include <vector>

using namespace llvm;

using std::unordered_map;
using std::vector;

namespace {
    struct AllocaPackPass : public FunctionPass {
        static char ID;
        AllocaPackPass() : FunctionPass(ID) {}

        void getAnalysisUsage(AnalysisUsage &AU) const { 
            AU.setPreservesAll();
        }

        virtual bool runOnFunction(Function &F) {
            unordered_map<uint64_t, vector<AllocaInst*>> size_to_allocas;
            vector<AllocaInst*> all_allocas;
            unordered_map<AllocaInst*, unordered_map<AllocaInst*, int>> alloca_to_score;
            findAlloca(F, size_to_allocas, all_allocas);
            alloca_to_score = getSimilarUses(F, size_to_allocas, all_allocas);

            for (auto& temp : alloca_to_score) {
                errs() << *(temp.first) << "\n";
                for (auto& val : temp.second) {
                    errs() << "\t" << *(val.first) << " " << val.second <<  "\n";
                }
            }
            packAlloca(F, size_to_allocas, all_allocas);

            return false;
        }

        void findAlloca(Function& F, unordered_map<uint64_t, vector<AllocaInst*>>& size_to_allocas, vector<AllocaInst*>& all_allocas) {
            for (BasicBlock& bb : F) {
                for (Instruction& i : bb) {
                    // Get infomration on Alloca's
                    if (AllocaInst* alloc = dyn_cast<AllocaInst>(&i)) {
                        auto sz = alloc->getAllocationSizeInBits(F.getParent()->getDataLayout());
                        // Only get data that is less than 64 bits
                        if (sz.hasValue() && sz.getValue() < 64) {
                            size_to_allocas[sz.getValue()].push_back(alloc);
                            all_allocas.push_back(alloc);
                        }
                    }
                }
            }

            for (auto& val : size_to_allocas) {
                for (auto a : val.second)
                    errs() << val.first << " " << *a << "\n";
            }
        }

        // For each alloca, find all similar uses (same BB)
        unordered_map<AllocaInst*, unordered_map<AllocaInst*, int>> getSimilarUses(Function& F, unordered_map<uint64_t, vector<AllocaInst*>>& size_to_allocas, vector<AllocaInst*>& all_allocas) {
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
        void packAlloca(Function& F, unordered_map<uint64_t, vector<AllocaInst*>>& size_to_allocas, vector<AllocaInst*>& all_allocas) {

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
