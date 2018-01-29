add_llvm_loadable_module(PrivAnalysis
    SplitBasicBlock.cpp
    PrivGraph.cpp
    CompressCallGraph.cpp
    CompressCFG.cpp
    UnfoldCallee.cpp
    PrivAnalysis.cpp

    ADT.cpp
    LocalAnalysis.cpp
    # SplitBB.cpp
)
