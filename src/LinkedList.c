#include "Collections/LinkedList.h"
#include "Defines.h"

#pragma region Enumerator

typedef struct LinkedListEnumerator_s {
    struct IEnumerator_s _parent;
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
    free(This);
}

static IEnumerator LinkedListGetEnumerator(const IEnumerable This)
{
    LinkedListEnumerator allocinit(LinkedListEnumerator, result) {
        ._parent = (struct IEnumerator_s) {
            .MoveNext = LinkedListMoveNext,
            .Reset = LinkedListReset,
            .Dispose = LinkedListDispose
        },
        ._currentNode = NULL,
        ._source = (LinkedList)This
    };
    return base(result);
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
        allocinit(LinkedNode, source->_end->Next) {
            .Next = NULL,
            .Value = item
        };
        source->_end = source->_end->Next;
        return;
    }

    allocinit(LinkedNode, source->_start) default(struct LinkedNode_s);
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
    free(startingPoint);
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
        LinkedNode allocinit(LinkedNode, newNode) {
            .Value = item,
            .Next = source->_start
        };
        source->_start = newNode;
        return;
    }

    if (index == source->Count - 1) {
        LinkedNode allocinit(LinkedNode, newNode) {
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
    LinkedNode allocinit(LinkedNode, newNode) {
        .Value = item,
        .Next = current->Next,
    };
    current->Next = newNode;
}

/// @brief Creates a new instance of the LinkedList enumerable.
/// @return A new LinkedList with no elements.
LinkedList LinkedList__ctor()
{
    LinkedList allocinit(LinkedList, result) {
        ._parent = (struct IEnumerable_s) {
            .GetEnumerator = LinkedListGetEnumerator
        },
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
    free(*source);
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
    LinkedList allocinit(LinkedList, result) {
        ._parent = (struct IEnumerable_s) {
            .GetEnumerator = LinkedListGetEnumerator
        },
        .Count = itemCount,
        ._start = NULL,
        ._end = NULL
    };
    if (itemCount == 0) return result;
    LinkedNode allocinit(LinkedNode, current) default(struct LinkedNode_s);
    result->_start = current;
    current->Value = items[0];
    for (int i = 1; i < itemCount; ++i) {
        allocinit(LinkedNode, current->Next) default(struct LinkedNode_s);
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
    LinkedList allocinit(LinkedList, result) default(struct LinkedList_s);
    foreach_as(object, item, source, LinkedList_Add(result, item));
    return result;
}

#pragma endregion