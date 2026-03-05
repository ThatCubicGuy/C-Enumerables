#include "Enumerable.h"
#include "LinkedList.h"
#include "Defines.h"

#pragma region Enumerator

typedef struct linked_list_enumerator_s {
    IEnumerator _parent;
    LinkedList *_source;
    LinkedNode *_currentNode;
} LinkedListEnumerator;

static bool LinkedListMoveNext(IEnumerator *This)
{
    LinkedListEnumerator *LLE = (LinkedListEnumerator*)This;
    if (LLE->_currentNode == NULL) {
        if (LLE->_source->_start == NULL) return false;
        LLE->_currentNode = LLE->_source->_start;
        This->Current = LLE->_source->_start->Value;
        return true;
    }
    if (((LinkedNode*)LLE->_currentNode)->Next == NULL) return false;
    LLE->_currentNode = LLE->_currentNode->Next;
    This->Current = LLE->_currentNode->Value;
    return true;
}

static void LinkedListReset(IEnumerator *This)
{
    This->Current = NULL;
}

static void LinkedListDispose(IEnumerator *This)
{
    free(This);
}

static IEnumerator* LinkedListGetEnumerator(const IEnumerable *This)
{
    LinkedListEnumerator *result = bare(LinkedListEnumerator);
    *result = (LinkedListEnumerator) {
        ._parent = (IEnumerator) {
            .MoveNext = LinkedListMoveNext,
            .Reset = LinkedListReset,
            .Dispose = LinkedListDispose
        },
        ._source = (LinkedList*)This
    };
    return (IEnumerator*)result;
}

#pragma endregion

#pragma region Methods

/// @brief Add an element to the end of the list.
/// @param list List to add an element to.
/// @param item Item to add to the end of the list.
void LinkedList_Add(LinkedList* source, object item)
{
    if (source->Count > 0) {
        source->Count += 1;
        source->_end->Next = bare(LinkedNode);
        source->_end = source->_end->Next;
        source->_end->Value = item;
        return;
    }

    source->_start = bare(LinkedNode);
    source->_end = source->_start;
    source->_start->Value = item;
    source->Count = 1;
}

static void RemoveNodes(LinkedNode* startingPoint)
{
    if (startingPoint->Next != NULL) {
        RemoveNodes(startingPoint->Next);
    }
    free(startingPoint);
}

/// @brief Removes all elements from the linked list.
/// @param source Linked list to clear.
void LinkedList_Clear(LinkedList* source)
{
    RemoveNodes(source->_start);
    source->_start = NULL;
    source->_end = NULL;
    source->Count = 0;
}

/// @brief Insert an item into the list at the given index.
/// @param source List to insert the element into.
/// @param item Item to insert into the list.
/// @param index Index to insert the item at.
void LinkedList_Insert(LinkedList* source, object item, int index)
{
    if (index > source->Count) return;
    source->Count += 1;
    if (index == 0) {
        LinkedNode* newNode = bare(LinkedNode);
        *newNode = (LinkedNode) {
            .Value = item,
            .Next = source->_start
        };
        source->_start = newNode;
        return;
    }

    if (index == source->Count - 1) {
        LinkedNode* newNode = bare(LinkedNode);
        *newNode = (LinkedNode) {
            .Value = item,
        };
        source->_end->Next = newNode;
        source->_end = newNode;
        return;
    }

    LinkedNode* current = source->_start;
    while (index > 1) {
        index -= 1;
        current = current->Next;
    }
    LinkedNode* newNode = bare(LinkedNode);
    *newNode = (LinkedNode) {
        .Value = item,
        .Next = current->Next,
    };
    current->Next = newNode;
}

/// @brief Creates a new instance of the LinkedList enumerable.
/// @return A new LinkedList with no elements.
LinkedList* LinkedList__ctor()
{
    LinkedList *result = bare(LinkedList);
    *result = (LinkedList) {
        ._parent = (IEnumerable) {
            .GetEnumerator = LinkedListGetEnumerator
        },
        .Count = 0,
        ._start = NULL,
        ._end = NULL
    };
    return result;
}

/// @brief Creates a LinkedList from a static array.
/// @param itemCount Amount of items in the array.
/// @param items Array of items.
/// @return A new LinkedList with elements from the array.
LinkedList* CreateLinkedListFromArray(int itemCount, object items[])
{
    LinkedList *result = bare(LinkedList);
    *result = (LinkedList) {
        ._parent = (IEnumerable) {
            .GetEnumerator = LinkedListGetEnumerator
        },
        .Count = itemCount,
        ._start = NULL,
        ._end = NULL
    };
    if (itemCount == 0) return result;
    LinkedNode *current = bare(LinkedNode);
    result->_start = current;
    current->Value = items[0];
    for (int i = 1; i < itemCount; ++i) {
        current->Next = bare(LinkedNode);
        current = current->Next;
        current->Value = items[i];
    }
    return result;
}

/// @brief Creates a LinkedList from an IEnumerable.
/// @param source Source enumerable to take items from.
/// @return A new LinkedList with elements from source.
LinkedList* Enumerable_ToLinkedList(IEnumerable* source)
{
    LinkedList* result = bare(LinkedList);
    IEnumerator* e = source->GetEnumerator(source);
    if (e->MoveNext(e)) LinkedList_Add(result, e->Current);
    while (e->MoveNext(e)) {
        LinkedList_Add(result, e->Current);
    }
    return result;
}

#pragma endregion