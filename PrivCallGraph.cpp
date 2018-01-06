/* CallGraph.h
 *
 *     The Automated Privileges Project
 *
 * This file implements PrivCallGraph class and PrivCallGraphNode class.
 *
 * PrivCallGraph only contains nodes corresponding to user-defined functions while ignores library functions.
 *
 * */

#include "PrivCallGraph.h"

using namespace llvm;
using namespace privCallGraph;

// constructor
PrivCallGraph::PrivCallGraph(Module &M) : M(M) {
    // build the original call graph
    for (Module::iterator mi = M.begin(); mi != M.end(); mi++) {
        // skip over library functions
        if (mi->isDeclaration()) continue;
        addNode(&*mi);
        
    }
}

void PrivCallGraph::addNode(Function *F) {
    if (functions.find(F) == functions.end()) {
        // meet a new function; 
        functions.insert(F);
        PrivCallGraphNode node(*F);
        nodes.insert(&node);
    }
}

// check if a function exists in the call graph
bool PrivCallGraph::hasFunction(Function *F) const {
    if (functions.find(F) != functions.end()) return true;
    else return false;
}


// ===================================
// implementation of PrivCallGraphNode
// ===================================
//

//constructor
PrivCallGraphNode::PrivCallGraphNode(Function &F) : F(F) {}

Function* PrivCallGraphNode::getFunction() const {
    return &F;
}
