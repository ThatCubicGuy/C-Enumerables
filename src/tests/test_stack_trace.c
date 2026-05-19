#include "EnumerableT.h"
#include "Keywords.h"
#include "Tests.h"
ENUMERABLE_DEFINE(int)
void test_stack_trace(FILE* output)
{
    try {
        Enumerable_int_Count(NULL);
    } catch (Exception ex) {
        fprintf(output, "Exception thrown: %s\n", ex->Message);
    }
}
