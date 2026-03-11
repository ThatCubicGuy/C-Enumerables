#include "Collections/Enumerable.h"
#include "Collections/Array.h"

int sort_ascending(object left, object right)
{
    if (*(int*)left < *(int*)right) return -1;
    if (*(int*)left > *(int*)right) return 1;
    return 0;
}

void test_ordering(void)
{
    int x[16] = {125, 1, 65, 34, 22, 1, 94, 90, -1, 65, 3, 18, 8, 88, 8889, 2};
    Array arr = new(Array)(sizeof(int), 20);
    Array_Fill(arr, &x);
    for (int i = 0; i < 16; ++i) {
        Array_Set(arr, i, x + i);
    }
    foreach_deref(int, item, arr, {
        printf("Array item: %d\n", item);
    });
    printf("NEXT!!!\n");
    foreach_deref(int, item, Enumerable_OrderBy((IEnumerable)arr, sort_ascending), {
        printf("Array item: %d\n", item);
    });

    // Array_Set(arr, 0, )
}