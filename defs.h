// Copyright (C) 2020-2021 Terence Kelly.  All rights reserved.
// Common #defines.

#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#define  P (void) printf
#define FP (void)fprintf

#define BAIL(...)             \
  do {                        \
    FP(stderr, __VA_ARGS__);  \
    exit(EXIT_FAILURE);       \
  } while (0)

#endif

