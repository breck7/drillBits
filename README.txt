Copyright (C) 2020-2021 Terence Kelly.  All rights reserved.

Author contact:  tpkelly@acm.org, tpkelly@eecs.umich.edu,
tpkelly@cs.princeton.edu

Addenda and/or errata to the source code supplied herewith will be
posted in association with my "Drill Bits" column on clearing
auctions in ACM _Queue_ magazine circa late 2020 / early 2021.

If the column is re-published in _Communications of the ACM_, that
might become the definitive home of addenda, errata, etc.  Check
both.

Another (possibly less permanent) place to find follow-on material
is my Web site:

    http://web.eecs.umich.edu/~tpkelly/Drill_Bits/

The source code has been compiled with GCC and tested on Linux.  It
is intended to be portable to other compilers and OSes without
excessive fuss.  Re-format to taste with the "indent" utility.

I recommend compiling with GCC's "-fsanitize=undefined" flag if your
version of GCC supports it.  That should catch out-of-bounds array
subscripts and signed integer overflow errors at run time.

Program "rb" generates random bids.  Program "auclear" clears
two-resource auctions.  Script "run.csh" compiles and runs both
programs.

