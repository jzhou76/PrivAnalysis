/* CallGraph.h
 *
 *     The Automated Privileges Project
 *
 * This files privides interfaces to build a call graph of a program.
 * It was created because the CallGraph and CallGraphNode class provided by LLVM doesn't suffice.
 *
 * @author Jie Zhou
 *
 **/

#ifndef PRIV_CALL_GRAPH_H
#define PRIV_CALL_GRAPH_H

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

#include <unordered_set>

#define ENTRY_FUNC "main"

using namespace llvm;
using namespace std;

namespace privCallGraph {
class PrivCallGraphNode;

class PrivCallGraph {
public:
    // constructor
    PrivCallGraph(Module &M);

    // the Module this call graph represents
    Module &M;

    // entry function, aka main
    Function *entryFunc;


    // get the PrivCallGraphNode by its Function
    PrivCallGraphNode *getNode(Function *F);

    // check if a function exists in the call graph
    bool hasFunction(Function *F) const;

    // just add a PrivCallGraphNode for function F
    PrivCallGraphNode *addFunctionNode(Function *F);

    // add caller-callee relation
    void addCallRelation(Function *caller, Function *callee);
    // delete a PrivCallGraphNode and all its relations
    void removeNode(PrivCallGraphNode *node);

    // print call relations of the program
    void dump() const;

    // print all node names and addresses
    void print() const;
private:
    // all functions
    /* unordered_set<Function *> functions; */

    // all PrivCallGraphNodes 
    unordered_set<PrivCallGraphNode *> nodes;

    // map a Function to a PrivCallGraphNode
    map<Function *, PrivCallGraphNode *> funcNodeMap;

    // remove all unreachable functions 
    void removeUnreachableFuncs(PrivCallGraphNode *node);
    void DFS(PrivCallGraphNode *node, unordered_set<PrivCallGraphNode *> &reachable);
    
};  // end of class PrivCallGraph


class PrivCallGraphNode {
public:
    PrivCallGraphNode(Function &F);

    // return corresponding function
    Function* getFunction() const;

    bool hasCaller(PrivCallGraphNode *caller) const;
    bool hasCallee(PrivCallGraphNode *callee) const;

    // print information of this node
    void dump() const;

private:
    friend class PrivCallGraph;
    Function &F;
    unordered_set<PrivCallGraphNode *> callers;  // prev-node
    unordered_set<PrivCallGraphNode *> callees;  // post-node

    inline void addCaller(PrivCallGraphNode *caller);
    inline void addCallee(PrivCallGraphNode *callee);
};
}  // end of privCallGraph namespace

#endif  // PRIV_CALL_GRAPH_H


