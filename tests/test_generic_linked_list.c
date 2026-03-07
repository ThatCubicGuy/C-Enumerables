#include "Collections/Generic/EnumerableImplement.h"
#include "Collections/Generic/LinkedListImplement.h"
#include "Tests.h"

#ifndef INT_ENUMERABLE_DEFINED
#define INT_ENUMERABLE_DEFINED
ENUMERABLE_DEFINE(int)
ENUMERABLE_DEFINE_SELECT(int, int)
ENUMERABLE_DEFINE_SELECTMANY(int, int)
ENUMERABLE_DEFINE_AGGREGATE(int, int)
#endif
#ifndef INT_LINKED_LIST_DEFINED
#define INT_LINKED_LIST_DEFINED
LINKED_LIST_DEFINE(int)
#endif

int multBy3(int item)
{
    return item * 3;
}

void test_generic_linked_lists(void)
{
    LinkedList_int int_list = new(LinkedList_int)();
    LinkedList_int_Add(int_list, 4);
    LinkedList_int_Add(int_list, 5);
    LinkedList_int_Add(int_list, 7);
    LinkedList_int_Add(int_list, 1);
    LinkedList_int_Add(int_list, 3);
    foreach(int, var, int_list, {
        printf("Item in list: %d\n", var);
    });
    printf("Select first 4 as multiplied by 3:\n");
    foreach(int, var, Enumerable_int_Take(Enumerable_int_Select_int(base(int_list), multBy3), 4), {
        printf("Item in list: %d\n", var);
    });
    printf("Insert 19 at index 2 and prepend 0:\n");
    LinkedList_int_Insert(int_list, 19, 2);
    foreach(int, var, Enumerable_int_Prepend(base(int_list), 0), {
        printf("Item in list: %d\n", var);
    });
}