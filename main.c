#include "Enumerable.h"
#include "LinkedList.h"
#include "List.h"

static void ShowValues(IEnumerable* source)
{
    IEnumerator* e = source->GetEnumerator(source);
    while (e->MoveNext(e)) fprintf(stderr, "Success! Value: %d\n", *(int*)(e->Current));
    e->Dispose(e);
}

object selector(const object item)
{
    int* result = new(int);
    *result = (*(const int*)item) + 3;
    return result;
}

object selector2(const object item)
{
    int* result = new(int);
    *result = (*(const int*)item) / 2;
    return result;
}

bool filter(const object item)
{
    return (*(const int*)item)%2 == 0;
}

IEnumerable* selectMany(const object item)
{
    List* L = CreateList(4);
    List_Add(L, item);
    List_Add(L, item);
    List_Add(L, item);
    return (IEnumerable*)L;
}

object aggregate(const object accumulate, const object item)
{
    int* result = new(int);
    *result = *(int*)accumulate + *(int*)item;
    return result;
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
    IEnumerable* enumerable = (IEnumerable*)list;
    ShowValues(enumerable);
    printf("Selector function that adds 3:\n");
    enumerable = Enumerable_Select(enumerable, selector);
    ShowValues(enumerable);
    printf("Selector function that divides by 2:\n");
    enumerable = Enumerable_Select(enumerable, selector2);
    ShowValues(enumerable);
    printf("Where numbers are even:\n");
    enumerable = Enumerable_Where(enumerable, filter);
    ShowValues(enumerable);
    printf("Project each value into an array of three times the thing:\n");
    enumerable = Enumerable_SelectMany(enumerable, selectMany);
    ShowValues(enumerable);
    printf("Take only the first five items:\n");
    enumerable = Enumerable_Take(enumerable, 5);
    ShowValues(enumerable);
    printf("Skip the first two items:\n");
    enumerable = Enumerable_Skip(enumerable, 2);
    ShowValues(enumerable);
    printf("Original list enumeration:\n");
    enumerable = (IEnumerable*)list;
    ShowValues(enumerable);
    printf("First even number in the list: %d\n", *(int*)Enumerable_FirstOrDefault(enumerable, filter));
    printf("Index of first even number in the list: %d\n", Enumerable_IndexOf(enumerable, Enumerable_FirstOrDefault(enumerable, filter)));
    printf("Sum of all values: %d\n", *(int*)Enumerable_Aggregate(enumerable, aggregate));
}
