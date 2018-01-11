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

bool CompressCFG::runOnModule(Module &M) {
    errs() << "Hello from CompressCFG Pass!\n";
    CompressCG &CCG = getAnalysis<CompressCG>();
    removeUnprivBB(CCG);

    return false;
}

/*
 * remove "unprivileged" basic blocks from all functions reachable from main.
 * A basic block is regarded as "privileged" if 
 * 1. it uses some privileges directly, or
 * 2. It calls some function which can reach a function using some privileges.
 *
 * Question: should entry block be skipped?
 * */
void CompressCFG::removeUnprivBB(CompressCG &CCG) {
    PrivCallGraph *privCG = CCG.privCG;
    for (unordered_map<Function *, PrivCallGraphNode *>::iterator fnmi = privCG->funcNodeMap.begin();
            fnmi != privCG->funcNodeMap.end(); fnmi++) {
        Function *F = fnmi->first;
        /* if (!F->getName().equals("main")) continue; */
        for (Function::iterator fi = F->begin(); fi != F->end(); fi++) {
            BasicBlock *bb = &*fi;
            // for each basic block
            // we keep the entrance block of a function
            if (bb == &(F->getEntryBlock())) {
                /* errs() << "skip the entry block of function " << F->getName() << "\n"; */
                continue;
            }
            
            // if this block uses some privilege
            if (CCG.bbCapMap.find(bb) != CCG.bbCapMap.end()) continue;
            else if (CCG.canReachPrivFunc(privCG->callinstFuncMap[privCG->bbInstMap[bb]])) continue;
            else {
                // this basic block neither use any privilegess nor reach priv-functions through CallInst
                for (BasicBlock *predecessor : predecessors(bb)) {
                    for (BasicBlock *successor : successors(bb)) {

                    }
                }
            }
        }
    }
}

char CompressCFG::ID = 0;
static RegisterPass<CompressCFG> A("compress_cfg",  // name used as an arugment for opt
                                    "Compress Control Flow Gragh pass",
                                    false,
                                    false);  // analysis pass
