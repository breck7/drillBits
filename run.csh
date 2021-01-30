#!/bin/csh -f

# Copyright (C) 2020-2021 Terence Kelly.  All rights reserved.
# Compile and run bid generator and auction-clearing program.

alias clean 'rm -f rb auclear'

clean

echo ''
echo 'Recommendations:'
echo '1. add "-fsanitize=undefined" to cflags if your compiler supports it'
echo '2. run both programs under Valgrind if you have it'
echo ''

# set cflags = '-std=c11 -Wall -Wextra -pedantic -g -fsanitize=undefined'
  set cflags = '-std=c11 -Wall -Wextra -pedantic -g'

gcc $cflags -o rb      rb.c
gcc $cflags -o auclear auclear.c

echo ''

# The following loop creates random inputs whose bids are small
# enough that the auction-clearing program checks its results
# against a brute-force algorithm.

set n = 1
set seed = 13
while ($n <= 1000)
    echo ''
    echo n $n seed $seed
    # valgrind -q ./rb 7 7 2 100 1000 $seed | valgrind -q ./auclear 100 100
                  ./rb 7 7 2 100 1000 $seed |             ./auclear 100 100
    echo ''
    @ seed += 13
    @ n ++
end

clean

