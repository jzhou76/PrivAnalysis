/* CompressCFG.h
 *
 *     The Automated Privileges Project
 *
 *
 * @author Jie Zhou
 *
 **/

#ifndef COMPRESS_CFG_H
#define COMPRESS_CFG_H

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "PrivCallGraph.h"

#include "ADT.h"
#include <unordered_map>

namespace compressCFG {
struct CompressCFG : public ModulePass {
public:
    static char ID;
    // constructor
    CompressCFG();

    virtual bool runOnModule(Module &M);

    virtual void getAnalysisUsage(AnalysisUsage &AU) const;
};  // end of CompressCFG

}  // end of namespace compressCFG



#endif  // end of define COMPRESS_CFG_H
