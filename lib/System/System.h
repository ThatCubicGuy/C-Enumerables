#include "Keywords.h"
#include "String.h"
#include <stdlib.h>

// Enable debug info
#define DEBUG 1
#define DEBUG_WRITELINE(FORMAT, ...) if(DEBUG) fprintf(stderr, FORMAT"\n",##__VA_ARGS__)

#define EXIT_UNHANDLED_EXCEPTION 134
#define EXIT_ASSERT_FAILED 34

#define ASSERT(CONDITION) do { if(!(CONDITION)) {fprintf(stderr, "Assert failed: "#CONDITION"\n"); exit(EXIT_ASSERT_FAILED); } } while (0)
