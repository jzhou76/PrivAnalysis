/* PrivAnalysis.cpp 
 *
 *     The Automated Privileges Project
 *
 * This file contains the implementation of the framework of the auto-priv project.
 * It works as a manager of the procedures taken to analyze the use of privileges of a program.
 *
 **/

#include "PrivAnalysis.h"
#include "CompressCallGraph.h"
#include "ADT.h"
#include "LocalAnalysis.h"
#include <sys/capability.h>
#include <linux/capability.h>

using namespace llvm;
using namespace privADT;
using namespace localAnalysis;
using namespace compressCG;
using namespace privAnalysis;

void getFamiliar(Module &M);
void getFamiliar(Module &M) {
    static LLVMContext context;
    StringRef sr("some_fun0");
    /* BasicBlock *b = BasicBlock::Create(context, "some_block", NULL, NULL); */
    Function *f = M.getFunction(sr);
    for (Value::user_iterator ui = f->user_begin(); ui != f->user_end(); ui++) {
        errs() << ui->getName() << " calls some_fun0\n";
    }
    
    errs() << "test ADT\n";
    /* CAPArray_t ca; */
    /* dumpCAPArray(ca); */

}



// PrivAnalysis constructor
PrivAnalysis::PrivAnalysis() : ModulePass(ID) {}

// entrance of this Pass
bool PrivAnalysis::runOnModule(Module &M) {
    errs() << "Hello from PrivAnalysis pass!\n";

    CompressCG &ccg = getAnalysis<CompressCG>();
    /* errs() << "ccfNum = " << ccg.ccgNum << "\n"; */
    
    return false;
}

/*
 * specify which passes should be run before PrivAnalysis
 * */
void PrivAnalysis::getAnalysisUsage(AnalysisUsage &AU) const {
    // run CompressCG pass first
    AU.addRequired<CompressCG>();  

    // still don't fully get why we need this...
    AU.setPreservesAll();
}

char PrivAnalysis::ID = 0;
static RegisterPass<PrivAnalysis> A("priv_analysis", "Privilege Analysis pass", false, true);
