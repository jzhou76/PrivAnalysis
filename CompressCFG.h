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

    // basic block - privileges map


    // constructor
    CompressCFG();

    // functions and their corresponding CFGs
    unordered_map<Function *, PrivCFG *> funcCFGMap;

    // basic block and their corresponding SCC if it is in a SCC
    unordered_map<BasicBlock *, PrivCFGSCC *> bbSCCMap;

    // runOnModule
    virtual bool runOnModule(Module &M);

    //remove "unprivileged" basic blocks from all functions reachable from main.
    void removeUnprivBB();

    // for debuging
    void debuger() ;

    virtual void getAnalysisUsage(AnalysisUsage &AU) const;
private:
    // create nodes for all basic blocks
    void initializeCFG();

    // find all SCCs of a function
    void computeSCCs();
    void forwardDFS(PrivCFGNode *node, unordered_set<PrivCFGNode *> &visited, vector<PrivCFGNode *> &order);
    void backwardDFS(PrivCFGNode *node, unordered_set<PrivCFGNode *> &visited, unordered_set<PrivCFGNode *> &processed);
    void collectSCCCaps(PrivCFGSCC &scc);

    // find SCCs of this 
};  // end of CompressCFG

}  // end of namespace compressCFG



#endif  // end of define COMPRESS_CFG_H
