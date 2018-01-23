/* CompressCFG.cpp
 *
 *     The Automated Privileges Project
 *
 *
 **/

#include "CompressCFG.h"
#include "CompressCallGraph.h"

using namespace llvm;
using namespace privADT;
using namespace privGraph;
using namespace compressCG;
using namespace compressCFG;

// constructor
CompressCFG::CompressCFG() : ModulePass(ID) {}


// run CompressCallGraph first
void CompressCFG::getAnalysisUsage(AnalysisUsage &AU) const {
    // get the compressed call graph first
    AU.addRequired<CompressCG>();
    AU.addPreserved<CompressCG>();

    AU.setPreservesAll();
}

// entrance of this Pass
bool CompressCFG::runOnModule(Module &M) {
    errs() << "Hello from CompressCFG Pass!\n";

    // get the Compressed Call Graph for this module
    CCG = &(getAnalysis<CompressCG>());
    theModule = CCG->theModule;

    if (&M != theModule) errs() << "WTF????\n";
    initializeCFG();

    // remove unprivileged basic blocks
    removeUnprivBB();

    // get all SCCs of each CFG
    computeSCCs();

    debuger();

    return false;
}

/*
 * initializeCFG()
 * This function creates nodes for all basic blocks of a function.
 * */
void CompressCFG::initializeCFG() {
    /* for (Module::iterator mi = theModule->begin(); mi != theModule->end(); mi++) { */
    for (PrivCallGraphNode *funcNode : CCG->privCG->nodes) {
        Function *F = funcNode->getFunction();

        PrivCFG *privCFG = new PrivCFG(*F);
        // add Function - PrivCFG pair
        funcCFGMap.insert(pair<Function *, PrivCFG *>(F, privCFG));

        // Mark which privileges a basic block uses.
        // It makes more sense semantically if this step is done in the contructor of a PrivCFG.
        // However, PrivCFG is defined an another source file which is not a Pass. 
        // Thus, it's not convenient to get the BasicBlock - privileges map from LocalAnalysis.
        for (Function::iterator fi = F->begin(); fi != F->end(); fi++) {
            BasicBlock *bb = &*fi;
            if (CCG->bbCapMap->find(bb) != CCG->bbCapMap->end()) privCFG->bbNodeMap[bb]->caps = (*CCG->bbCapMap)[bb];
        }
    }
}

/*
 * remove "unprivileged" basic blocks from all functions reachable from main.
 * A basic block is regarded as "privileged" if 
 * 1. it uses some privileges directly, or
 * 2. It calls some function which can reach a function using some privileges.
 *
 * Question1: should entry block be kept? 
 * Yes, otherwise there might be multiple starting points of a function, which makes things messy.
 *
 * Question2: should the last block be kept if it doesn't use any privilege?
 * Yes, otherwise there might be multiple ending points of a function, which makes things messy.
 *
 * */
void CompressCFG::removeUnprivBB() {
    PrivCallGraph *privCG = CCG->privCG;
    for (unordered_map<Function *, PrivCFG *>::iterator fcfgmi = funcCFGMap.begin();
            fcfgmi != funcCFGMap.end(); fcfgmi++) {
        Function *F = fcfgmi->first;
        PrivCFG *privCFG = funcCFGMap[F];

        for (Function::iterator fi = F->begin(); fi != F->end(); fi++) {
            BasicBlock *bb = &*fi;
            // for each basic block
            
            // keep the entry block of a function
            if (bb == privCFG->entry || bb == privCFG->end) {
                continue;
            }
            
            if (!(CCG->bbCapMap->find(bb) != CCG->bbCapMap->end() ||
                 (CCG->canReachPrivFunc(privCG->getCalleeFromBB(bb))))) {
                privCFG->removeNode(privCFG->bbNodeMap[bb]);
            }
        }
    }

    /* errs() << "=== finish removeUnprivBB() ===\n"; */

    /* Function *F = theModule->getFunction("main"); */
    /* F->dump(); */
    /* errs() << "after removeUnprivBB():\n"; */
    /* for (auto funcCFG : funcCFGMap) { */
    /*     if (!funcCFG.first->getName().equals("main")) continue; */
    /*     funcCFG.second->dump(); */
    /* } */

}

/*
 * computeSCCs() finds all SCCs of the CFG of a function
 * */
void CompressCFG::computeSCCs() {
    for (pair<Function *, PrivCFG *> funcCFG : funcCFGMap) {
        Function *f = funcCFG.first;
        PrivCFG *cfg = funcCFG.second;

        vector<PrivCFGNode *> order;
        unordered_set<PrivCFGNode *> visited;
        
        /* errs() << cfg->F.getName() << "\n"; */
        assert(cfg->entry && "entry is NULL!!!");
        forwardDFS(cfg->bbNodeMap[cfg->entry], visited, order);


        unordered_set<PrivCFGNode *> processed;
        while (order.empty() == false) {
            PrivCFGNode *node = order.back();
            order.pop_back();

            // skip processed node
            if (processed.find(node) != processed.end()) continue;

            visited.clear();
            backwardDFS(node, visited, processed);

            // if there is only one node and it has no link to itself
            if (visited.size() == 1 && !node->hasPredecessor(node)) continue;

            /* errs() << "find a SCC from " << f->getName() << "\n"; */
            // add these blocks into a new SCC and add this SCC to its corresponding CFG
            PrivCFGSCC *scc = new PrivCFGSCC(*f);
            cfg->sccs.insert(scc);
            for (PrivCFGNode *node : visited) {
                BasicBlock *bb = node->bb;
                scc->bbs.insert(bb);
                bbSCCMap.insert(pair<BasicBlock *, PrivCFGSCC *>(bb, scc));
            }
            /* errs() << "; it has " << scc->bbs.size() << " basic block\n"; */

            // collect privileges used in this SCC
            collectSCCCaps(*scc);
        }
    }
}
// helper function for computeSCCs()
// It is part of Kosaraju's allgorithm
void CompressCFG::forwardDFS(PrivCFGNode *node, unordered_set<PrivCFGNode *> &visited, vector<PrivCFGNode *> &order) {
    if (visited.find(node) != visited.end()) return;

    visited.insert(node);
    for (PrivCFGNode *successor : node->successors) forwardDFS(successor, visited, order);

    order.push_back(node);
}
void CompressCFG::backwardDFS(PrivCFGNode *node, unordered_set<PrivCFGNode *> &visited, 
                                unordered_set<PrivCFGNode *> &processed) {
    if (visited.find(node) != visited.end()) return;

    visited.insert(node);
    processed.insert(node);
    for (PrivCFGNode * predecessor : node->predecessors) {
        if (processed.find(predecessor) == processed.end()) backwardDFS(predecessor, visited, processed);
    }
}


/*
 * collectSCCCaps() collects all privileges used by basic blocks in this SCC and 
 * all privileges used in functions reachable from this SCC.
 *
 * @param scc is the target SCC.
 * */
void CompressCFG::collectSCCCaps(PrivCFGSCC &scc) {
    BBCAPTable_t &bbCapMap = *CCG->bbCapMap;
    Function *callee;  // possible callees in basic blocks

    for (BasicBlock *bb : scc.bbs) {
        if (bbCapMap.find(bb) != bbCapMap.end()) UnionCAPArrays(scc.caps, bbCapMap[bb]);
        else if ((callee = CCG->privCG->getCalleeFromBB(bb))) {
            assert(callee && "get null callee from getCalleeFromBB()");
            for (PrivCallGraphNode *cgNode : CCG->reachablePrivFunc[callee]) {
                UnionCAPArrays(scc.caps, (*CCG->funcCapMap)[cgNode->getFunction()]);
            }
        } else {
            assert(false && "Basic Blocks are not split as intended");
        }
    }
}

void CompressCFG::debuger() {

    // print SCCs 
    Function *f = theModule->getFunction("foo");
    PrivCFG *cfg = funcCFGMap[f];
    cfg->printSCCs();
}


// register this pass
char CompressCFG::ID = 0;
static RegisterPass<CompressCFG> A("compress_cfg",  // name used as an arugment for opt
                                    "Compress Control Flow Gragh pass",
                                    false,
                                    false);  // analysis pass
