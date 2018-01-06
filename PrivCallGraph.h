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
#include "llvm/Support/raw_ostream.h"

#include <unordered_set>

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

    map<Function *, PrivCallGraphNode *> funcNodeMap;
    map<PrivCallGraphNode *, Function *> nodeFuncMap;

    void addNode(Function *F);

    void dump();
private:
    // all functions
    unordered_set<Function *> functions;
    // all PrivCallGraphNodes 
    unordered_set<PrivCallGraphNode *> nodes;

    bool hasFunction(Function *F) const;

    
};  // end of class PrivCallGraph

class PrivCallGraphNode {
public:
    PrivCallGraphNode(Function &F);

    // return corresponding function
    Function* getFunction() const;

    // print information of this node
    void dump();
private:
    Function &F;
    unordered_set<PrivCallGraphNode*> callers;  // prev-node
    unordered_set<PrivCallGraphNode*> calees;  // post-node
};
}  // end of privCallGraph namespace

#endif  // PRIV_CALL_GRAPH_H


