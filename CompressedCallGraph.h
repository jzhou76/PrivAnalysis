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
#include "llvm/Support/raw_ostream.h"

#ifndef COMPRESSED_CALL_GRAPH_H
#define COMPRESSED_CALL_GRAPH_H

using namespace llvm;
/* using namespace privADT; */

namespace compressedCG {
struct CompressedCG : public ModulePass {
public:
    static char ID;

    int ccgNum = 10;
    // constructor
    CompressedCG();

    // do initialization
    /* virtual bool doInitialization(Module &M); */

    // runOnModule
    virtual bool runOnModule(Module &M);

    virtual bool getAnalysisUsage(AnalysisUsage &AU) const;
};
}

#endif
