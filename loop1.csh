#!/bin/csh

foreach t ( 1 2 4 8 12 16 20 24 28 32 )
  foreach n ( 50 100 250 500 750 1000 1500 2000 2500 5000 )
        g++ -O3   Project2.cpp  -DNUMT=$t -DNUMNODES=$n  -o Project2  -lm  -fopenmp
    ./Project2
  end
end