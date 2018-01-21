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
using namespace privGraph;
using namespace compressCG;
using namespace compressCFG;

// constructor
CompressCFG::CompressCFG() : ModulePass(ID) {}


// run CompressCallGraph first
void CompressCFG::getAnalysisUsage(AnalysisUsage &AU) const {
    // get the compressed call graph first
    AU.addRequired<CompressCG>();
    AU.addPreserved<CompressCG>();

    AU.setPreservesAll();
}

// entrance of this Pass
bool CompressCFG::runOnModule(Module &M) {
    errs() << "Hello from CompressCFG Pass!\n";

    // get the Compressed Call Graph for this module
    CCG = &(getAnalysis<CompressCG>());
    theModule = CCG->theModule;

    initializeCFG();
    removeUnprivBB(*CCG);

    return false;
}

/*
 * initializeCFG()
 * This function creates nodes for all basic blocks of a function.
 * */
void CompressCFG::initializeCFG() {
    for (Module::iterator mi = theModule->begin(); mi != theModule->end(); mi++) {
        Function *F = &*mi;
        PrivCFG *privCFG = new PrivCFG(*F);
        // add Function - PrivCFG pair
        funcCFGMap.insert(pair<Function *, PrivCFG *>(F, privCFG));

        // Mark which privileges a basic block uses.
        // It makes more sense semantically if this step is done in the contructor of a PrivCFG.
        // However, PrivCFG is defined an another source file which is not a Pass. 
        // Thus, it's not convenient to get the BasicBlock - privileges map from LocalAnalysis.
        for (Function::iterator fi = F->begin(); fi != F->end(); fi++) {
            BasicBlock *bb = &*fi;
            if (CCG->bbCapMap.find(bb) != CCG->bbCapMap.end()) privCFG->bbNodeMap[bb]->caps = CCG->bbCapMap[bb];
        }
    }
}

/*
 * remove "unprivileged" basic blocks from all functions reachable from main.
 * A basic block is regarded as "privileged" if 
 * 1. it uses some privileges directly, or
 * 2. It calls some function which can reach a function using some privileges.
 *
 * Question1: should entry block be skipped? 
 * Yes, otherwise there might be multiple starting points of a function, which makes things messy.
 *
 * Question2: should the last block be kept if it doesn't use any privilege?
 * Yes, otherwise there might be multiple ending points of a function, which makes things messy.
 *
 * */
void CompressCFG::removeUnprivBB(CompressCG &CCG) {
    PrivCallGraph *privCG = CCG.privCG;
    for (unordered_map<Function *, PrivCFG *>::iterator fcfgmi = funcCFGMap.begin();
            fcfgmi != funcCFGMap.end(); fcfgmi++) {
        Function *F = fcfgmi->first;
        PrivCFG *privCFG = funcCFGMap[F];

        for (Function::iterator fi = F->begin(); fi != F->end(); fi++) {
            BasicBlock *bb = &*fi;
            // for each basic block
            
            // keep the entry block of a function
            if (bb == privCFG->entry || bb == privCFG->end) {
                continue;
            }
            
            if (!(CCG.bbCapMap.find(bb) != CCG.bbCapMap.end() ||
                 (CCG.canReachPrivFunc(privCG->callinstFuncMap[privCG->bbInstMap[bb]])))) {
                privCFG->removeNode(privCFG->bbNodeMap[bb]);
            }
        }
    }


    Function *F = theModule->getFunction("main");
    F->dump();
    errs() << "after removeUnprivBB():\n";
    for (auto funcCFG : funcCFGMap) {
        if (!funcCFG.first->getName().equals("main")) continue;
        funcCFG.second->dump();
    }

}

char CompressCFG::ID = 0;
static RegisterPass<CompressCFG> A("compress_cfg",  // name used as an arugment for opt
                                    "Compress Control Flow Gragh pass",
                                    false,
                                    false);  // analysis pass
