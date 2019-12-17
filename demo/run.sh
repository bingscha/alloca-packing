#!/bin/bash
### benchmark runner script
### Locate this script at each benchmark directory. e.g, 583simple/run.sh
### usage: ./run.sh ${benchmark_name} ${input} 
### e.g., ./run.sh compress compress.in   or   ./run.sh simple

rm -f *prof* cccp.c test1* new_test*

PATH_MYPASS=${ALLOCA_PATH}                                                    ### Action Required: Specify the path to your pass ###
NAME_MYPASS=-AllocaPack                                                       ### Action Required: Specify the name for your pass ###
#BENCH=../src/test1.c
# BENCH=../src/test_sort.c

# Convert source code to bitcode (IR)
# This approach has an issue with -O2, so we are going to stick with default optimization level (-O0)
echo "Converting to bitcode"
clang -emit-llvm -c test_summation.c -o test_summation.bc

echo "Running Pass"
# Apply your pass to bitcode (IR)
opt -load ${PATH_MYPASS} ${NAME_MYPASS} < test_summation.bc > test_summation_opt.bc

clang -O2 test_summation.bc -o test_summation_base
clang -O2 test_summation_opt.bc -o test_summation_opt
llvm-dis test_summation_opt.bc

echo ""
echo "Baseline"
time ./test_summation_base > out
echo ""
echo "Optimized"
time ./test_summation_opt > out_other
echo ""

diff out out_other

rm -f out out_other test_summation_opt.bc test_summation_opt test_summation.bc test_summation_base