#!/bin/bash
### benchmark runner script
### Locate this script at each benchmark directory. e.g, 583simple/run.sh
### usage: ./run.sh ${benchmark_name} ${input} 
### e.g., ./run.sh compress compress.in   or   ./run.sh simple

rm -f *prof* cccp.c test1*

PATH_MYPASS=${ALLOCA_PATH}                                                    ### Action Required: Specify the path to your pass ###
NAME_MYPASS=-AllocaPack                                                       ### Action Required: Specify the name for your pass ###
BENCH=../src/test1.c


# Convert source code to bitcode (IR)
# This approach has an issue with -O2, so we are going to stick with default optimization level (-O0)
clang -emit-llvm -c ${BENCH} -o test1.bc

# Apply your pass to bitcode (IR)
opt -load ${PATH_MYPASS} ${NAME_MYPASS} < test1.bc > new_test.bc

opt -load ${PATH_MYPASS} ${NAME_MYPASS} -dce < test1.bc > new_test_with_dce.bc

clang test1.bc -o test1
clang new_test.bc -o new_test
clang new_test_with_dce.bc -o new_test_with_dce
echo ""
echo "Baseline"
time ./test1 > out
echo ""
echo "Optimized"
time ./new_test > out_other
echo ""
echo "With DCE"
time ./new_test_with_dce > out_dce

diff out out_other
diff out out_dce