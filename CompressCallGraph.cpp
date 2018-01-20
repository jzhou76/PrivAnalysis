/** CompressCallGraph.cpp
 *
 *      The Automated Privileges Project
 *
 * This file implements the building of compressed call graphs, which only keep functions that use privileges.
 *
 *
 * */


#include "CompressCallGraph.h"

using namespace llvm;
using namespace compressCG;
using namespace privGraph;
using namespace localAnalysis;
using namespace privADT;

// constructor
CompressCG::CompressCG() : ModulePass(ID) {
    /* errs() << "Hello from CompressCG's constructor!\n"; */
}

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

    theModule = &M;
    /* CallGraph CG(M); */
    /* CG.dump(); */

    // get LocalAnalysis's result
    LocalAnalysis &LA = getAnalysis<LocalAnalysis>();
    funcCapMap = LA.FuncCAPTable;
    bbCapMap = LA.BBCAPTable;

    // build the original call graph
    privCG = new PrivCallGraph(M);
    /* privCG->print(); */

    // remove unreachable functions and blocks from funcCapMap
    removeUnneededFromCapTable();

    // get all priv-functions a function can reach
    getReachablePrivFunc();

    // remove functions that neither use nor reach any privileges
    removeUnprivFunc();


    // compute SCCs
    computeSCCs();

    debugHelper(M);
    return false;
}

/*
 * removeUnneededFromCapTable() removes unreachable functions from funcCapMap.
 * It also remove basic blocks in those deleted functions, though the function name omits that.
 *
 * Actually, we don't need call this function to get a correct result for the whole analysis...
 * */
void CompressCG::removeUnneededFromCapTable() {
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

/*
 * funcReachPrivFunc() computes which priv-functions that a function can reach.
 * For those functions that never reach a priv-function, they can be ignored when inlining callees in the
 * control flow grap.
 * */
void CompressCG::getReachablePrivFunc() {
    for (unordered_set<PrivCallGraphNode *>::iterator ni = privCG->nodes.begin(), nie = privCG->nodes.end();
            ni != nie; ++ni) {
        unordered_set<PrivCallGraphNode *> reachable, visited;
        DFS(*ni, reachable, visited);
        if (reachable.size() != 0) {
            reachablePrivFunc.insert(pair<Function *, unordered_set<PrivCallGraphNode *>>(
                        (*ni)->getFunction(), unordered_set<PrivCallGraphNode *>(reachable.begin(), reachable.end())));
        }
        // add reachable
    }
}
/*
 * This DFS is a helper function for getReachablePrivFunc().
 * */
void CompressCG::DFS(PrivCallGraphNode *node, unordered_set<PrivCallGraphNode *> &reachable,
                            unordered_set<PrivCallGraphNode *> &visited) {
    if (visited.find(node) != visited.end()) return;
    
    visited.insert(node);
    // if this furctions uses some privileges, add it to reachable
    if (funcCapMap.find(node->getFunction()) != funcCapMap.end()) reachable.insert(node);

    for (auto i = node->callees.begin(); i != node->callees.end(); i++) { DFS(*i, reachable, visited); }
}

// check if a function can reach a priv-function
bool CompressCG::canReachPrivFunc(Function *F) const {
    return reachablePrivFunc.find(F) != reachablePrivFunc.end();
}

/*
 * removeUnprivFunc() removes "unprileged functions". 
 * A function is unprivileged if it neither uses any privileges nor reaches any function which 
 * uses privileges.
 * */
void CompressCG::removeUnprivFunc() {
    unordered_set<PrivCallGraphNode *> unprivFuncNodes;
    for (unordered_set<PrivCallGraphNode *>::iterator cgni = privCG->nodes.begin(); 
            cgni != privCG->nodes.end(); cgni++) {
        PrivCallGraphNode *node = *cgni;
        if (reachablePrivFunc.find(node->getFunction()) == reachablePrivFunc.end()) {
            // this function is unprivileged
            unprivFuncNodes.insert(node);
        }
    }

    for (auto ni = unprivFuncNodes.begin(); ni != unprivFuncNodes.end(); ni++) {
        privCG->removeNode(*ni);
    }


    /* // equivalent implementation */
    /* for (auto i = privCG->nodes.begin(); i != privCG->nodes.end(); i++) { */
    /*     if (reachablePrivFunc.find(*i->getFunction()) == reachablePrivFunc.end()) privCG->removeNode(*i++); */
    /*     else i++; */
    /* } */

}

/*
 * computeSCCs() gets all SCCs in this call graph.
 * We only consider functions involved in recursive calls as nodes of a SCC, alghough strictly speaking
 * every node in the call graph is a SCC.
 *
 * Currently we implement Kosaraju's algorithm to find all SCCs.
 * Tarjan's algorithm is faster: it only does one DFS. However, it's tricker to implement than Kosaraju's algorithm.
 * */
void CompressCG::computeSCCs() {
    vector<PrivCallGraphNode *> order;
    unordered_set<PrivCallGraphNode *> visited;
    
    forwardDFS(privCG->getNode(privCG->entryFunc), visited, order);

    errs() << "functions in the stack: \n";
    for (int i = order.size() - 1; i >= 0; i--) {
        errs() << order[i]->getFunction()->getName() << " ";
    }
    errs() << "\n";

    unordered_set<PrivCallGraphNode *> processed;
    while (!order.empty()) {
        PrivCallGraphNode *node = order.back();
        order.pop_back();

        //skip processed node
        if (processed.find(node) != processed.end()) continue;

        // if hasn't processed this node
        visited.clear();
        backwardDFS(node, visited, processed);

        // if there is only one 
        if (visited.size() == 1 && !(*visited.begin())->hasCallee(*visited.begin())) continue;

        PrivCGSCC *cgscc = new PrivCGSCC;
        for (PrivCallGraphNode *node : visited) {
            Function *f = node->getFunction();
            cgscc->funcs.insert(f);
            privCG->funcSCCMap.insert(pair<Function *, PrivCGSCC *>(f, cgscc));
            /* processed.insert(node); */
        }
        // collect all privileges reachable from this SCC
        cgscc->collectCaps(funcCapMap);
    }

    // print SCCs
    /* privCG->printSCCs(); */
}

// a helper function for Kosaraju's algorithm
void CompressCG::forwardDFS(PrivCallGraphNode *node, unordered_set<PrivCallGraphNode *> &visited, 
                                    vector<PrivCallGraphNode *> &order) {
    if (visited.find(node) != visited.end()) return;
    
    visited.insert(node);
    for (PrivCallGraphNode *callee : node->callees) forwardDFS(callee, visited, order);

    order.push_back(node);
}

void CompressCG::backwardDFS(PrivCallGraphNode *node, unordered_set<PrivCallGraphNode *> &visited,
                                unordered_set<PrivCallGraphNode *> &processed) {
    if (visited.find(node) != visited.end()) return;
    
    visited.insert(node);
    processed.insert(node);
    for (PrivCallGraphNode *caller : node->callers) {
        if (processed.find(caller) == processed.end()) backwardDFS(caller, visited, processed);

    }

}

void CompressCG::debugHelper(Module &M) const {
    /* printAllFunc(M); */
    /* printReachablePrivFunc(); */
   
    // dump calling relationship
    privCG->dump();
}
    
// print all Function names and their addresses
void CompressCG::printAllFunc(Module &M) const {
    errs() << "all functions:\n";
    for (Module::iterator mi = M.begin(); mi != M.end(); mi++) {
        errs() << mi->getName() << ": " << &*mi << "\n";
    }
}

void CompressCG::printReachableFunc() const {
    errs() << "reachable functions from main:\n";
    for (auto i = privCG->nodes.begin(); i != privCG->nodes.end(); i++) {
        errs() << (*i)->getFunction()->getName() << " ";
    }
    errs() << "\n";
}
void CompressCG::printReachablePrivFunc() const {
    for (auto rpfi = reachablePrivFunc.begin(); rpfi != reachablePrivFunc.end(); rpfi++) {
        errs() << "Function " << rpfi->first->getName() << " can reach: ";
        for (auto fi = rpfi->second.begin(); fi != rpfi->second.end(); fi++) {
            errs() << (*fi)->getFuncName() << " ";
        }
        errs() << "\n";
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
