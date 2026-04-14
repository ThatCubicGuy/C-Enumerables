#ifndef COLLECTIONS_LINKED_LIST_IMPLEMENTATIONS
#define COLLECTIONS_LINKED_LIST_IMPLEMENTATIONS
#include "Collections/Generic/LinkedListT.h"

#pragma region Implement

#define LINKED_LIST_IMPLEMENT(T)                                                    \
struct LinkedNode_##T##_s {                                                         \
    struct LinkedNode_##T##_s *Next;                                                \
    T Value;                                                                        \
};                                                                                  \
typedef struct LinkedListEnumerator_##T##_s {                                       \
    struct _IEnumerator_##T##_s _parent;                                            \
    LinkedList(T) _source;                                                          \
    LinkedNode_##T _currentNode;                                                    \
} *LinkedListEnumerator_##T;                                                        \
static bool LinkedListMoveNext_##T(IEnumerator(T) This)                             \
{                                                                                   \
    LinkedListEnumerator_##T LLE = (LinkedListEnumerator_##T)This;                  \
    if (LLE->_currentNode == NULL) {                                                \
        if (LLE->_source->_start == NULL) return false;                             \
        LLE->_currentNode = LLE->_source->_start;                                   \
        This->Current = LLE->_currentNode->Value;                                   \
        return true;                                                                \
    }                                                                               \
    if ((LLE->_currentNode)->Next == NULL) return false;                            \
    LLE->_currentNode = LLE->_currentNode->Next;                                    \
    This->Current = LLE->_currentNode->Value;                                       \
    return true;                                                                    \
}                                                                                   \
static void LinkedListReset_##T(IEnumerator(T) This)                                \
{                                                                                   \
    ((LinkedListEnumerator_##T)This)->_currentNode = NULL;                          \
    This->Current = default(T);                                                     \
}                                                                                   \
static void LinkedListDispose_##T(IEnumerator(T) This)                              \
{                                                                                   \
    free(This);                                                                     \
}                                                                                   \
static IEnumerator(T) LinkedListGetEnumerator_##T(const IEnumerable(T) This)        \
{                                                                                   \
    LinkedListEnumerator_##T allocinit(LinkedListEnumerator_##T, result) {          \
        ._parent = (struct _IEnumerator_##T##_s) {                                  \
            .MoveNext = LinkedListMoveNext_##T,                                     \
            .Reset = LinkedListReset_##T,                                           \
            .Dispose = LinkedListDispose_##T                                        \
        },                                                                          \
        ._currentNode = NULL,                                                       \
        ._source = (LinkedList(T))This                                              \
    };                                                                              \
    return base(result);                                                            \
}                                                                                   \
void LinkedList_##T##_Add(LinkedList(T) source, T item)                             \
{                                                                                   \
    if (source->Count > 0) {                                                        \
        source->Count += 1;                                                         \
        allocinit(LinkedNode_##T, source->_end->Next) {                             \
            .Next = NULL,                                                           \
            .Value = item                                                           \
        };                                                                          \
        source->_end = source->_end->Next;                                          \
        return;                                                                     \
    }                                                                               \
    allocinit(LinkedNode_##T, source->_start) default(struct LinkedNode_##T##_s);   \
    source->_end = source->_start;                                                  \
    source->_start->Value = item;                                                   \
    source->Count = 1;                                                              \
}                                                                                   \
static void RemoveNodes_##T(LinkedNode_##T startingPoint)                           \
{                                                                                   \
    if (startingPoint == NULL) return;                                              \
    if (startingPoint->Next != NULL) {                                              \
        RemoveNodes_##T(startingPoint->Next);                                       \
    }                                                                               \
    free(startingPoint);                                                            \
}                                                                                   \
void LinkedList_##T##_Clear(LinkedList(T) source)                                   \
{                                                                                   \
    RemoveNodes_##T(source->_start);                                                \
    source->_start = NULL;                                                          \
    source->_end = NULL;                                                            \
    source->Count = 0;                                                              \
}                                                                                   \
void LinkedList_##T##_Insert(LinkedList(T) source, T item, int index)               \
{                                                                                   \
    if (index > source->Count) return;                                              \
    source->Count += 1;                                                             \
    if (index == 0) {                                                               \
        LinkedNode_##T allocinit(LinkedNode_##T, newNode) {                         \
            .Value = item,                                                          \
            .Next = source->_start                                                  \
        };                                                                          \
        source->_start = newNode;                                                   \
        return;                                                                     \
    }                                                                               \
    if (index == source->Count - 1) {                                               \
        LinkedNode_##T allocinit(LinkedNode_##T, newNode) {                         \
            .Value = item,                                                          \
            .Next = NULL                                                            \
        };                                                                          \
        source->_end->Next = newNode;                                               \
        source->_end = newNode;                                                     \
        return;                                                                     \
    }                                                                               \
    LinkedNode_##T current = source->_start;                                        \
    while (index > 1) {                                                             \
        index -= 1;                                                                 \
        current = current->Next;                                                    \
    }                                                                               \
    LinkedNode_##T allocinit(LinkedNode_##T, newNode) {                             \
        .Value = item,                                                              \
        .Next = current->Next,                                                      \
    };                                                                              \
    current->Next = newNode;                                                        \
}                                                                                   \
LinkedList(T) new(LinkedList(T))()                                                  \
{                                                                                   \
    LinkedList(T) allocinit(LinkedList(T), result) {                                \
        ._parent = (struct _IEnumerable_##T##_s) {                                  \
            .GetEnumerator = LinkedListGetEnumerator_##T                            \
        },                                                                          \
        .Count = 0,                                                                 \
        ._start = NULL,                                                             \
        ._end = NULL                                                                \
    };                                                                              \
    return result;                                                                  \
}                                                                                   \
LinkedList(T) Enumerable_##T##_ToLinkedList(IEnumerable(T) source)                  \
{                                                                                   \
    LinkedList(T) allocinit(LinkedList(T), result) {                                \
        ._parent = (struct _IEnumerable_##T##_s) {                                  \
            .GetEnumerator = LinkedListGetEnumerator_##T                            \
        },                                                                          \
        ._start = NULL,                                                             \
        ._end = NULL,                                                               \
        .Count = 0                                                                  \
    };                                                                              \
    IEnumerator(T) e = source->GetEnumerator(source);                               \
    if (!e->MoveNext(e)) {                                                          \
        e->Dispose(e);                                                              \
        return result;                                                              \
    }                                                                               \
    allocinit(LinkedNode_##T, result->_start) {                                     \
        .Value = e->Current,                                                        \
        .Next = NULL                                                                \
    };                                                                              \
    LinkedNode_##T current = result->_start;                                        \
    while (e->MoveNext(e)) {                                                        \
        ++result->Count;                                                            \
        allocinit(LinkedNode_##T, current->Next) {                                  \
            .Value = e->Current,                                                    \
            .Next = NULL                                                            \
        };                                                                          \
        current = current->Next;                                                    \
    }                                                                               \
    result->_end = current;                                                         \
    e->Dispose(e);                                                                  \
    return result;                                                                  \
}                                                                                   \
void LinkedList_##T##_Destroy(LinkedList(T)* source)                                \
{                                                                                   \
    LinkedList_##T##_Clear(*source);                                                \
    free(*source);                                                                  \
    *source = NULL;                                                                 \
}                                                                                   \
static void ReverseNodes_##T(LinkedNode_##T previous, LinkedNode_##T current)       \
{                                                                                   \
    if (!current) return;                                                           \
    if (current->Next) ReverseNodes_##T(current, current->Next);                    \
    current->Next = previous;                                                       \
}                                                                                   \
void LinkedList_##T##_Reverse(LinkedList(T) source)                                 \
{                                                                                   \
    if (source->Count <= 1) return;                                                 \
    ReverseNodes_##T(source->_start, source->_start->Next);                         \
}                                                                                   \
void LinkedList_##T##_Sort(LinkedList(T) source, int (*comparer)(T, T))             \
{                                                                                   \
    for (LinkedNode_##T i = source->_start; i->Next; i = i->Next) {                 \
        for (LinkedNode_##T j = i; j; j = j->Next) {                                \
            if (comparer(i->Value, j->Value) < 0) {                                 \
                T tmp = i->Value;                                                   \
                i->Value = j->Value;                                                \
                j->Value = tmp;                                                     \
            }                                                                       \
        }                                                                           \
    }                                                                               \
}

#pragma endregion
#endif