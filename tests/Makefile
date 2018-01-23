BUILD_DIR = ../../../../../build

all:
	clang -emit-llvm -c hello.c lib.c priv.c
	llvm-link hello.bc lib.bc priv.bc -o test.bc

two:
	clang -emit-llvm -c test2.c
	llvm-link test2.bc lib.bc -o test2.bc
opt2:
	@opt -load $(BUILD_DIR)/lib/PrivAnalysis.so -priv_analysis < test2.bc > test2.bc.opt


dis:
	llvm-dis test.bc

.PHONY: opt
opt:
	@# run PrivAnalysis Pass
	@opt -load $(BUILD_DIR)/lib/PrivAnalysis.so -priv_analysis < test.bc > test.bc.opt
.PHONY: local
local:
	@# run LocalAnalysis Pass
	@opt -load $(BUILD_DIR)/lib/PrivAnalysis.so -LocalAnalysis < test.bc > test.bc.opt

split:
	@opt -load $(BUILD_DIR)/lib/PrivAnalysis.so -SplitBB < test.bc > test.bc.opt

clean:
	@rm *.bc *.ll *.o 2> /dev/null
