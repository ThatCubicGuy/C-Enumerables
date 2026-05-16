#include "Collections/LinkedList.h"
#include "Keywords.h"

#pragma region Enumerator

typedef TAG(LinkedListEnumerator) {
    IMPL(IEnumerator);
    LinkedList _source;
    LinkedNode _currentNode;
} *LinkedListEnumerator;

static bool LinkedListMoveNext(IEnumerator This)
{
    LinkedListEnumerator LLE = (LinkedListEnumerator)This;
    if (LLE->_currentNode == NULL) {
        if (LLE->_source->_start == NULL) return false;
        LLE->_currentNode = LLE->_source->_start;
        This->Current = LLE->_currentNode->Value;
        return true;
    }
    if ((LLE->_currentNode)->Next == NULL) return false;
    LLE->_currentNode = LLE->_currentNode->Next;
    This->Current = LLE->_currentNode->Value;
    return true;
}

static void LinkedListReset(IEnumerator This)
{
    ((LinkedListEnumerator)This)->_currentNode = NULL;
    This->Current = NULL;
}

static void LinkedListDispose(IEnumerator This)
{
    memfree(This);
}

static IEnumerator LinkedListGetEnumerator(const IEnumerable This)
{
    auto result = meminit(LinkedListEnumerator) {
        .MoveNext = LinkedListMoveNext,
        .Reset = LinkedListReset,
        .Dispose = LinkedListDispose,
        ._currentNode = NULL,
        ._source = (LinkedList)This
    };
    return (IEnumerator)(result);
}

#pragma endregion

#pragma region Methods

/// @brief Add an element to the end of the list.
/// @param list List to add an element to.
/// @param item Item to add to the end of the list.
void LinkedList_Add(LinkedList source, object item)
{
    if (source->Count > 0) {
        source->Count += 1;
        source->_end->Next = meminit(LinkedNode) {
            .Next = NULL,
            .Value = item
        };
        source->_end = source->_end->Next;
        return;
    }

    source->_start = meminit(LinkedNode) default(TAG(LinkedNode));
    source->_end = source->_start;
    source->_start->Value = item;
    source->Count = 1;
}

static void RemoveNodes(LinkedNode startingPoint)
{
    if (startingPoint == NULL) return;
    if (startingPoint->Next != NULL) {
        RemoveNodes(startingPoint->Next);
    }
    memfree(startingPoint);
}

/// @brief Removes all elements from the linked list.
/// @param source Linked list to clear.
void LinkedList_Clear(LinkedList source)
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
void LinkedList_Insert(LinkedList source, object item, int index)
{
    if (index > source->Count) return;
    source->Count += 1;
    if (index == 0) {
        auto newNode = meminit(LinkedNode) {
            .Value = item,
            .Next = source->_start
        };
        source->_start = newNode;
        return;
    }

    if (index == source->Count - 1) {
        LinkedNode newNode = meminit(LinkedNode) {
            .Value = item,
            .Next = NULL,
        };
        source->_end->Next = newNode;
        source->_end = newNode;
        return;
    }

    LinkedNode current = source->_start;
    while (index > 1) {
        index -= 1;
        current = current->Next;
    }
    LinkedNode newNode = meminit(LinkedNode) {
        .Value = item,
        .Next = current->Next,
    };
    current->Next = newNode;
}

/// @brief Creates a new instance of the LinkedList enumerable.
/// @return A new LinkedList with no elements.
LinkedList LinkedList__ctor()
{
    LinkedList result = meminit(LinkedList) {
        .GetEnumerator = LinkedListGetEnumerator,
        .Count = 0,
        ._start = NULL,
        ._end = NULL
    };
    return result;
}

/// @brief Frees up all memory occupied by a linked list.
/// @param source List to destroy.
void LinkedList_Destroy(LinkedList* source)
{
    LinkedList_Clear(*source);
    memfree(*source);
    *source = NULL;
}

static void ReverseNodes(LinkedNode previous, LinkedNode current)
{
    if (!current) return;
    if (current->Next) ReverseNodes(current, current->Next);
    current->Next = previous;
}

void LinkedList_Reverse(LinkedList source)
{
    if (source->Count <= 1) return;
    ReverseNodes(source->_start, source->_start->Next);
    source->_start->Next = NULL;
    LinkedNode last = source->_end;
    source->_end = source->_start;
    source->_start = last;

}

/// @brief Creates a LinkedList from a static array.
/// @param itemCount Amount of items in the array.
/// @param items Array of items.
/// @return A new LinkedList with elements from the array.
LinkedList CreateLinkedListFromArray(int itemCount, object items[])
{
    LinkedList result = meminit(LinkedList) {
        .GetEnumerator = LinkedListGetEnumerator,
        .Count = itemCount,
        ._start = NULL,
        ._end = NULL
    };
    if (itemCount == 0) return result;
    LinkedNode current = meminit(LinkedNode) default(TAG(LinkedNode));
    result->_start = current;
    current->Value = items[0];
    for (int i = 1; i < itemCount; ++i) {
        current->Next = meminit(LinkedNode) default(TAG(LinkedNode));
        current = current->Next;
        current->Value = items[i];
    }
    return result;
}

/// @brief Creates a LinkedList from an IEnumerable.
/// @param source Source enumerable to take items from.
/// @return A new LinkedList with elements from source.
LinkedList Enumerable_ToLinkedList(IEnumerable source)
{
    LinkedList result = meminit(LinkedList) default(TAG(LinkedList));
    foreach_as(object, item, source, LinkedList_Add(result, item));
    return result;
}

#pragma endregion