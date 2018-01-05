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

#ifndef COMPRESSED_CALL_GRAPH_H
#define COMPRESSED_CALL_GRAPH_H

using namespace llvm;
/* using namespace privADT; */

namespace compressCG {
struct CompressCG : public ModulePass {
public:
    static char ID;

    // constructor
    CompressCG();

    // do initialization
    /* virtual bool doInitialization(Module &M); */

    // runOnModule
    virtual bool runOnModule(Module &M);

    // to some pass before this one
    virtual void getAnalysisUsage(AnalysisUsage &AU) const;
private:
    void prijtAllFunc(Module &M) const;
    void printCallGraph(Module &M, CallGraph &CG) const;
    void debugHelper(Module &M) const;
};
}

#endif
