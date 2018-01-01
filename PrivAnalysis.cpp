#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

void getFamiliar(Module &M);

namespace {
    struct PrivAnalysis : public ModulePass {
        static char ID;
        PrivAnalysis() : ModulePass(ID) { }

        bool runOnModule(Module &M) override {
            errs() << "Helo from PrivAnalysis Pass\n";
            errs().write_escaped(M.getName()) << "\n";
            /* errs() << "Module name: " << M.getName() << "\n"; */

            getFamiliar(M);
            return false;
        }
    };


}

void getFamiliar(Module &M) {
    static LLVMContext context;
    StringRef sr("some_fun0");
    BasicBlock *b = BasicBlock::Create(context, "some_block", NULL, NULL);
    Function *f = M.getFunction(sr);
    for (Value::user_iterator ui = f->user_begin(); ui != f->user_end(); ui++) {
        errs() << ui->getName() << " calls some_fun0\n";
    }
}

char PrivAnalysis::ID = 0;
static RegisterPass<PrivAnalysis> A("priv_analysis", "Privilege Analysis pass", false, false);

