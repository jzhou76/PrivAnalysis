/** CompressCallGraph.cpp
 *
 *      The Automated Privileges Project
 *
 * This file implements the building of a compressed call graph, which only keeps functions that use privileges.
 *
 * This pass keeps a copy of the original call graph of the target program. 
 * It also has compressed call graphs for all functions. A compressed call graph for a function 
 * has a source which represents the function;  each node in the compressed call graph 
 * except the source is some function that 
 *  1. uses at least one privilege
 *  2. reachable from the source function in the original call graph. 
 * */


#include "CompressCallGraph.h"
#include <algorithm>

using namespace llvm;
using namespace compressCG;
using namespace privCallGraph;
using namespace localAnalysis;
using namespace privADT;

// constructor
CompressCG::CompressCG() : ModulePass(ID) {}

/* 
 * Speficy pass(es) that should be run before CompressCG:  LocalAnalysis
 * */
void CompressCG::getAnalysisUsage(AnalysisUsage &AU) const {
    // run LocalAnalysis first
    AU.addPreserved<LocalAnalysis>();
    AU.addRequired<LocalAnalysis>();

    // doesn't transform program
    AU.setPreservesAll();
}


/*
 * runOnModule: entrance of this Module Pass
 * */
bool CompressCG::runOnModule(Module &M) {
    errs() << "Hello from CompressCG Pass!\n";

    CallGraph CG(M);
    /* CG.dump(); */
    for (auto i = CG.begin(); i != CG.end(); i++) {
        if (i->first == NULL) {
            errs() << "found an NULL function\n";
        }
        /* const Function *f = dyn_cast<Function>(i->first); */
        /* if (f != NULL) errs() << "YES IT IS\n"; */
        /* else errs() << "NO\n"; */
    }

    /* errs() << "callingNode dump:\n"; */
    CallGraphNode *callingNode = CG.getExternalCallingNode();
    for (auto cni = callingNode->begin(); cni != callingNode->end(); cni++) cni->second->dump();
    /* callingNode->dump(); */
    /* errs() << "callsNode dump:\n"; */
    /* CallGraphNode *callsNode = CG.getCallsExternalNode(); */
    /* callsNode->dump(); */


    // get LocalAnalysis's result
    LocalAnalysis &LA = getAnalysis<LocalAnalysis>();
    funcCapMap = LA.FuncCAPTable;
    bbCapMap = LA.BBCAPTable;

    // build the original call graph
    privCG = new PrivCallGraph(M);
    /* privCG->print(); */

    // remove unreachable functions and blocks
    removeUnreachableFuncs(privCG);
    
    debugHelper(M);
    return false;
}

/*
 * removeUnreachableFuncs() removes unreachable functions from funcCapMap.
 * It also remove basic blocks in those deleted functions, though the function name omits that.
 *
 * Actually, we don't need call this function to get correct a result for the whole analysis...
 * */
void CompressCG::removeUnreachableFuncs(PrivCallGraph *privCG) {
    // remove functions
    for (auto fcmi = funcCapMap.begin(); fcmi != funcCapMap.end();) {
        if (!privCG->hasFunction(fcmi->first)) funcCapMap.erase(fcmi++);
        else fcmi++;
    }

    // remove basic blocks in the just deleted functions
    for (auto bbcmi = bbCapMap.begin(); bbcmi != bbCapMap.end();) {
        if (!privCG->hasFunction(bbcmi->first->getParent())) bbCapMap.erase(bbcmi++);
        else bbcmi++;
    }
}


void CompressCG::debugHelper(Module &M) const {
    /* printAllFunc(M); */

}
    
// print all Function names and their addresses
void CompressCG::printAllFunc(Module &M) const {
    errs() << "all functions:\n";
    for (Module::iterator mi = M.begin(); mi != M.end(); mi++) {
        errs() << mi->getName() << ": " << &*mi << "\n";
    }
}
void CompressCG::printCallGraph(Module &M, llvm::CallGraph &CG) const {
    /* errs() << "call relations:\n"; */
    /* errs() << "external nodes:\n"; */
    errs() << "print call graph nodes\n";
    for (auto mi = M.begin(); mi != M.end(); mi++) {
        if (mi->getName().equals("main") == false) continue;
        errs() << mi->getName() << " gets called ";
        llvm::CallGraphNode *cgn = CG[&*mi];
        /* cgn->dump(); */
        errs() << cgn->getNumReferences() << " times: ";
        llvm::CallGraphNode *callee = (*cgn)[0];
        Function *f = callee->getFunction();
        if (f == NULL) {
            errs() << "f is null\n";
        }
        errs() << "\n";
    }
}


char CompressCG::ID = 0;
static RegisterPass<CompressCG> A("compress_cg",  // name used as an arugment for opt
                                    "Compress Call Gragh pass",
                                    false,
                                    false);  // analysis pass
