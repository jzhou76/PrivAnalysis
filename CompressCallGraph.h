/** CompressCallGraph.h
 *
 *     The Automated Privileges Project
 *
 * PrivAnalysis.h declares data and function types for CompressCallGraph pass.
 *
 * This Pass keeps a copy of the original call graph of the target program. 
 * It also has compressed call graphs for all functions. A compressed call graph for a function 
 * has a source which represents the function;  each node in the compressed call graph 
 * except the source is some function that 
 *  1. uses at least one privilege
 *  2. reachable from the source function in the original call graph. 
 *
 * @author Jie Zhou
 *
 **/

#ifndef COMPRESSED_CALL_GRAPH_H
#define COMPRESSED_CALL_GRAPH_H

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/ADT/SCCIterator.h"
#include "llvm/Support/raw_ostream.h"

#include "PrivGraph.h"
#include "LocalAnalysis.h"
#include "ADT.h"
#include <unordered_map>
#include <vector>


using namespace llvm;
using namespace privGraph;
using namespace localAnalysis;
using namespace privADT;

namespace compressCG {
struct CompressCG : public ModulePass {
public:
    static char ID;
 
    // this module
    Module *theModule;

    // map from function to CAPArray
    FuncCAPTable_t *funcCapMap;
    // map from basic block to CAPArray
    BBCAPTable_t *bbCapMap;

    // original call graph
    PrivCallGraph *privCG;

    // map a fucntion F to functions that use some privilege and are reachable from F
    std::unordered_map<Function *, unordered_set<PrivCallGraphNode *>> reachablePrivFunc;

    // constructor
    CompressCG();

    // do initialization
    /* virtual bool doInitialization(Module &M); */

    // runOnModule
    virtual bool runOnModule(Module &M);

    // to some pass before this one
    virtual void getAnalysisUsage(AnalysisUsage &AU) const;

    // check if F can reach a priv-function
    bool canReachPrivFunc(Function *F) const;

private:
    // remove unreachable functions from funcCapMap and bbCapMap
    void removeUnneededFromCapTable();

    // get which priv-functions a function can reach
    void getReachablePrivFunc();
    // helper function for getReachablePrivFunc()
    void DFS(PrivCallGraphNode *node, unordered_set<PrivCallGraphNode *> &reachable, 
                                     unordered_set<PrivCallGraphNode *> &visited);

    // remove an unpriv-function
    void removeUnprivFunc();

    // compute SCCs of the Call Graph
    void computeSCCs();
    void forwardDFS(PrivCallGraphNode *node, unordered_set<PrivCallGraphNode *> &visited, 
                             vector<PrivCallGraphNode *> &order);
    void backwardDFS(PrivCallGraphNode *node, unordered_set<PrivCallGraphNode *> &visited,
                        unordered_set<PrivCallGraphNode *> &processed);
    void collectSCCCaps(PrivCGSCC &scc);


    // debuger helpers
    void printAllFunc(Module &M) const;
    void printCallGraph(Module &M, llvm::CallGraph &CG) const;
    void printReachablePrivFunc() const;
    void printReachableFunc() const;
    void debugHelper(Module &M) const;
};

}  // end of compressCG namespace

#endif  // COMPRESSED_CALL_GRAPH_H
