/* CompressCFG.cpp
 *
 *     The Automated Privileges Project
 *
 *
 **/

#include "CompressCFG.h"
#include "CompressCallGraph.h"

using namespace llvm;
using namespace privADT;
using namespace compressCG;
using namespace compressCFG;

// constructor
CompressCFG::CompressCFG() : ModulePass(ID) {}


// run CompressCallGraph first
void CompressCFG::getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequired<CompressCG>();
    AU.addPreserved<CompressCG>();

    AU.setPreservesAll();
}

bool CompressCFG::runOnModule(Module &M) {
    /* CompressCG &CCG = getAnalysis<CompressCG>(); */
}

