#include "Collections/Enumerable.h"
#include "Collections/LinkedList.h"
#include "Tests.h"

typedef struct list_item_s {
    int Number;
    const char Name[16];
} ListItem;

void showItemNumber(object item)
{
    printf("%s: %d\n", ((ListItem*)item)->Name, ((ListItem*)item)->Number);
}

void test_linked_lists(void)
{
    ListItem a = (ListItem) {
        .Number = 4,
        .Name = "im not sure"
    }, b = (ListItem) {
        .Number = 1,
        .Name = "does this work"
    }, c = (ListItem) {
        .Number = 7,
        .Name = "watahelly"
    };
    LinkedList* list = LinkedList__ctor();
    LinkedList_Add(list, &c);
    LinkedList_Add(list, &b);
    LinkedList_Add(list, &a);
    Enumerable_ForEach(base(list), showItemNumber);
    LinkedList_Clear(list);
    LinkedList_Add(list, &c);
    LinkedList_Add(list, &c);
    LinkedList_Add(list, &c);
    LinkedList_Add(list, &c);
    LinkedList_Insert(list, &a, 2);
    LinkedList_Insert(list, &a, 2);
    printf("It's-a %d!\n", ((ListItem*)list->_end->Value)->Number);
    Enumerable_ForEach(base(list), showItemNumber);
    printf("Count: %d\n", list->Count);
}