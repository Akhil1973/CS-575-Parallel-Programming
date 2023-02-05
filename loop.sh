#!/bin/csh

echo "ArraySize,nonsimd_megamults,simd_megamults,nonsimd_speedup,nonsimd_addmults,simd_addmults,simd_speedup\n";
  foreach n ( 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608 )			
        g++ -DARRAYSIZE=$n Project4.cpp -o Project4  -lm  -fopenmp
    ./Project4
  end
