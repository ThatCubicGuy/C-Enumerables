#include "Collections/Generic/EnumerableT.h"
#include "Collections/Generic/ListT.h"
#include "Tuple.h"
#include "Tests.h"
#include <stdio.h>

TUPLE_2_DEFINE(int, double)
ENUMERABLE_DEFINE(double)
ENUMERABLE_DEFINE(int_double)
ENUMERABLE_DEFINE_INDEX(double)
LIST_DEFINE(double)

void test_index(void)
{
    List(double) list = new(List(double))(16);
    List_double_Add(list, 27.5);
    List_double_Add(list, 0.15);
    List_double_Add(list, 2.0/19);
    List_double_Add(list, 700);
    foreach (double nr in list) {
        printf("Number is: %lf\n", nr);
    }
    foreach (t(int,double) tuple in Enumerable_double_Index((IEnumerable(double))(list))) {
        printf("Number at index %d is: %lf\n", tuple.Item1, tuple.Item2);
    }
}
