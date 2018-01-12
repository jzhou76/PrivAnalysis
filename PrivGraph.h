/* CallGraph.h
 *
 *     The Automated Privileges Project
 *
 * This files privides interfaces to build a call graph of a program and control flow graph for all functions.
 *
 * @author Jie Zhou
 *
 **/

#ifndef PRIV_GRAPH_H
#define PRIV_GRAPH_H

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CFG.h"
#include "llvm/Support/raw_ostream.h"

#include "ADT.h"
#include <set>
#include <unordered_set>
#include <unordered_map>

#define ENTRY_FUNC "main"

using namespace llvm;
using namespace std;
using namespace privADT;

namespace privGraph {
class PrivCallGraphNode;

/*
 * class: PrivCallGraph
 *
 * It was created because the CallGraph and CallGraphNode class provided by LLVM doesn't suffice.
 * */
class PrivCallGraph {
public:
    // constructor
    PrivCallGraph(Module &M);

    // the Module this call graph represents
    Module &M;

    // all PrivCallGraphNodes 
    unordered_set<PrivCallGraphNode *> nodes;

    // map a Function to a PrivCallGraphNode
    unordered_map<Function *, PrivCallGraphNode *> funcNodeMap;

    // entry function, aka main
    Function *entryFunc;

    // map a basic block and the call instructions in it; because of SplitBB, there is at most one CallInst in a BB
    unordered_map<BasicBlock *, CallInst *> bbInstMap;
    // map a call instruction to the function it calls
    unordered_map<CallInst *, Function *> callinstFuncMap;

    // get the PrivCallGraphNode by its Function
    PrivCallGraphNode *getNode(Function *F);

    // check if a function exists in the call graph
    bool hasFunction(Function *F) const;


    // add caller-callee relation
    void addCallRelation(Function *caller, Function *callee);
    // delete a PrivCallGraphNode and all its relations
    void removeNode(PrivCallGraphNode *node);

    static void DFS(PrivCallGraphNode *node, unordered_set<PrivCallGraphNode *> &reachable);

    // print call relations of the program
    void dump() const;

    // print all node names and addresses
    void print() const;
private:
    // just add a PrivCallGraphNode for function F
    PrivCallGraphNode *addFunctionNode(Function *F);

    // ignore library function, priv_raise, and priv_lower
    bool isIgnorable(Function *F) const;

    // remove all unreachable functions 
    void removeUnreachableFuncs(PrivCallGraphNode *node);
    
};  // end of class PrivCallGraph


/*
 * class: PrivCallGraphNode
 * */
class PrivCallGraphNode {
public:
    PrivCallGraphNode(Function &F);

    Function &F;
    unordered_set<PrivCallGraphNode *> callers;  // prev-node
    unordered_set<PrivCallGraphNode *> callees;  // post-node

    // return corresponding function
    Function* getFunction() const;
    // return Function name
    StringRef getFuncName() const;

    bool hasCaller(PrivCallGraphNode *caller) const;
    bool hasCallee(PrivCallGraphNode *callee) const;

    // print information of this node
    void dump() const;

private:
    friend class PrivCallGraph;

    inline void addCaller(PrivCallGraphNode *caller);
    inline void addCallee(PrivCallGraphNode *callee);
};


/*
 * class: PrivCFG
 * */
class PrivCFGNode;
class PrivCFG {
public:
    PrivCFG(Function &F);

    // represent this function
    Function &F;

    // entry block
    BasicBlock *entry;

    // map a basic block to a PRVCFGNode; 
    unordered_map<BasicBlock *, PrivCFGNode *> bbNodeMap;

    // this one will be set later in CompressCFG
    /* map<PrivCFGNode *, CAPArray_t> privNodeCapMap; */

    // add new predecessor - successor relation
    void addInheritance(BasicBlock *from, BasicBlock *to);
    void removeBB(BasicBlock *bb);

    // add a node for a basic block
    PrivCFGNode *addBBNode(BasicBlock *bb);

    // remove a node from this CFG
    void removeNode(PrivCFGNode *node);

    // print out information of all basic blocks of a function
    void dump() const;
private:
};  // end of class PrivCFG

/*
 * class PrivCFGNode
 * */
class PrivCFGNode {
public:
    // constructor
    PrivCFGNode(BasicBlock *bb);

    // this basibc block
    BasicBlock *bb;

    // privileges this basic block uses
    CAPArray_t caps;

    // predecessors
    set<PrivCFGNode *> predecessors;
    set<PrivCFGNode *> successors;

    StringRef getBBName() const;
    // dump inheritance relation of this basic block
    void dump() const;

};  // end of class PrivCFGNode
}  // end of privCallGraph namespace

#endif  // end of namespace PRIV_GRAPH


