/* CompressedCallGraph.h
 *
 *     The Automated Privileges Project
 *
 * PrivAnalysis.h declares data and function types for PrivAnalysis pass.
 *
 * @author Jie Zhou
 *
 **/

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Support/raw_ostream.h"
#include "PrivCallGraph.h"

#include "LocalAnalysis.h"
#include "ADT.h"
#include <unordered_map>

#ifndef COMPRESSED_CALL_GRAPH_H
#define COMPRESSED_CALL_GRAPH_H

using namespace llvm;
using namespace privCallGraph;
using namespace localAnalysis;
using namespace privADT;

namespace compressCG {
struct CompressCG : public ModulePass {
public:
    static char ID;

    // map from function to CAPArray
    FuncCAPTable_t funcCapMap;
    // map from basic block to CAPArray
    BBCAPTable_t bbCapMap;

    // map a fucntion F to functions that use some privilege and are reachable from F
    std::unordered_map<Function *, PrivCallGraphNode *> reachablePrivFunc;

    // constructor
    CompressCG();

    // do initialization
    /* virtual bool doInitialization(Module &M); */

    // runOnModule
    virtual bool runOnModule(Module &M);

    // to some pass before this one
    virtual void getAnalysisUsage(AnalysisUsage &AU) const;
private:
    PrivCallGraph *privCG;

    // remove unreachable functions from funcCapMap and bbCapMap
    void removeUnreachableFuncs(PrivCallGraph *privCG);

    void printAllFunc(Module &M) const;
    void printCallGraph(Module &M, llvm::CallGraph &CG) const;
    void debugHelper(Module &M) const;
};

}  // end of compressCG namespace

#endif  // COMPRESSED_CALL_GRAPH_H
