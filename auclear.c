// Copyright (C) 2020-2021 Terence Kelly.  All rights reserved.
// Auction clearing program for "Drill Bits" column in ACM _Queue_
// magazine circa late 2020 / early 2021.  Compile as C99 or C11.

// Notes:  Number of resource types is fixed at 2.  Variable names
// etc. are intended to roughly correspond to my LNAI 3435 paper;
// equation numbers in comments also refer to that paper.

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

#define CI const int

enum {
  Tmax = 100,  // bidder IDs range from [1..T] where T <= Tmax
  Bmax = 100,  // max number of resource bundles allowed in a bid
  Nmax = 100   // max quantity of a resource
};

static int T,          // number of bidders
           B[1+Tmax],  // number of bundles in given bidder's bid
           BTmax;      // max number of bundles in any input bid

static struct bundle {
  int q1, q2, op;   // quantities of resources, offer price
} Q[1+Tmax][Bmax];  // indexed by bidderID, bundle number

#define CHK(e) \
do { if (! (e)) BAIL("input line %d: '" #e "'\n", line); } while (0)

static void addbundle(CI t, CI q1, CI q2, CI op, CI line) {
  int b;
  CHK(0 < t && Tmax >= t);
  CHK(Nmax >= q1 && Nmax >= q2);
  CHK(0 <= q1 && 0 <= q2 && 0 <= op);
  CHK(   ((0 < q1 || 0 < q2) && 0 < op)
      || (0 == q1 && 0 == q2 && 0 == op));
  b = B[t]++;
  if (B[t] > BTmax)
    BTmax = B[t];
  CHK(0 <= b && Bmax > b);
  assert(0 == Q[t][b].q1 && 0 == Q[t][b].q2 && 0 == Q[t][b].op);
  Q[t][b].q1 = q1;
  Q[t][b].q2 = q2;
  Q[t][b].op = op;
}

static struct bundle getbundle(CI t, CI b) {
  assert(0 < t && Tmax >= t && 0 <= b && Bmax > b);
  return Q[t][b];
}

static void dumpbids(void) {
  int t, b;
  for (t = 1; t <= T; t++)
    for (b = 0; b < B[t]; b++) {
      struct bundle u = getbundle(t, b);
      P("%d %d %d %d\n", t, u.q1, u.q2, u.op);
    }
}

static int D(CI n1, CI n2) {  // cost / value of unallocated goods
  assert(0 <= n1 && Nmax >= n1 && 0 <= n2 && Nmax >= n2);
  return 0;  // free disposal:  no penalty for unallocated goods
}

static int A[1+Tmax][1+Nmax][1+Nmax];  // Eq. 5

#define Uninit   INT_MIN
#define Neg_Inf (INT_MIN + 1)

static int Fcall, Cent, Chit,          // performance counters
           C[1+Tmax][1+Nmax][1+Nmax];  // cache for F() values

#define Ctotent (sizeof C / sizeof C[0][0][0])

static int F(CI t, CI n1, CI n2) {  // Eq. 4
  int b, max = Uninit, amax = -1;
  ++Fcall;
  if (0 > n1 || 0 > n2)
    return Neg_Inf;  // infeasible
  if (0 == t)
    return D(n1, n2);
  if (Uninit != C[t][n1][n2]) { // already computed
    ++Chit;
    return C[t][n1][n2];
  }
  for (b = 0; b < B[t]; b++) {
    struct bundle u = Q[t][b];
    int f = F(t - 1, n1 - u.q1, n2 - u.q2);
    if (Neg_Inf != f)
      f += u.op;
    if (f > max) {
      max = f;
      amax = b;
    }
  }
  assert(0 <= amax);
  assert(0 == A[t][n1][n2]);
  A[t][n1][n2] = amax;
  C[t][n1][n2] = max;
  ++Cent;
  return max;
}

// brute force: check all ways to choose one bundle per bid
static int brute(CI t, CI n1, CI n2) {
  int b, f, max = 0;
  if (0 > n1 || 0 > n2)
    return Neg_Inf;
  if (0 == t)
    return 0;
  for (b = 0; b < B[t]; b++) {
    struct bundle u = getbundle(t, b);
    f = brute(t - 1, n1 - u.q1, n2 - u.q2);
    if (Neg_Inf != f) {
      assert(0 <= f);
      f += u.op;
      if (f > max)
        max = f;
    }
  }
  return max;
}

int main(int argc, char *argv[]) {
  int N1, N2,  // quantities of resources available
      line = 0, t, q1, q2, op, v, ch1 = 0, ch2 = 0, chp = 0;
  if (3 != argc)
    BAIL("usage:  cat bidfile | %s N1 N2\n", argv[0]);
  N1 = atoi(argv[1]);
  N2 = atoi(argv[2]);
  if (0 > N1 || Nmax < N1 || 0 > N2 || Nmax < N2)
    BAIL("bad args: %d %d\n", N1, N2);
  while (4 == scanf("%d %d %d %d\n", &t, &q1, &q2, &op)) {
    addbundle(t, q1, q2, op, ++line);
    if (t > T) T = t;
  }
  if (0 == N1 && 0 == N2) {
    dumpbids();  // stdout should be identical to stdin
    return 0;
  }
  for (t = 0; t <= Tmax; t++)
    for (q1 = 0; q1 <= Nmax; q1++)
      for (q2 = 0; q2 <= Nmax; q2++)
        C[t][q1][q2] = Uninit;
  v = F(T, N1, N2);
  P("F() calls: %d  C[] entries: %d %f%%  C[] hits: %d  %f%%\n",
    Fcall,
    Cent, (double)100.0f * ((double)Cent/(double)Ctotent),
    Chit, (double)100.0f * ((double)Chit/(double)Fcall));
  if (0 > v) {
    assert(Neg_Inf == v);
    P("no feasible solution\n");
    return 0;  // maybe return error?
  }
  P("value of optimal solution: %d\n", v);
  q1 = N1;
  q2 = N2;
  for (t = T; 0 < t; t--) {
    int b = A[t][q1][q2];
    struct bundle u = getbundle(t, b);
    if (0 < u.op)
      P("bidder %d bid %d q1 %d q2 %d price %d\n", t, b, u.q1, u.q2, u.op);
    ch1 += u.q1;  q1 -= u.q1;
    ch2 += u.q2;  q2 -= u.q2;
    chp += u.op;
  }
  P("check:  resource1 %d resource2 %d value %d\n", ch1, ch2, chp);
  assert(chp == v && ch1 <= N1 && ch2 <= N2);
  if (8 > T && 8 > BTmax) {
    int v2 = brute(T, N1, N2);
    P("%s: brute force yields %d\n", v == v2 ? "confirmed" : "ERROR", v2);
    assert(v == v2);
  }
  return 0;
}

