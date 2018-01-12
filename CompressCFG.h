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
#include "llvm/IR/CFG.h"
#include "llvm/Support/raw_ostream.h"

#include "PrivGraph.h"
#include "CompressCallGraph.h"
#include "ADT.h"
#include <unordered_map>

using namespace privGraph;
using namespace compressCG;

namespace compressCFG {
struct CompressCFG : public ModulePass {
public:
    static char ID;

    // this module
    Module *theModule;

    // compressed call graph
    CompressCG *CCG;

    // constructor
    CompressCFG();

    // a compressed call graph
    /* CompressCG &CCG; */

    //
    unordered_map<Function *, PrivCFG *> funcCFGMap;

    // runOnModule
    virtual bool runOnModule(Module &M);

    //remove "unprivileged" basic blocks from all functions reachable from main.
    void removeUnprivBB(CompressCG &CCG);

    virtual void getAnalysisUsage(AnalysisUsage &AU) const;
private:
    void initializeCFG();
};  // end of CompressCFG

}  // end of namespace compressCFG



#endif  // end of define COMPRESS_CFG_H
