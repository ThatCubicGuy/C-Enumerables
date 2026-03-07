#include "Collections/Generic/EnumerableImplement.h"
#include "Collections/Generic/LinkedListImplement.h"
#include "Tests.h"

#ifndef INT_ENUMERABLE_DEFINED
#define INT_ENUMERABLE_DEFINED
ENUMERABLE_DEFINE(int)
#endif
LINKED_LIST_DEFINE(int)

void test_generic_linked_lists(void)
{
    LinkedList_int* int_list = new(LinkedList_int)();
    LinkedList_int_Add(int_list, 4);
    LinkedList_int_Add(int_list, 5);
    LinkedList_int_Add(int_list, 7);
    LinkedList_int_Add(int_list, 1);
    LinkedList_int_Add(int_list, 3);
    foreach(int, var, int_list, {
        printf("Item in list: %d\n", var);
    });
}

LINKED_LIST_IMPLEMENT(int)