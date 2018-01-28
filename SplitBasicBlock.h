/** SplitBasicBlock.h
 *
 *     The Automated Privileges Project
 *
 * This pass split basic blocks by CallInst.
 * If a CallInst in a basic blocks calls some function rather than a library function,
 * then we split this basic block after this CallInst.
 *
 * @author Jie Zhou
 * */

#ifndef SPLIT_BB_H
#define SPLIT_BB_H

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

#include "ADT.h"

using namespace llvm;

namespace llvm {
namespace splitBB {
struct SplitBB : public ModulePass {
public:
    static char ID;

    // constructor
    SplitBB();

    virtual bool runOnModule(Module &M);

    /* void getAnalysisUsage(AnalysisUsage &AU) const; */
    
    void dump(Module &M) const;
private:
    void splitFunc(Function &F);


};  // end of struct SplitBB
}  // end of namespace splitBB
}


#endif  // end of SPLIT_BB_H
 
