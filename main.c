#include "Enumerable.h"
#include "LinkedList.h"
#include "List.h"

object selector(object item)
{
    int* result = new(int);
    *result = (*(int*)item) + 3;
    return result;
}

object selector2(object item)
{
    int* result = new(int);
    *result = (*(int*)item) - 4;
    return result;
}

bool filter(object item)
{
    return (*(int*)item)%2 == 0;
}

int main(void)
{
    int x = 4, y = 1, z = 5;
    object items[] = { &x, &y, &z };
    printf("Array items: %d, %d, %d\n", *(int*)items[0], *(int*)items[1], *(int*)items[2]);
    List* list = CreateList(7);
    printf("Item count: %d\nList capacity: %d\n", list->Count, list->Capacity);
    List_Add(list, &x);
    List_Add(list, &y);
    List_Add(list, &z);
    printf("Item count: %d\nList capacity: %d\n", list->Count, list->Capacity);
    List_TrimExcess(list);
    printf("Item count: %d\nList capacity: %d\n", list->Count, list->Capacity);
    IEnumerable* enumerable = Enumerable_Select((IEnumerable*)list, selector);
    IEnumerator* enumerator = ((IEnumerable*)list)->GetEnumerator((IEnumerable*)list);
    while (enumerator->MoveNext(enumerator)) printf("Success! Value: %d\n", *(int*)(enumerator->Current));
    printf("Selector function that adds 3:\n");
    enumerator = enumerable->GetEnumerator(enumerable);
    while (enumerator->MoveNext(enumerator)) printf("Success! Value: %d\n", *(int*)(enumerator->Current));
    printf("Selector function that deducts 4:\n");
    enumerable = Enumerable_Select(enumerable, selector2);
    enumerator = enumerable->GetEnumerator(enumerable);
    while (enumerator->MoveNext(enumerator)) printf("Success! Value: %d\n", *(int*)(enumerator->Current));
    printf("select only even numbers:\n");
    enumerable = Enumerable_Where(enumerable, filter);
}
