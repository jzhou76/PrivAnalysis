/* PrivGraph.h
 *
 *     The Automated Privileges Project
 *
 *
 * !TODO!: handle indirect function calls
 * */

#include "PrivGraph.h"

using namespace llvm;
using namespace privGraph;

//
// ===============================
// implementation of PrivCallGraph
// ===============================
//

// constructor
PrivCallGraph::PrivCallGraph(Module &M) : M(M) {
    // build the original call graph
    for (Module::iterator mi = M.begin(); mi != M.end(); mi++) {
        // skip over library functions and priv_raise/lower/lowerall
        if (isIgnorable(&*mi)) continue;

        /* errs() << "processing function " << mi->getName() << "\n"; */

        // check if this is the entry function
        if (mi->getName().equals(ENTRY_FUNC)) {
            entryFunc = &*mi;
            addFunctionNode(entryFunc);
        } 

        for (Function::iterator fi = mi->begin(); fi != mi->end(); fi++) {
            // iterate over every basic block
            for (BasicBlock::iterator bbi = fi->begin(); bbi != fi->end(); bbi++) {
                CallInst *ci = dyn_cast<CallInst>(&*bbi);
                if (ci != NULL) {

                    Function *callee = dyn_cast<Function>(ci->getCalledValue()->stripPointerCasts());


                    if (callee == NULL) {
                        // TODO: deal with indirect function calls
                        // this is an indirect call
                        /* ci->getCalledValue()->dump(); */
                        /* ci->getArgOperand(1)->dump(); */
                        /* LoadInst *li = dyn_cast<LoadInst>(ci->getCalledValue()); */
                        /* if (li != NULL) errs() << "YES\n"; */
                        for (auto ui = mi->user_begin(); ui != mi->user_end(); ui++) {
                        }
                        continue;
                    }

                    // skip over library functions, priv_raise, and priv_lower
                    if (isIgnorable(callee)) continue;

                    // add caller-callee relation into the call graph
                    addCallRelation(&*mi, callee);

                    // store bb - ci pair
                    bbInstMap.insert(pair<BasicBlock *, CallInst *>(&*fi, ci));
                    // add CallInst - Function map; TODO: to fix the function pointer problem
                    callinstFuncMap.insert(pair<CallInst *, Function *>(ci, callee));
                } 
            }
        }
    }

    // remove unrechable functions from main
    removeUnreachableFuncs(funcNodeMap[entryFunc]);

    // get which functions can reach a priv function
    /* funcReachPrivFunc(); */

    /* dump(); */
}

// ignore library function, priv_raise, and priv_lower
bool PrivCallGraph::isIgnorable(Function *F) const {
    if (F->isDeclaration() || F->getName().equals(PRIVRAISE)
                           || F->getName().equals(PRIVLOWER) 
                           || F->getName().equals(PRIVLOWERALL)) return true;
    else return false;
}

// return the Function a PrivCallGraphNode represents
// Do we really need this?
PrivCallGraphNode *PrivCallGraph::getNode(Function *F) {
    return funcNodeMap[F];
}


// add a caller-callee relation 
void PrivCallGraph::addCallRelation(Function *caller, Function *callee) {
    // in case the caller or callee has not been added to the call graph
    PrivCallGraphNode *callerNode = addFunctionNode(caller);
    PrivCallGraphNode *calleeNode = addFunctionNode(callee);

    if (callerNode->hasCallee(calleeNode)) {
        // this call-relation has already existed
        return;
    }
    
    // add caller and callee
    callerNode->callees.insert(calleeNode);
    calleeNode->callers.insert(callerNode);
}


// add a function to the call graph
PrivCallGraphNode *PrivCallGraph::addFunctionNode(Function *F) {
    if (funcNodeMap.find(F) == funcNodeMap.end()) {
        // got a new function
        /* errs() << "add " << F->getName() << " to the call graph\n"; */
        PrivCallGraphNode *node = new PrivCallGraphNode(*F);
        nodes.insert(node);
        funcNodeMap.insert(pair<Function *, PrivCallGraphNode *>(F, node));
        
        return node;
    }

    return funcNodeMap[F];
}


// remove a node from the call graph
void PrivCallGraph::removeNode(PrivCallGraphNode *node) {
    nodes.erase(node);   
    funcNodeMap.erase(&node->F);

    // delete it all its callees' caller list
    for (auto i = node->callees.begin(); i != node->callees.end(); i++) {
        (*i)->callers.erase(node);
    }

    // delete it from all its callers' callee list
    for (auto i = node->callers.begin(); i != node->callers.end(); i++) {
        (*i)->callees.erase(node);
    }

    // free up the node itself
    delete node;
}


/*
 * removeUnreachableFuncs() removes all functions that can not be reached from main() function
 *
 * Start from the entry function (main), and do a DFS.
 * */
void PrivCallGraph::removeUnreachableFuncs(PrivCallGraphNode *entry) {
    assert(entry && "lost main() function");
    unordered_set<PrivCallGraphNode *> reachable;
    unordered_set<PrivCallGraphNode *> unreachable;
    DFS(entry, reachable);

    // remove all nodes that don't appear in reachable
    for (auto ni = nodes.begin(); ni != nodes.end(); ni++) {
        PrivCallGraphNode *node = *ni;
        if (reachable.find(node) == reachable.end()) {
            // this node is not reachable from entry function; remove it
            /* errs() << "remove " << node->F.getName() << "\n"; */
            unreachable.insert(node);
        }
    }

    for (auto i = unreachable.begin(); i != unreachable.end(); i++) removeNode(*i);
}
void PrivCallGraph::DFS(PrivCallGraphNode *node, unordered_set<PrivCallGraphNode *> &reachable) {
    assert(node && "the node to be explored is NULL.");

    if (reachable.find(node) != reachable.end()) return;

    reachable.insert(node);
    for (auto i = node->callees.begin(); i != node->callees.end(); i++) { DFS(*i, reachable); }
}


// return a callee called from a basic block
Function *PrivCallGraph::getCalleeFromBB(BasicBlock *bb) {
    return callinstFuncMap[bbInstMap[bb]];
}


// check if a function exists in the call graph
bool PrivCallGraph::hasFunction(Function *F) const {
    if (funcNodeMap.find(F) != funcNodeMap.end()) return true;
    else return false;
}


// print all function names and their addresses
void PrivCallGraph::print() const {
    for (auto i = funcNodeMap.begin(); i != funcNodeMap.end(); i++) {
        errs() << i->first->getName() << ": " << i->first << "\n";
    }
    errs() << "\n";
}

//
// print out call relations
void PrivCallGraph::dump() const {
    errs() << "\nCalling relations of this program: \n";
    for (auto fni = nodes.begin(); fni != nodes.end(); fni++) {
        (*fni)->dump();
    }
}

void PrivCallGraph::printSCCs() const {
    errs() << "\n===== start of printing Call Graph SCCs =====\n";
    unordered_set<PrivCGSCC *> printed;
    for (pair<Function *, PrivCGSCC *> funcSCC : funcSCCMap) {
        PrivCGSCC *scc = funcSCC.second;
        if (printed.find(scc) != printed.end()) continue;

        printed.insert(scc);
        errs() << "SCC: ";
        for (Function *f : scc->funcs) {
            errs() << f->getName() << " ";
        }
        errs() << "\n";

        /* errs() << "CAPArray = " << scc->caps << " This SCC uses "; */
        errs() << "This SCC uses ";
        for (uint64_t i = 0; i < CAP_TOTALNUM; i++) {
            uint64_t loc = ((uint64_t)1 << i);
            /* uint64_t loc = (1 << i); */  // WRONG!
            if (loc & scc->caps) errs() << CAPString[i] << " ";
        }
    }
    errs() << "\n===== end of printing Call Graph SCCs =====\n\n";
}

//
// ===================================
// implementation of PrivCallGraphNode
// ===================================
//

//constructor
PrivCallGraphNode::PrivCallGraphNode(Function &F) : F(F) {}

// check if this Function has some caller
bool PrivCallGraphNode::hasCaller(PrivCallGraphNode *caller) const {
    if (callers.find(caller) != callers.end()) return true;
    else return false;
}

// check if this Function has some callee
bool PrivCallGraphNode::hasCallee(PrivCallGraphNode *callee) const {
    if (callees.find(callee) != callees.end()) return true;
    else return false;
}

Function* PrivCallGraphNode::getFunction() const { return &F; }
StringRef PrivCallGraphNode::getFuncName() const { return F.getName(); }

/*
 * dump() prints out information of a PrivCallGraphNode.
 * It prints out all callees this function calls, and all callers that call this function.
 * */
void PrivCallGraphNode::dump() const {
    // print callees
    errs() << "Function " << F.getName() << " calls: ";
    for (auto i = callees.begin(); i != callees.end(); i++) {
        errs() << (*i)->getFunction()->getName() << " ";
    }
    if (callees.size() == 0) errs() << "nothing";
    errs() << "\n";

    // prints callers
    errs() << "Function " << F.getName() << " is called by: ";
    for (auto i = callers.begin(); i != callers.end(); i++) {
        errs() << (*i)->getFunction()->getName() << " ";
    }
    if (callers.size() == 0) errs() << "nothing";
    errs() << "\n";
    errs() << "\n";
}

// Is it a good idea to write code like this?
inline void PrivCallGraphNode::addCaller(PrivCallGraphNode *caller) {
    callers.insert(caller);
}
inline void PrivCallGraphNode::addCallee(PrivCallGraphNode *callee) {
    callees.insert(callee);
}

//
//===========================
//implementation of PrivCGSCC
//===========================
//

// constructor
PrivCGSCC::PrivCGSCC() {}




//
// =========================
// implementation of PrivCFG
// =========================
//

// constructor
// It maps a function's inheritance to a privCFG for this function
PrivCFG::PrivCFG(Function &F) : F(F) {
    for (Function::iterator fi = F.begin(); fi != F.end(); fi++) {
        BasicBlock *bb = &*fi;

        // mark entry and end block
        if (bb == &(F.getEntryBlock())) entry = bb;
        if (bb == &(F.back())) end = bb;

        // add a node for this basic block
        /* PrivCFGNode *node = addBBNode(bb); */
        addBBNode(bb);

        // add this node's predecessors and successors
        for (BasicBlock *pred : predecessors(bb)) addInheritance(pred, bb);
        for (BasicBlock *successor : successors(bb)) addInheritance(bb, successor);
    }
}

void PrivCFG::addInheritance(BasicBlock *from, BasicBlock *to) {
    PrivCFGNode *fromNode = addBBNode(from);
    PrivCFGNode *toNode = addBBNode(to);

    fromNode->successors.insert(toNode);
    toNode->predecessors.insert(fromNode);
}

// add a new node in CFG
PrivCFGNode *PrivCFG::addBBNode(BasicBlock *bb) {
    if (bbNodeMap.find(bb) == bbNodeMap.end()) {
        // add a new node
        PrivCFGNode *node = new PrivCFGNode(bb);
        bbNodeMap.insert(pair<BasicBlock *, PrivCFGNode *>(bb, node));

        return node;
    }

    return bbNodeMap[bb];
}

/*
 * removeNode() removes a node from its CFG and add link from all its predecessors to all its successors
 *
 * 1. remove corresponding item from bbNodeMap
 * 2. remove it from all its predecessors's successor list
 * 3. remove it from all its successors's predecessor list
 * 4. add new inheritance 
 * 5. free itself
 * */
void PrivCFG::removeNode(PrivCFGNode *node) {
    BasicBlock *bb = node->bb;
    // 1. remove item from bbNodeMap
    bbNodeMap.erase(bb);

    for (PrivCFGNode *predecessor : node->predecessors) {
        //2. remove it from all its predecessors's successor list
        // skip itself; otherwise this node couldn't be removed
        if (predecessor == node) continue; 
        predecessor->successors.erase(node);
        for (PrivCFGNode *successor : node->successors) {
            // 3. remove it from all its successors's predecessor list
            if (successor == node) continue;  // skip itself to avoid dogged loop
            successor->predecessors.erase(node);

            // 4. add new inheritance
            addInheritance(predecessor->bb, successor->bb);
        }
    }

    // 5. free the node
    delete node;
}

// check if a node has some predecessor or successor
bool PrivCFGNode::hasPredecessor(PrivCFGNode *node) const {
    return predecessors.find(node) != predecessors.end();
}
bool PrivCFGNode::hasSuccessor(PrivCFGNode *node) const {
    return successors.find(node) != successors.end();
}


// print SCCs of a CFG
void PrivCFG::printSCCs() const {
    errs() << "\n===== start of printing SCC(s) of the CFG of function " << F.getName() << " =====\n";

    for (PrivCFGSCC *scc : sccs){
        scc->dump();

        errs() << "This SCC uses ";
        for (uint64_t i = 0; i < CAP_TOTALNUM; i++) {
            uint64_t loc = (uint64_t)1 << i;
            if (loc & scc->caps) errs() << CAPString[i] << " ";
        }

    }

    errs() << "\n===== end of printing SCC(s) of the CFG of function " << F.getName() << " =====\n\n";
}

void PrivCFG::dump() const {
    for (auto bbNode : bbNodeMap) bbNode.second->dump();
}

//
// =============================
// implementation of PrivCFGNode
// =============================
//
// constructor
PrivCFGNode::PrivCFGNode(BasicBlock *bb) : bb(bb) { }

void PrivCFGNode::dump() const {
    /* errs() << "Basic Block " << bb.getName() << ":\n"; */
    errs() << "For Basic Block";
    bb->dump();
    errs() << "predecessors: ";
    for (PrivCFGNode *pred : predecessors) {
        pred->bb->dump();
        /* errs() << pred->getBBName() << " "; */
    }
    errs() << "\n";
    errs() << " successors: ";
    for (PrivCFGNode *successor : successors) {
        successor->bb->dump();
        /* errs() << successor->getBBName() << " "; */
    }
    errs() << "\n\n";
}

StringRef PrivCFGNode::getBBName() const {
    return bb->getName();
}


//
//PrivCFGSCC
//
PrivCFGSCC::PrivCFGSCC(Function &F) : F(F) { }


//
//
// ============================
// implementation of PrivCFGSCC
// ============================
//
void PrivCFGSCC::dump() const {
    for (BasicBlock *bb : bbs) {
        bb->dump();
    }
}
