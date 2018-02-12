# We need compile programs with -O2, which will run a series of optimization passes.
# We also need run the "-internalize" pass of LLVM before our own passes. 
# And one more -O2 after internalize


BUILD_DIR = ../../../../../build
CC = clang -emit-llvm -c -O2
LINK = llvm-link
OPT_LIB = $(BUILD_DIR)/lib/PrivAnalysis.so 
INTERNALIZE = -internalize

HELLO_SRC = hello.c lib.c priv.c
HELLO_BC = hello.bc lib.bc priv.bc


# hello:$(HELLO_SRC)
# 	$(CC) $^
# 	$(LINK) $(HELLO_BC) -o test.bc

hello:$(HELLO_BC)
	$(LINK) $^ -o hello.bc
	@llvm-dis hello.bc -o hello.ll

%.bc: %.c
	$(CC) $^

hello.bc:hello.c lib.c priv.c

.PHONY: opt
.ONESHELL:
opt: hello.bc
	@opt -load $(OPT_LIB) $(INTERNALIZE) -priv_analysis  < hello.bc > hello.bc.opt
	@llvm-dis hello.bc.opt -o hello.opt.ll


.PHONY: local split
ccg: hello.bc
	opt -load $(OPT_LIB) -compress_cg < $^ > hello.ccg.bc
	llvm-dis hello.ccg.bc -o hello.ccg.ll

local:
	@# run LocalAnalysis Pass
	opt -load $(OPT_LIB) -LocalAnalysis < hello.bc > hello.bc.opt
	llvm-dis hello.bc.opt -o hello.local.ll

split: hello.bc
	@opt -load $(OPT_LIB) $(INTERNALIZE) -SplitBB < hello.bc > hello.split.bc
	@llvm-dis hello.split.bc -o hello.split.ll

dis: test.bc
	llvm-dis $^ -o test.ll


two:
	clang -emit-llvm -c test2.c
	llvm-link test2.bc lib.bc -o test2.bc
opt2:
	@opt -load $(BUILD_DIR)/lib/PrivAnalysis.so -priv_analysis < test2.bc > test2.bc.opt



clean:
	@rm -f *.bc* *.ll 
