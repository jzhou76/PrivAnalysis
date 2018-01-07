/* CallGraph.h
 *
 *     The Automated Privileges Project
 *
 * This file implements PrivCallGraph class and PrivCallGraphNode class.
 *
 * PrivCallGraph only contains nodes corresponding to user-defined functions while ignores library functions.
 *
 * */

#include "PrivCallGraph.h"

using namespace llvm;
using namespace privCallGraph;

// constructor
PrivCallGraph::PrivCallGraph(Module &M) : M(M) {
    // build the original call graph
    for (Module::iterator mi = M.begin(); mi != M.end(); mi++) {
        // skip over library functions
        if (mi->isDeclaration()) continue;

        // check if this is the entry function
        if (mi->getName().equals(ENTRY_FUNC)) entryFunc = &*mi;

        for (Function::iterator fi = mi->begin(); fi != mi->end(); fi++) {
            // iterate over every basic block
            for (BasicBlock::iterator bbi = fi->begin(); bbi != fi->end(); bbi++) {
                CallInst *ci = dyn_cast<CallInst>(&*bbi);
                if (ci != NULL) {
                    Function *callee = dyn_cast<Function>(ci->getCalledValue()->stripPointerCasts());
                    if (callee->isDeclaration()) continue;
                    /* errs() << funcName << " calls " << callee->getName() << "\n"; */
                    // add caller-callee relation into the call graph
                    addCallRelation(&*mi, callee);
                } else {
                    // this function doesn't call any other functions
                    // it's possibe it is not called by any other functions;
                    /* addFunctionNode(&*mi); */
                }
            }
        }
    }

    removeUnreachableFuncs(funcNodeMap[entryFunc]);

    dump();
}

PrivCallGraphNode *PrivCallGraph::getNode(Function *F) {
    return funcNodeMap[F];
}

void PrivCallGraph::addCallRelation(Function *caller, Function *callee) {
    // in case the caller or callee has not been added to the call graph
    PrivCallGraphNode *callerNode = addFunctionNode(caller);
    PrivCallGraphNode *calleeNode = addFunctionNode(callee);

    if (callerNode->hasCallee(calleeNode)) {
        // this call-relation has already existed
        return;
    }
    
    callerNode->callees.insert(calleeNode);
    calleeNode->callers.insert(callerNode);
}

// add a function to the call graph
PrivCallGraphNode *PrivCallGraph::addFunctionNode(Function *F) {
    if (funcNodeMap.find(F) == funcNodeMap.end()) {
        // got a new function
        PrivCallGraphNode *node = new PrivCallGraphNode(*F);
        nodes.insert(node);
        funcNodeMap.insert(pair<Function *, PrivCallGraphNode *>(F, node));
        
        return node;
    }

    return funcNodeMap[F];
}


// remove a node from the call graph
void PrivCallGraph::removeNode(PrivCallGraphNode *node) {
    nodes.erase(node);   
    funcNodeMap.erase(&node->F);

    // delete it all its callees' caller list
    for (auto i = node->callees.begin(); i != node->callees.end(); i++) {
        (*i)->callers.erase(node);
    }

    // delete it from all its callers' callee list
    for (auto i = node->callers.begin(); i != node->callers.end(); i++) {
        (*i)->callees.erase(node);
    }

    // free up the node itself
    delete node;
}

/*
 * removeUnreachableFuncs() removes all functions that can not be reached from main() function
 *
 * Start from the entry function, and do a DFS.
 * */
void PrivCallGraph::removeUnreachableFuncs(PrivCallGraphNode *node) {
    unordered_set<PrivCallGraphNode *> reachable;
    DFS(node, reachable);
    
    // remove all nodes that don't appear in reachable
    for (auto ni = nodes.begin(); ni != nodes.end(); ni++) {
        PrivCallGraphNode *node = *ni;
        if (reachable.find(node) == reachable.end()) {
            // this node is not reachable from entry function; remove it
            removeNode(node);
        }
    }
}
void PrivCallGraph::DFS(PrivCallGraphNode *node, unordered_set<PrivCallGraphNode *> &reachable) {
    if (reachable.find(node) != reachable.end()) return;

    reachable.insert(node);
    for (auto i = node->callees.begin(); i != node->callees.end(); i++) { DFS(*i, reachable); }
}

// check if a function exists in the call graph
bool PrivCallGraph::hasFunction(Function *F) const {
    if (funcNodeMap.find(F) != funcNodeMap.end()) return true;
    else return false;
}

// print out call relations
void PrivCallGraph::dump() const {
    errs() << "\nCalling relations of this program: \n";
    for (auto fni = nodes.begin(); fni != nodes.end(); fni++) {
        /* PrivCallGraphNode *funcNode = *fni; */
        /* Function *f = funcNode->getFunction(); */
        /* errs() << f->getName() << " calls \n"; */
        /* for (auto cni = funcNode->callees.begin(); cni != funcNode->callees.end(); cni++) { */
        /*     errs() << (*cni)->getFunction()->getName() << " "; */
        /* } */
        /* errs() << "\n"; */
        /* errs() << f->getName() << " is called by \n"; */
        /* for (auto cni = funcNode->callers.begin(); cni != funcNode->callees.end(); cni++) { */
        /*     errs() << (*cni)->getFunction()->getName() << " "; */
        /* } */
        /* errs() << "\n"; */
        /* errs() << "\n"; */
        (*fni)->dump();
    }
}

// ===================================
// implementation of PrivCallGraphNode
// ===================================
//

//constructor
PrivCallGraphNode::PrivCallGraphNode(Function &F) : F(F) {}

// check if this Function has some caller
bool PrivCallGraphNode::hasCaller(PrivCallGraphNode *caller) const {
    if (callers.find(caller) != callers.end()) return true;
    else return false;
}

// check if this Function has some callee
bool PrivCallGraphNode::hasCallee(PrivCallGraphNode *callee) const {
    if (callees.find(callee) != callees.end()) return true;
    else return false;
}

Function* PrivCallGraphNode::getFunction() const {
    return &F;
}

/*
 * dump() prints out information of a PrivCallGraphNode.
 * It prints out all callees this function calls, and all callers that call this function.
 * */
void PrivCallGraphNode::dump() const {
    // print callees
    errs() << "Function " << F.getName() << " calls: ";
    for (auto i = callees.begin(); i != callees.end(); i++) {
        errs() << (*i)->getFunction()->getName() << " ";
    }
    if (callees.size() == 0) errs() << "nothing";
    errs() << "\n";

    // prints callers
    errs() << "Function " << F.getName() << " is called by: ";
    for (auto i = callers.begin(); i != callers.end(); i++) {
        errs() << (*i)->getFunction()->getName() << " ";
    }
    if (callers.size() == 0) errs() << "nothing";
    errs() << "\n";
    errs() << "\n";
}

// Is it a good idea to write code like this?
inline void PrivCallGraphNode::addCaller(PrivCallGraphNode *caller) {
    callers.insert(caller);
}
inline void PrivCallGraphNode::addCallee(PrivCallGraphNode *callee) {
    callees.insert(callee);
}
