#ifndef COLLECTIONS_DOUBLY_LINKED_LIST_IMPLEMENTATIONS
#define COLLECTIONS_DOUBLY_LINKED_LIST_IMPLEMENTATIONS

#include "Collections/Generic/DoublyLinkedListT.h"

#pragma region Implement

#define DOUBLY_LINKED_LIST_IMPLEMENT(T)                                                 \
typedef struct DoublyLinkedListEnumerator_##T##_s {                                     \
    struct _IEnumerator_##T##_s _parent;                                                \
    DoublyLinkedNode_##T _sentinel;                                                     \
    DoublyLinkedNode_##T _currentNode;                                                  \
} *DoublyLinkedListEnumerator_##T;                                                      \
static bool DoublyLinkedListMoveNext_##T(IEnumerator(T) This)                           \
{                                                                                       \
    DoublyLinkedListEnumerator_##T DLLE = (DoublyLinkedListEnumerator_##T)This;         \
	if (DLLE->_currentNode == DLLE->_sentinel) return false;                            \
    This->Current = DLLE->_currentNode->Value;                                          \
    DLLE->_currentNode = DLLE->_currentNode->Next;                                      \
    return true;                                                                        \
}                                                                                       \
static void DoublyLinkedListReset_##T(IEnumerator(T) This)                              \
{                                                                                       \
    DoublyLinkedListEnumerator_##T e = (DoublyLinkedListEnumerator_##T)This;            \
    e->_currentNode = e->_sentinel;                                                     \
    This->Current = default(T);                                                         \
}                                                                                       \
static void DoublyLinkedListDispose_##T(IEnumerator(T) This)                            \
{                                                                                       \
    free(This);                                                                         \
}                                                                                       \
static IEnumerator(T) DoublyLinkedListGetEnumerator_##T(const IEnumerable(T) This)      \
{                                                                                       \
    DoublyLinkedListEnumerator_##T allocinit(DoublyLinkedListEnumerator_##T, result) {  \
        ._parent = (struct _IEnumerator_##T##_s) {                                      \
            .MoveNext = DoublyLinkedListMoveNext_##T,                                   \
            .Reset = DoublyLinkedListReset_##T,                                         \
            .Dispose = DoublyLinkedListDispose_##T                                      \
        },                                                                              \
        ._currentNode = ((DoublyLinkedList(T))This)->_start->Next,                      \
        ._sentinel = ((DoublyLinkedList(T))This)->_start                                \
    };                                                                                  \
    return base(result);                                                                \
}                                                                                       \
void DoublyLinkedList_##T##_Add(DoublyLinkedList(T) source, T item)                     \
{                                                                                       \
    source->Count += 1;                                                                 \
    allocinit(DoublyLinkedNode_##T, source->_start->Prev->Next) {                       \
        .Next = source->_start,                                                         \
        .Prev = source->_start->Prev,                                                   \
        .Value = item                                                                   \
    };                                                                                  \
    source->_start->Prev = source->_start->Prev->Next;                                  \
}                                                                                       \
void DoublyLinkedList_##T##_Remove(DoublyLinkedList(T) source, T item)                  \
{                                                                                       \
    DoublyLinkedNode_##T curr = source->_start->Next;                                   \
    while (!equals(curr->Value, item) && curr != source->_start) curr = curr->Next;     \
    if (curr == source->_start) return;                                                 \
    source->Count -= 1;                                                                 \
    curr->Prev->Next = curr->Next;                                                      \
    curr->Next->Prev = curr->Prev;                                                      \
    free(curr);                                                                         \
}                                                                                       \
static void RemoveDoublyNodes_##T(DoublyLinkedNode_##T curr, DoublyLinkedNode_##T sent) \
{                                                                                       \
    if (curr == sent) return;                                                           \
    RemoveDoublyNodes_##T(curr->Next, sent);                                            \
    free(curr);                                                                         \
}                                                                                       \
void DoublyLinkedList_##T##_Clear(DoublyLinkedList(T) source)                           \
{                                                                                       \
    RemoveDoublyNodes_##T(source->_start->Next, source->_start);                        \
    source->_start->Next = source->_start->Prev = source->_start;                       \
    source->Count = 0;                                                                  \
}                                                                                       \
void DoublyLinkedList_##T##_Insert(DoublyLinkedList(T) source, T item, int index)       \
{                                                                                       \
    if (index > source->Count) return;                                                  \
    source->Count += 1;                                                                 \
    DoublyLinkedNode_##T current = source->_start;                                      \
    while (index > 0) {                                                                 \
        index -= 1;                                                                     \
        current = current->Next;                                                        \
    }                                                                                   \
    DoublyLinkedNode_##T allocinit(DoublyLinkedNode_##T, newNode) {                     \
        .Value = item,                                                                  \
        .Next = current->Next,                                                          \
    };                                                                                  \
    current->Next = newNode;                                                            \
}                                                                                       \
DoublyLinkedList(T) new(DoublyLinkedList(T))()                                          \
{                                                                                       \
    DoublyLinkedList(T) allocinit(DoublyLinkedList(T), result) {                        \
        ._parent = (struct _IEnumerable_##T##_s) {                                      \
            .GetEnumerator = DoublyLinkedListGetEnumerator_##T                          \
        },                                                                              \
        .Count = 0,                                                                     \
    };                                                                                  \
    allocinit(DoublyLinkedNode_##T, result->_start) {                                   \
            .Next = result->_start,                                                     \
            .Prev = result->_start,                                                     \
            .Value = default(T)                                                         \
    };                                                                                  \
    return result;                                                                      \
}                                                                                       \
DoublyLinkedList(T) Enumerable_##T##_ToDoublyLinkedList(IEnumerable(T) source)          \
{                                                                                       \
    DoublyLinkedList(T) result = new(DoublyLinkedList(T))();                            \
    DoublyLinkedNode_##T current = result->_start;                                      \
    foreach(T, item, source, {                                                          \
        ++result->Count;                                                                \
        allocinit(DoublyLinkedNode_##T, current->Next) {                                \
            .Value = item,                                                              \
            .Next = result->_start,                                                     \
            .Prev = result->_start->Prev                                                \
        };                                                                              \
        current = current->Next;                                                        \
    });                                                                                 \
    return result;                                                                      \
}                                                                                       \
void DoublyLinkedList_##T##_Destroy(DoublyLinkedList(T)* source)                        \
{                                                                                       \
    DoublyLinkedList_##T##_Clear(*source);                                              \
    free((*source)->_start);                                                            \
    free(*source);                                                                      \
    *source = NULL;                                                                     \
}                                                                                       \
void DoublyLinkedList_##T##_Sort(DoublyLinkedList(T) source, int (*comparer)(T, T))     \
{                                                                                       \
    for (DoublyLinkedNode_##T i = source->_start->Next; i->Next != source->_start; i = i->Next) { \
        for (DoublyLinkedNode_##T j = i; j != source->_start; j = j->Next) {            \
            if (comparer(i->Value, j->Value) < 0) {                                     \
                T tmp = i->Value;                                                       \
                i->Value = j->Value;                                                    \
                j->Value = tmp;                                                         \
            }                                                                           \
        }                                                                               \
    }                                                                                   \
}

#pragma endregion
#endif
