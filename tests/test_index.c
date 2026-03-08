#include "Collections/Generic/EnumerableT.h"
#include "Collections/Generic/ListT.h"
#include "Tuple.h"

// temporary "fix" cuz late
typedef struct int_int_tuple {
    int Item1;
    int Item2;
} int_int;
ENUMERABLE_DEFINE(int)
ENUMERABLE_DEFINE(int_int)
ENUMERABLE_DEFINE_INDEX(int)

LIST_DEFINE(int)

void test_index(void)
{
    List_int list = new(List_int)(16);
    List_int_Add(list, 27);
    List_int_Add(list, 15);
    List_int_Add(list, 2);
    List_int_Add(list, 7);
    foreach (int, nr, list, {
        printf("Number is: %d\n", nr);
    });
    foreach (int_int, tuple, Enumerable_int_Index(base(list)), {
        printf("Number at index %d is: %d\n", tuple.Item2, tuple.Item1);
    });
}