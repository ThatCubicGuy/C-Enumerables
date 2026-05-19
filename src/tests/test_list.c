#include "Collections/Generic/EnumerableT.h"
#include "Collections/Generic/ListT.h"
#include "Tests.h"
#include <stdio.h>

ENUMERABLE_DEFINE(int)
LIST_DEFINE(int)
ENUMERABLE_DEFINE_SELECT(int, int)
ENUMERABLE_DEFINE_AGGREGATE(int, int)
#undef var
static void ShowValues(IEnumerable(int) source)
{
    foreach (int var in source) {
        fprintf(stderr, "Success! Value: %d\n", var);
    }
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

IEnumerable(int) selectMany(int item)
{
    List(int) L = new(List(int))(3);
    List_int_Add(L, item);
    List_int_Add(L, item);
    List_int_Add(L, item);
    return (IEnumerable(int))L;
}

int aggregate(int accumulate, int item)
{
    return accumulate + item;
}

void test_with_numbers(FILE* output)
{
    int items[] = { 17, 4, 1, 5 };
    List(int) int_list = new(List(int))(7);
    fprintf(output, "Item count: %d\nList capacity: %d\n", int_list->Count, int_list->Capacity);
    fprintf(output, "Array items: ");
    for (int i = 0; i < 4; ++i)
    {
        fprintf(output, "%d, ", items[i]);
        List_int_Add(int_list, items[i]);
    }
    fprintf(output, "\nItem count: %d\nList capacity: %d\n", int_list->Count, int_list->Capacity);
    List_int_TrimExcess(int_list);
    fprintf(output, "Item count: %d\nList capacity: %d\n", int_list->Count, int_list->Capacity);
    IEnumerable(int) enumerable = (IEnumerable(int))int_list;
    ShowValues(enumerable);
    fprintf(output, "Selector function that adds 3:\n");
    enumerable = Enumerable_int_Select_int(enumerable, selector);
    ShowValues(enumerable);
    fprintf(output, "Selector function that divides by 2:\n");
    enumerable = Enumerable_int_Select_int(enumerable, selector2);
    ShowValues(enumerable);
    fprintf(output, "Where numbers are even:\n");
    enumerable = Enumerable_int_Where(enumerable, filter);
    ShowValues(enumerable);
    fprintf(output, "Project each value into an array of three times the thing:\n");
    enumerable = Enumerable_int_SelectMany_int(enumerable, selectMany);
    ShowValues(enumerable);
    fprintf(output, "Take only the first five items:\n");
    enumerable = Enumerable_int_Take(enumerable, 5);
    ShowValues(enumerable);
    fprintf(output, "Skip the first two items:\n");
    enumerable = Enumerable_int_Skip(enumerable, 2);
    ShowValues(enumerable);
    fprintf(output, "Original list enumeration:\n");
    enumerable = (IEnumerable(int))int_list;
    ShowValues(enumerable);
    fprintf(output, "First even number in the list: %d\n", Enumerable_int_FirstOrDefault(enumerable, filter));
    fprintf(output, "Index of first even number in the list: %d\n", Enumerable_int_IndexOf(enumerable, Enumerable_int_FirstOrDefault(enumerable, filter)));
    fprintf(output, "Sum of all values: %d\n", Enumerable_int_Aggregate_int(enumerable, 0, aggregate));
    List_int_Destroy(&int_list);
}
