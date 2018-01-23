/* UnfoldCallee.cpp
 *
 *     The Automated Privileges Project
 *
 * */

#include "UnfoldCallee.h"
#include "CompressCFG.h"
#include "CompressCallGraph.h"

using namespace llvm;
using namespace unfoldCallee;
using namespace compressCFG;
using namespace compressCG;

// constructor
UnfoldCallee::UnfoldCallee() : ModulePass(ID) {}

void UnfoldCallee::getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequired<CompressCFG>();
    AU.addPreserved<CompressCFG>();

    AU.setPreservesAll();
}

// entrance of this pass
bool UnfoldCallee::runOnModule(Module &M) {
    // get compress CFG
    CCFG = &(getAnalysis<CompressCFG>());

    return false;
} 


// register this pass
char UnfoldCallee::ID = 0;
RegisterPass<UnfoldCallee> A("unfold_callee",  // used in command line as argument
                             "Unfold callee Pass",
                             false,
                             false);
