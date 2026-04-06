#include "Collections/Generic/EnumerableT.h"
#include "Collections/Generic/DoublyLinkedListT.h"
#include "Tests.h"
#include "Defines.h"
#ifndef INT_ENUMERABLE_DEFINED
#define INT_ENUMERABLE_DEFINED
ENUMERABLE_DEFINE(int)
ENUMERABLE_DEFINE_SELECT(int, int)
ENUMERABLE_DEFINE_SELECTMANY(int, int)
ENUMERABLE_DEFINE_AGGREGATE(int, int)
#endif
#ifndef INT_DOUBLY_LINKED_LIST_DEFINED
#define INT_DOUBLY_LINKED_LIST_DEFINED
DOUBLY_LINKED_LIST_DEFINE(int)
#endif

void test_doubly_linked_list(void)
{
    DoublyLinkedList_int list = new(DoublyLinkedList_int)();
    DoublyLinkedList_int_Add(list, 15);
    DoublyLinkedList_int_Add(list, 5);
    DoublyLinkedList_int_Add(list, 1);
    DoublyLinkedList_int_Add(list, 9);
    printf("Item count: %d\n", list->Count);
    foreach (int, item, list, {
        printf("Item: %d\n", item);
    });
}