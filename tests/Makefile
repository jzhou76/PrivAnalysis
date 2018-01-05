BUILD_DIR = ../../../../../build

all:
	clang -emit-llvm -c hello.c lib.c
	llvm-link hello.bc lib.bc -o test.bc
dis:
	llvm-dis test.bc

opt:
	@# run PrivAnalysis Pass
	@opt -load $(BUILD_DIR)/lib/PrivAnalysis.so -priv_analysis < test.bc > test.bc.opt
local:
	@# run LocalAnalysis Pass
	@opt -load $(BUILD_DIR)/lib/PrivAnalysis.so -LocalAnalysis < test.bc > test.bc.opt

clean:
	@rm *.bc *.ll *.o 2> /dev/null
