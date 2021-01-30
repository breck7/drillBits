// Copyright (C) 2020-2021 Terence Kelly.  All rights reserved.
// Random bid generator for "Drill Bits" column in ACM _Queue_
// magazine circa late 2020 / early 2021.  Compile as C99 or C11.

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

#define A(i) atoi(argv[i])

int main(int argc, char *argv[]) {
  int t, T,  // number of bidders
      b, B,  // length of bids (number of bundles)
      r, R,  // number of resource types
         N,  // number of units of each resource
         X;  // maximum offer price
  if (7 != argc)
    BAIL("usage:  %s  T  B  R  N  X  seed\n", argv[0]);
  T = A(1); B = A(2); R = A(3); N = A(4); X = A(5);
  srand((unsigned)A(6));
  if (! (0 < T && 0 < B && 0 < R && 0 < N && 0 < X))
    BAIL("bad arg: %d %d %d %d %d\n", T, B, R, N, X);
  for (t = 1; t <= T; t++) {
    for (b = 1; b < B; b++) {
      P("%d", t);                  // bidder ID in [1..T]
      for (r = 0; r < R; r++)
        P(" %d", 1 + rand() % N);  // # units of resource type r
      P(" %d\n", 1 + rand() % X);  // offer price for bundle
    }
    P("%d", t);              // Every bid contains a "zero bundle"
    for (r = 0; r < R; r++)  //   with offer price zero; choosing
      P(" 0");               //   this bundle means bidder pays
    P(" 0\n");               //   nothing and gets nothing.
  }
  return 0;
}

