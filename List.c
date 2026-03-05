#include "List.h"

typedef struct list_enumerator_s {
    IEnumerator _parent;
    int _currentIndex;
    List *_list;
} ListEnumerator;

static bool ListMoveNext(IEnumerator *This)
{
    ListEnumerator *LE = (ListEnumerator*)This;
    if (LE->_currentIndex < LE->_list->Count) {
        This->Current = LE->_list->_items[LE->_currentIndex];
        LE->_currentIndex += 1;
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
    ListEnumerator* result = new(ListEnumerator);
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
        source->_items = realloc(source->_items, sizeof(void*) * capacity);
        source->Capacity = capacity;
    }
}

/// @brief Sets the capacity of the list to the amount of elements, if that amount is less than 90% of current capacity.
/// @param source List to trim the excess of.
void List_TrimExcess(List* source)
{
    if (source->Count < source->Capacity * 0.9) {
        source->_items = realloc(source->_items, sizeof(void*) * source->Count);
        source->Capacity = source->Count;
    }
}

/// @brief Add an element to the end of the list.
/// @param list List to add an element to.
/// @param item Item to add to the end of the list.
void List_Add(List* source, const object item)
{
    if (source->Count >= source->Capacity) {
        List_EnsureCapacity(source, source->Capacity * 2);
    }
    source->_items[source->Count++] = item;
}

/// @brief Removes an element from the list.
/// @param source List to remove the element from.
/// @param item Item to remove from the list.
void List_Remove(List* source, const object item)
{
    for (int i = 0; i < source->Count; ++i) {
        if (source->_items[i] == item) {
            source->Count -= 1;
            for (int j = i; j < source->Count; ++j) {
                source->_items[j] = source->_items[j + 1];
            }
            return;
        }
    }
}

/// @brief Insert an item into the list at the given index.
/// @param source List to insert the element into.
/// @param item Item to insert into the list.
/// @param index Index to insert the item at.
void List_Insert(List* source, const object item, int index)
{
    if (source->Count >= source->Capacity) {
        List_EnsureCapacity(source, source->Capacity * 2);
    }
    for (int i = source->Count; i >= index; --i) {
        source->_items[i + 1] = source->_items[i];
    }
    source->_items[index] = item;
}

/// @brief Creates a new List with the given capacity.
/// @param capacity Initial capacity for the created list.
/// @return A new List with the given capacity.
List* CreateList(int capacity)
{
    List* result = new(List);
    *result = (List) {
        .Capacity = capacity,
        .Count = 0,
        ._items = new_arr(object, capacity),
        ._parent = (IEnumerable) {
            .GetEnumerator = ListGetEnumerator
        }
    };
    return result;
}

/// @brief Frees all memory relating to a list.
/// @param list 
void DestroyList(List** list)
{
    free((*list)->_items);
    free(*list);
    *list = NULL;
}

List* Enumerable_ToList(IEnumerable *source)
{
    // Assume initial capacity
    int capacity = 16;
    List *result = new(List);
    *result = (List) {
        ._parent = (IEnumerable) {
            .GetEnumerator = ListGetEnumerator
        },
        .Count = 0,
        .Capacity = capacity,
        ._items = malloc(sizeof(void*) * capacity)
    };
    for (IEnumerator *e = source->GetEnumerator(source); e->MoveNext(e); ++result->Count) {
        if (result->Count == capacity) {
            result->_items = realloc(result->_items, capacity *= 2);
        }
        result->_items[result->Count] = e->Current;
    }
    return result;
}
