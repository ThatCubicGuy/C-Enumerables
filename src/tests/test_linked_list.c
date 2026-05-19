#include "Collections/Enumerable.h"
#include "Collections/LinkedList.h"
#include "Tests.h"

typedef struct list_item_s {
    int Number;
    const char Name[16];
} ListItem;

void showItemNumber(FILE* output, object item)
{
    fprintf(output, "%s: %d\n", ((ListItem*)item)->Name, ((ListItem*)item)->Number);
}

void test_linked_lists(FILE* output)
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
    LinkedList list = new(LinkedList)();
    fprintf(output, "List address: %p\n", list);
    LinkedList_Add(list, &c);
    LinkedList_Add(list, &b);
    LinkedList_Add(list, &b);
    LinkedList_Add(list, &b);
    LinkedList_Add(list, &a);
    foreach_as(object, item, (IEnumerable)(list), showItemNumber(output, item));
    LinkedList_Clear(list);
    LinkedList_Add(list, &c);
    LinkedList_Add(list, &c);
    LinkedList_Add(list, &c);
    LinkedList_Add(list, &c);
    LinkedList_Insert(list, &a, 2);
    LinkedList_Insert(list, &a, 2);
    fprintf(output, "It's-a %d!\n", ((ListItem*)list->_end->Value)->Number);
    foreach_as(object, item, (IEnumerable)(list), showItemNumber(output, item));
    fprintf(output, "Count: %d\n", list->Count);
    LinkedList_Clear(list);
    fprintf(output, "Count: %d\n", list->Count);
    LinkedList_Add(list, &a);
    LinkedList_Add(list, &b);
    LinkedList_Add(list, &c);
    foreach_as(object, item, (IEnumerable)(list), showItemNumber(output, item));
    fprintf(output, "Reverse list:\n");
    LinkedList_Reverse(list);
    foreach_as(object, item, (IEnumerable)(list), showItemNumber(output, item));
    LinkedList_Destroy(&list);
}
