/** SplitBasicBlock.cpp
 *
 *      The Automated Privileges Project
 *
 * */

#include "SplitBasicBlock.h"

#include <vector>

using namespace std;
using namespace llvm;
using namespace splitBB;

// constructor
SplitBB::SplitBB() : ModulePass(ID) { }


/* void SplitBB::getAnalysisUsage(AnalysisUsage &AU) const; */


bool SplitBB::runOnModule(Module &M) {
    errs() << "Hello from SplitBB pass!\n";

    for (Module::iterator mi = M.begin(); mi != M.end(); mi++) splitFunc(*mi);

    dump(M);

    return true;
}

// split a basic block by Callinst
void SplitBB::splitFunc(Function &F) {
    vector<CallInst *> CIs;
    for (Function::iterator fi = F.begin(); fi != F.end(); fi++) {
        for (BasicBlock::iterator bi = fi->begin(); bi != fi->end(); bi++) {
            CallInst *ci = dyn_cast<CallInst>(&*bi);
            if (ci != NULL) {
                Function *callee = dyn_cast<Function>(ci->getCalledValue()->stripPointerCasts());
                // skip library functions
                if (callee != NULL && callee->isDeclaration()) continue;

                // collect CallInst that should be a split point
                CIs.push_back(ci);
            }
        }
    }

    // split this Function
    for (CallInst *ci : CIs) ci->getParent()->splitBasicBlock(ci);
}

void SplitBB::dump(Module &M) const {
    /* Function *F = M.getFunction("indirect_foo"); */
    /* errs() << "\n===== After spliting, " << F->getName() << " became\n"; */
    /* F->dump(); */
}

// Pass registry
char SplitBB::ID = 0;
static RegisterPass<SplitBB> B("SplitBB", "Split BasicBlock pass", 
                               false, /* Modifies the CFG */
                               false /* Modify the program */);





