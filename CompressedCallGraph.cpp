/** CompressedCallGraph.cpp
 *
 *      The Automated Privileges Project
 *
 * This file implements the building of a compressed call graph, which only keeps functions that use privileges.
 * */

#include "CompressedCallGraph.h"

using namespace llvm;
using namespace compressedCG;

// constructor
CompressedCG::CompressedCG() : ModulePass(ID) {}

// runOnModule
bool CompressedCG::runOnModule(Module &M) {
    errs() << "Hello from CompressedCG Pass!\n";
    ccgNum = 20;
    return false;
}

char CompressedCG::ID = 0;
static RegisterPass<CompressedCG> A("compressed_cg",  // name used as an arugment for opt
                                    "Compressed Call Gragh generator pass",
                                    false,
                                    false);  // analysis pass
