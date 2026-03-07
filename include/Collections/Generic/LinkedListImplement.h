#ifndef COLLECTIONS_GENERIC_LINKED_LIST_IMPLEMENTATIONS
#define COLLECTIONS_GENERIC_LINKED_LIST_IMPLEMENTATIONS
#include "Collections/Generic/LinkedListT.h"

#pragma region Implement

#define LINKED_LIST_IMPLEMENT(T)                                                    \
typedef struct generic_linked_list_enumerator_##T##_s {                             \
    struct generic_enumerator_##T##_s _parent;                                      \
    LinkedList_##T _source;                                                         \
    LinkedNode_##T _currentNode;                                                    \
} *LinkedListEnumerator_##T;                                                        \
static bool LinkedListMoveNext_##T(IEnumerator_##T This)                            \
{                                                                                   \
    LinkedListEnumerator_##T LLE = (LinkedListEnumerator_##T)This;                  \
    if (LLE->_currentNode == NULL) {                                                \
        if (LLE->_source->_start == NULL) return false;                             \
        LLE->_currentNode = LLE->_source->_start;                                   \
        This->Current = LLE->_source->_start->Value;                                \
        return true;                                                                \
    }                                                                               \
    if ((LLE->_currentNode)->Next == NULL) return false;                            \
    LLE->_currentNode = LLE->_currentNode->Next;                                    \
    This->Current = LLE->_currentNode->Value;                                       \
    return true;                                                                    \
}                                                                                   \
static void LinkedListReset_##T(IEnumerator_##T This)                               \
{                                                                                   \
    ((LinkedListEnumerator_##T)This)->_currentNode = NULL;                          \
}                                                                                   \
static void LinkedListDispose_##T(IEnumerator_##T This)                             \
{                                                                                   \
    free(This);                                                                     \
}                                                                                   \
static IEnumerator_##T LinkedListGetEnumerator_##T(const IEnumerable_##T This)      \
{                                                                                   \
    LinkedListEnumerator_##T result =                                               \
        alloc(struct generic_linked_list_enumerator_##T##_s);                       \
    *result = (struct generic_linked_list_enumerator_##T##_s) {                     \
        ._parent = (struct generic_enumerator_##T##_s) {                            \
            .MoveNext = LinkedListMoveNext_##T,                                     \
            .Reset = LinkedListReset_##T,                                           \
            .Dispose = LinkedListDispose_##T                                        \
        },                                                                          \
        ._source = (LinkedList_##T)This                                             \
    };                                                                              \
    return base(result);                                                            \
}                                                                                   \
void LinkedList_##T##_Add(LinkedList_##T source, T item)                            \
{                                                                                   \
    if (source->Count > 0) {                                                        \
        source->Count += 1;                                                         \
        source->_end->Next = alloc(struct generic_linked_node_##T##_s);             \
        source->_end = source->_end->Next;                                          \
        source->_end->Value = item;                                                 \
        return;                                                                     \
    }                                                                               \
    source->_start = alloc(struct generic_linked_node_##T##_s);                     \
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
void LinkedList_##T##_Clear(LinkedList_##T source)                                  \
{                                                                                   \
    RemoveNodes_##T(source->_start);                                                \
    source->_start = NULL;                                                          \
    source->_end = NULL;                                                            \
    source->Count = 0;                                                              \
}                                                                                   \
void LinkedList_##T##_Insert(LinkedList_##T source, T item, int index)              \
{                                                                                   \
    if (index > source->Count) return;                                              \
    source->Count += 1;                                                             \
    if (index == 0) {                                                               \
        LinkedNode_##T newNode = alloc(struct generic_linked_node_##T##_s);         \
        *newNode = (struct generic_linked_node_##T##_s) {                           \
            .Value = item,                                                          \
            .Next = source->_start                                                  \
        };                                                                          \
        source->_start = newNode;                                                   \
        return;                                                                     \
    }                                                                               \
    if (index == source->Count - 1) {                                               \
        LinkedNode_##T newNode = alloc(struct generic_linked_node_##T##_s);         \
        *newNode = (struct generic_linked_node_##T##_s) {                           \
            .Value = item,                                                          \
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
    LinkedNode_##T newNode = alloc(struct generic_linked_node_##T##_s);             \
    *newNode = (struct generic_linked_node_##T##_s) {                               \
        .Value = item,                                                              \
        .Next = current->Next,                                                      \
    };                                                                              \
    current->Next = newNode;                                                        \
}                                                                                   \
LinkedList_##T LinkedList_##T##__ctor()                                             \
{                                                                                   \
    LinkedList_##T result = alloc(struct generic_linked_list_##T##_s);              \
    *result = (struct generic_linked_list_##T##_s) {                                \
        ._parent = (struct generic_enumerable_##T##_s) {                            \
            .GetEnumerator = LinkedListGetEnumerator_##T                            \
        },                                                                          \
        .Count = 0,                                                                 \
        ._start = NULL,                                                             \
        ._end = NULL                                                                \
    };                                                                              \
    return result;                                                                  \
}                                                                                   \
LinkedList_##T Enumerable_##T##_ToLinkedList(IEnumerable_##T source)                \
{                                                                                   \
    LinkedList_##T result = alloc(struct generic_linked_list_##T##_s);              \
    *result = (struct generic_linked_list_##T##_s) {                                \
        ._parent = (struct generic_enumerable_##T##_s) {                            \
            .GetEnumerator = LinkedListGetEnumerator_##T                            \
        },                                                                          \
        ._start = NULL,                                                             \
        ._end = NULL,                                                               \
        .Count = 0,                                                                 \
    };                                                                              \
    IEnumerator_##T e = source->GetEnumerator(source);                              \
    if (!e->MoveNext(e)) {                                                          \
        e->Dispose(e);                                                              \
        return result;                                                              \
    }                                                                               \
    result->_start = alloc(struct generic_linked_node_##T##_s);                     \
    LinkedNode_##T current = result->_start;                                        \
    current->Value = e->Current;                                                    \
    while (e->MoveNext(e)) {                                                        \
        ++result->Count;                                                            \
        current->Next = alloc(struct generic_linked_node_##T##_s);                  \
        current = current->Next;                                                    \
        current->Value = e->Current;                                                \
    }                                                                               \
    result->_end = current;                                                         \
    e->Dispose(e);                                                                  \
    return result;                                                                  \
}                                                                                   \
void DestroyLinkedList_##T(LinkedList_##T* source)                                  \
{                                                                                   \
    LinkedList_##T##_Clear(*source);                                                \
    free(*source);                                                                  \
    *source = NULL;                                                                 \
}

#pragma endregion
#endif