#include "Collections/Generic/EnumerableT.h"
#include "Collections/Generic/ListT.h"
#include "Tests.h"

ENUMERABLE_DEFINE(int)
LIST_DEFINE(int)
ENUMERABLE_DEFINE_SELECT(int, int)
ENUMERABLE_DEFINE_SELECTMANY(int, int)
ENUMERABLE_DEFINE_AGGREGATE(int, int)

#include "Collections/Generic/EnumerableImplement.h"
#include "Collections/Generic/ListImplement.h"

static void ShowValues(IEnumerable_int source)
{
    foreach(int, var, source,{
        fprintf(stderr, "Success! Value: %d\n", var);
    });
}

int selector(int item)
{
    return item + 3;
}

int selector2(int item)
{
    return item / 2;
}

bool filter(int item)
{
    return item % 2 == 0;
}

IEnumerable_int selectMany(int item)
{
    List_int L = new(List_int)(3);
    List_int_Add(L, item);
    List_int_Add(L, item);
    List_int_Add(L, item);
    return base(L);
}

int aggregate(int accumulate, int item)
{
    return accumulate + item;
}

void test_with_numbers(void)
{
    int items[] = { 17, 4, 1, 5 };
    List_int int_list = new(List_int)(7);
    printf("Item count: %d\nList capacity: %d\n", int_list->Count, int_list->Capacity);
    printf("Array items: ");
    for (int i = 0; i < 4; ++i)
    {
        printf("%d, ", items[i]);
        List_int_Add(int_list, items[i]);
    }
    printf("\nItem count: %d\nList capacity: %d\n", int_list->Count, int_list->Capacity);
    List_int_TrimExcess(int_list);
    printf("Item count: %d\nList capacity: %d\n", int_list->Count, int_list->Capacity);
    IEnumerable_int enumerable = base(int_list);
    ShowValues(enumerable);
    printf("Selector function that adds 3:\n");
    enumerable = Enumerable_int_Select_int(enumerable, selector);
    ShowValues(enumerable);
    printf("Selector function that divides by 2:\n");
    enumerable = Enumerable_int_Select_int(enumerable, selector2);
    ShowValues(enumerable);
    printf("Where numbers are even:\n");
    enumerable = Enumerable_int_Where(enumerable, filter);
    ShowValues(enumerable);
    printf("Project each value into an array of three times the thing:\n");
    enumerable = Enumerable_int_SelectMany_int(enumerable, selectMany);
    ShowValues(enumerable);
    printf("Take only the first five items:\n");
    enumerable = Enumerable_int_Take(enumerable, 5);
    ShowValues(enumerable);
    printf("Skip the first two items:\n");
    enumerable = Enumerable_int_Skip(enumerable, 2);
    ShowValues(enumerable);
    printf("Original list enumeration:\n");
    enumerable = base(int_list);
    ShowValues(enumerable);
    printf("First even number in the list: %d\n", Enumerable_int_FirstOrDefault(enumerable, filter));
    printf("Index of first even number in the list: %d\n", Enumerable_int_IndexOf(enumerable, Enumerable_int_FirstOrDefault(enumerable, filter)));
    printf("Sum of all values: %d\n", Enumerable_int_Aggregate_int(enumerable, 0, aggregate));
}
