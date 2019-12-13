#!/bin/bash
### benchmark runner script
### Locate this script at each benchmark directory. e.g, 583simple/run.sh
### usage: ./run.sh ${benchmark_name} ${input} 
### e.g., ./run.sh compress compress.in   or   ./run.sh simple

rm -f *prof* cccp.c test1*

PATH_MYPASS=${ALLOCA_PATH}                                                    ### Action Required: Specify the path to your pass ###
NAME_MYPASS=-AllocaPack                                                       ### Action Required: Specify the name for your pass ###
#BENCH=../src/test1.c
# BENCH=../src/test_sort.c


for file in ../src/*.c; do 
    if [ -f "$file" ]; then 
        # Convert source code to bitcode (IR)
        # This approach has an issue with -O2, so we are going to stick with default optimization level (-O0)
        clang -emit-llvm -c $file -o test1.bc

        # Apply your pass to bitcode (IR)
        opt -load ${PATH_MYPASS} ${NAME_MYPASS} < test1.bc > new_test.bc 2> logs.txt

        clang -O2 test1.bc -o test1
        clang -O2 new_test.bc -o new_test
        echo ""
        echo "Benchmarking $file"
        echo ""
        echo "Baseline"
        time ./test1 > out
        echo ""
        echo "Optimized"
        time ./new_test > out_other

        diff out out_other
    fi 
done
