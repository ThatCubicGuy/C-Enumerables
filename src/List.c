#include "List.h"

typedef struct list_enumerator_s {
    IEnumerator _parent;
    int _currentIndex;
    List* _list;
} ListEnumerator;

static bool ListMoveNext(IEnumerator *This)
{
    ListEnumerator *e = (ListEnumerator*)This;
    if (e->_currentIndex < e->_list->Count) {
        This->Current = e->_list->Values[e->_currentIndex];
        e->_currentIndex += 1;
        return true;
    }
    return false;
}

static void ListReset(IEnumerator *This)
{
    ((ListEnumerator*)This)->_currentIndex = 0;
    This->Current = NULL;
}

static void ListDispose(IEnumerator *This)
{
    free(This);
}

static IEnumerator* ListGetEnumerator(const IEnumerable* This)
{
    ListEnumerator* result = bare(ListEnumerator);
    *result = (ListEnumerator) {
        ._parent = (IEnumerator) {
            .MoveNext = ListMoveNext,
            .Reset = ListReset,
            .Dispose = ListDispose
        },
        ._currentIndex = 0,
        ._list = (List*)This
    };
    return (IEnumerator*)result;
}

/// @brief Ensures the given list has the capacity to hold a given amount of items.
/// @param source List to ensure the capacity for.
/// @param capacity Capacity to ensure.
void List_EnsureCapacity(List* source, int capacity)
{
    if (source->Capacity < capacity) {
        source->Values = realloc(source->Values, sizeof(void*) * capacity);
        source->Capacity = capacity;
    }
}

/// @brief Sets the capacity of the list to the amount of elements, if that amount is less than 90% of current capacity.
/// @param source List to trim the excess of.
void List_TrimExcess(List* source)
{
    if (source->Count < source->Capacity * 0.9) {
        source->Values = realloc(source->Values, sizeof(void*) * source->Count);
        source->Capacity = source->Count;
    }
}

/// @brief Add an element to the end of the list.
/// @param list List to add an element to.
/// @param item Item to add to the end of the list.
void List_Add(List* source, object item)
{
    if (source->Count >= source->Capacity) {
        List_EnsureCapacity(source, source->Capacity * 2);
    }
    source->Values[source->Count++] = item;
}

/// @brief Removes an element from the list.
/// @param source List to remove the element from.
/// @param item Item to remove from the list.
void List_Remove(List* source, object item)
{
    for (int i = 0; i < source->Count; ++i) {
        if (source->Values[i] == item) {
            source->Count -= 1;
            for (int j = i; j < source->Count; ++j) {
                source->Values[j] = source->Values[j + 1];
            }
            source->Values[source->Count] = NULL;
            return;
        }
    }
}

/// @brief Removes all elements from the list.
/// @param source List to clear.
void List_Clear(List* source)
{
    for (int i = 0; i < source->Count; ++i) {
        source->Values[i] = NULL;
    }
    source->Count = 0;
}

/// @brief Insert an item into the list at the given index.
/// @param source List to insert the element into.
/// @param item Item to insert into the list.
/// @param index Index to insert the item at.
void List_Insert(List* source, object item, int index)
{
    if (source->Count >= source->Capacity) {
        List_EnsureCapacity(source, source->Capacity * 2);
    }
    for (int i = source->Count; i >= index; --i) {
        source->Values[i + 1] = source->Values[i];
    }
    source->Values[index] = item;
}

/// @brief Creates a new List with the given capacity.
/// @param capacity Initial capacity for the created list.
/// @return A new List with the given capacity.
List* List__ctor(int capacity)
{
    List* result = bare(List);
    *result = (List) {
        .Capacity = capacity,
        .Count = 0,
        ._parent = (IEnumerable) {
            .GetEnumerator = ListGetEnumerator
        }
    };
    if (capacity > 0) {
        result->Values = new_array(object, capacity);
    }
    return result;
}

/// @brief Frees all memory relating to a list.
/// @param list List to destroy.
void DestroyList(List** list)
{
    free((*list)->Values);
    free(*list);
    *list = NULL;
}

/// @brief Creates a List from an IEnumerable.
/// @param source Enumerable to take items from.
/// @return A new list.
List* Enumerable_ToList(IEnumerable *source)
{
    // Assume initial capacity
    int capacity = 16;
    List *result = bare(List);
    *result = (List) {
        ._parent = (IEnumerable) {
            .GetEnumerator = ListGetEnumerator
        },
        .Count = 0,
        .Capacity = capacity,
        .Values = malloc(sizeof(void*) * capacity)
    };
    for (IEnumerator *e = source->GetEnumerator(source); e->MoveNext(e); ++result->Count) {
        if (result->Count == capacity) {
            result->Values = realloc(result->Values, capacity *= 2);
        }
        result->Values[result->Count] = e->Current;
    }
    List_TrimExcess(result);
    return result;
}
