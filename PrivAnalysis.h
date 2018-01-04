/* PrivAnalysis.h
 *
 *      The Automated Privilege Project
 *
 * PrivAnalysis.h declares data and function types for PrivAnalysis pass.
 *
 * @author Jie Zhou
 *
 **/

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LLVMContext.h"

#ifndef PRIV_ANALYSIS_H
#define PRIV_ANALYSIS_H

using namespace llvm;

namespace privAnalysis {
struct PrivAnalysis : public ModulePass {
public:
    static char ID;
    
    // constructor
    PrivAnalysis(); 

    // do initialization
    /* virtual bool doInitialization(Module &M); */

    // runOnModule
    virtual bool runOnModule(Module &M);

    // for pass interaction
    virtual void getAnalysisUsage(AnalysisUsage &Info) const;

}; // end of struct PrivAnalysis
} // end of namespace PrivAnalysis


#endif

