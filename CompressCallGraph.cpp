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
#include "LocalAnalysis.h"

using namespace llvm;
using namespace compressCG;
using namespace localAnalysis;

// constructor
CompressCG::CompressCG() : ModulePass(ID) {}

/* 
 * Speficy pass(es) that should be run before CompressCG:  LocalAnalysis
 * */
void CompressCG::getAnalysisUsage(AnalysisUsage &AU) const {
    // run LocalAnalysis first
    /* AU.addRequired<LocalAnalysis>(); */

    // doesn't transform program
    AU.setPreservesAll();
}


// runOnModule
bool CompressCG::runOnModule(Module &M) {
    errs() << "Hello from CompressCG Pass!\n";

    debugHelper(M);
    return false;
}

void CompressCG::debugHelper(Module &M) const {
    /* printAllFunc(M); */
    
    CallGraph CG(M);
    /* printCallGraph(M, CG); */
    /* CG.print(errs()); */
    CG.dump();
}
void CompressCG::printAllFunc(Module &M) const {
    errs() << "all functions:\n";
    for (Module::iterator mi = M.begin(); mi != M.end(); mi++) {
        errs() << mi->getName() << "\n";
    }
        
}
void CompressCG::printCallGraph(Module &M, CallGraph &CG) const {
    /* errs() << "call relations:\n"; */
    /* errs() << "external nodes:\n"; */
    errs() << "print call graph nodes\n";
    for (auto mi = M.begin(); mi != M.end(); mi++) {
        errs() << mi->getName() << " gets called ";
        CallGraphNode *cgn = CG[&*mi];
        /* cgn->dump(); */
        errs() << cgn->getNumReferences() << " times: ";
        CallGraphNode *callee = (*cgn)[1];
        Function *f = callee->getFunction();
        errs() << "====";
        errs() << f->getName();
        /* for (unsigned i = 0; i < cgn->getNumReferences(); i++) { */
            /* errs() << ((*cgn)[0])->getFunction()->getName() <<" "; */
        /* } */
        errs() << "\n";
    }
}

char CompressCG::ID = 0;
static RegisterPass<CompressCG> A("compress_cg",  // name used as an arugment for opt
                                    "Compress Call Gragh pass",
                                    false,
                                    false);  // analysis pass
