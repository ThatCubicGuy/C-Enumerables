#include "Enumerable.h"
#include "LinkedList.h"
#include "List.h"

static void ShowValues(IEnumerable* source)
{
    IEnumerator* enumerator = source->GetEnumerator(source);
    while (enumerator->MoveNext(enumerator)) fprintf(stderr, "Success! Value: %d\n", *(int*)(enumerator->Current));
}

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
    int x = 4, y = 1, z = 5, t = 17;
    object items[] = { &t, &x, &y, &z };
    List* list = CreateList(7);
    printf("Item count: %d\nList capacity: %d\n", list->Count, list->Capacity);
    printf("Array items: ");
    for (int i = 0; i < 4; ++i)
    {
        printf("%d, ", *(int*)items[i]);
        List_Add(list, items[i]);
        fprintf(stderr, "Loop number: %d\n", i);
    }
    printf("\nItem count: %d\nList capacity: %d\n", list->Count, list->Capacity);
    List_TrimExcess(list);
    printf("Item count: %d\nList capacity: %d\n", list->Count, list->Capacity);
    List_EnsureCapacity(list, 15);
    printf("Item count: %d\nList capacity: %d\n", list->Count, list->Capacity);
    for (int i = 0; i < 12; ++i)
    {
        List_Add(list, items[1]);
    }
    printf("Item count: %d\nList capacity: %d\n", list->Count, list->Capacity);
    for (int i = 0; i < 12; ++i)
    {
        List_Remove(list, items[1]);
    }
    List_TrimExcess(list);
    printf("Item count: %d\nList capacity: %d\n", list->Count, list->Capacity);
    IEnumerable* enumerable = (IEnumerable*)list;
    ShowValues(enumerable);
    printf("Selector function that adds 3:\n");
    enumerable = Enumerable_Select(enumerable, selector);
    ShowValues(enumerable);
    printf("Selector function that deducts 4:\n");
    enumerable = Enumerable_Select(enumerable, selector2);
    ShowValues(enumerable);
    printf("select only even numbers:\n");
    enumerable = Enumerable_Where(enumerable, filter);
    ShowValues(enumerable);
}
