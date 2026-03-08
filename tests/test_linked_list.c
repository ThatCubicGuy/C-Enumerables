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
    LinkedList list = LinkedList__ctor();
    printf("List address: %p\n", list);
    LinkedList_Add(list, &c);
    LinkedList_Add(list, &b);
    LinkedList_Add(list, &a);
    foreach_ref(object, item, base(list), showItemNumber(item));
    LinkedList_Clear(list);
    LinkedList_Add(list, &c);
    LinkedList_Add(list, &c);
    LinkedList_Add(list, &c);
    LinkedList_Add(list, &c);
    LinkedList_Insert(list, &a, 2);
    LinkedList_Insert(list, &a, 2);
    printf("It's-a %d!\n", ((ListItem*)list->_end->Value)->Number);
    foreach_ref(object, item, base(list), showItemNumber(item));
    printf("Count: %d\n", list->Count);
    LinkedList_Clear(list);
    printf("Count: %d\n", list->Count);
    LinkedList_Destroy(&list);
}