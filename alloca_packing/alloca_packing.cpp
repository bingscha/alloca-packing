#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/Format.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"

using namespace llvm;

namespace {
  struct AllocaPackPass : public FunctionPass {
    static char ID;
    AllocaPackPass() : FunctionPass(ID) {}

    void getAnalysisUsage(AnalysisUsage &AU) const { 
        AU.setPreservesAll();
    }

    virtual bool runOnFunction(Function &F) {
        // Get Analysis Pass Information

        for (BasicBlock& bb : F) {
            for (Instruction& i : bb) {
                if (AllocaInst* temp = dyn_cast<AllocaInst>(&i)) {
                    auto sz = temp->getAllocationSizeInBits(F.getParent()->getDataLayout());
                    if (sz.hasValue()) {
                        auto value = sz.getValue();
                        errs() << i << " " << value << "\n";
                    }
                }
            }
        }

        return false;
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
