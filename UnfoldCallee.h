/* UnfoldCallee.h
 *
 *     The Automated Privileges Project
 *
 * This Pass unfolds all callees in a program by making them inline. 
 *
 * @author Jie Zhou
 *
 * */


#ifndef UNFOLD_CALLEE_H
#define UNFOLD_CALLEE_H

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

#include "CompressCFG.h"

using namespace llvm;
using namespace compressCFG;


namespace unfoldCallee {
struct UnfoldCallee : public ModulePass {
public:
    static char ID;
    
    // CompressCFG
    CompressCFG *CCFG;

    //constructor
    UnfoldCallee();

    virtual void getAnalysisUsage(AnalysisUsage &AU) const;

    // runOnModule
    virtual bool runOnModule(Module &M);

};  // edn of Pass UnfoldCallee

}  // end of namespace unfoldCallee

#endif  // end of UNFOLD_CALLEE_H
