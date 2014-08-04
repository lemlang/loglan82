#ifndef DEBUG_H
#include <stdio.h>

#define DEBUG_H
#ifndef NDEBUG
#define DEBUG_PRINT(fmt, args...)    fprintf(stderr, fmt, ## args)
#else
#define DEBUG_PRINT(fmt, args...)    /* Don't do anything in release builds */
#endif

#endif
