#ifndef COLLECTIONS_GENERIC_LIST_IMPLEMENTATIONS
#define COLLECTIONS_GENERIC_LIST_IMPLEMENTATIONS
#include "Collections/Generic/ListT.h"

#pragma region Implement

#define LIST_IMPLEMENT(T)                                                                       \
typedef struct list_enumerator_s {                                                              \
    IEnumerator_##T _parent;                                                                    \
    int _currentIndex;                                                                          \
    List_##T* _list;                                                                            \
} ListEnumerator_##T;                                                                           \
static bool ListMoveNext_##T(IEnumerator_##T *This)                                             \
{                                                                                               \
    ListEnumerator_##T *e = (ListEnumerator_##T*)This;                                          \
    if (e->_currentIndex < e->_list->Count) {                                                   \
        This->Current = e->_list->Values[e->_currentIndex];                                     \
        e->_currentIndex += 1;                                                                  \
        return true;                                                                            \
    }                                                                                           \
    return false;                                                                               \
}                                                                                               \
static void ListReset_##T(IEnumerator_##T *This)                                                \
{                                                                                               \
    ((ListEnumerator_##T*)This)->_currentIndex = 0;                                             \
    This->Current = (T){0};                                                                     \
}                                                                                               \
static void ListDispose_##T(IEnumerator_##T *This)                                              \
{                                                                                               \
    free(This);                                                                                 \
}                                                                                               \
static IEnumerator_##T* ListGetEnumerator_##T(const IEnumerable_##T* This)                      \
{                                                                                               \
    ListEnumerator_##T* result = alloc(ListEnumerator_##T);                                     \
    *result = (ListEnumerator_##T) {                                                            \
        ._parent = (IEnumerator_##T) {                                                          \
            .MoveNext = ListMoveNext_##T,                                                       \
            .Reset = ListReset_##T,                                                             \
            .Dispose = ListDispose_##T                                                          \
        },                                                                                      \
        ._currentIndex = 0,                                                                     \
        ._list = (List_##T*)This                                                                \
    };                                                                                          \
    return (IEnumerator_##T*)result;                                                            \
}                                                                                               \
void List_##T##_EnsureCapacity(List_##T* source, int capacity)                                  \
{                                                                                               \
    if (source->Capacity < capacity) {                                                          \
        source->Values = realloc(source->Values, sizeof(T) * capacity);                         \
        source->Capacity = capacity;                                                            \
    }                                                                                           \
}                                                                                               \
void List_##T##_TrimExcess(List_##T* source)                                                    \
{                                                                                               \
    if (source->Count < source->Capacity * 0.9) {                                               \
        source->Values = realloc(source->Values, sizeof(T) * source->Count);                    \
        source->Capacity = source->Count;                                                       \
    }                                                                                           \
}                                                                                               \
void List_##T##_Add(List_##T* source, T item)                                                   \
{                                                                                               \
    if (source->Count >= source->Capacity) {                                                    \
        List_##T##_EnsureCapacity(source, source->Capacity * 2);                                \
    }                                                                                           \
    source->Values[source->Count++] = item;                                                     \
}                                                                                               \
void List_##T##_Remove(List_##T* source, T item)                                                \
{                                                                                               \
    for (int i = 0; i < source->Count; ++i) {                                                   \
        if (ValueEquator(sizeof(T), &source->Values[i], &item)) {                               \
            source->Count -= 1;                                                                 \
            for (int j = i; j < source->Count; ++j) {                                           \
                source->Values[j] = source->Values[j + 1];                                      \
            }                                                                                   \
            source->Values[source->Count] = (T){0};                                             \
            return;                                                                             \
        }                                                                                       \
    }                                                                                           \
}                                                                                               \
void List_##T##_Clear(List_##T* source)                                                         \
{                                                                                               \
    for (int i = 0; i < source->Count; ++i) {                                                   \
        source->Values[i] = (T){0};                                                             \
    }                                                                                           \
    source->Count = 0;                                                                          \
}                                                                                               \
void List_##T##_Insert(List_##T* source, T item, int index)                                     \
{                                                                                               \
    if (source->Count >= source->Capacity) {                                                    \
        List_##T##_EnsureCapacity(source, source->Capacity * 2);                                \
    }                                                                                           \
    for (int i = source->Count; i >= index; --i) {                                              \
        source->Values[i + 1] = source->Values[i];                                              \
    }                                                                                           \
    source->Values[index] = item;                                                               \
}                                                                                               \
void List_##T##_ForEach(List_##T* source, void (*action)(T*))                                   \
{                                                                                               \
    for (int i = 0; i < source->Count; ++i) {                                                   \
        action(&source->Values[i]);                                                             \
    }                                                                                           \
}                                                                                               \
List_##T* List_##T##__ctor(int capacity)                                                        \
{                                                                                               \
    List_##T* result = alloc(List_##T);                                                         \
    *result = (List_##T) {                                                                      \
        .Capacity = capacity,                                                                   \
        .Count = 0,                                                                             \
        ._parent = (IEnumerable_##T) {                                                          \
            .GetEnumerator = ListGetEnumerator_##T                                              \
        }                                                                                       \
    };                                                                                          \
    if (capacity > 0) {                                                                         \
        result->Values = alloc_array(T, capacity);                                              \
    }                                                                                           \
    return result;                                                                              \
}                                                                                               \
void DestroyList_##T(List_##T** list)                                                           \
{                                                                                               \
    free((*list)->Values);                                                                      \
    free(*list);                                                                                \
    *list = NULL;                                                                               \
}                                                                                               \
List_##T* Enumerable_##T##_ToList(IEnumerable_##T *source)                                      \
{                                                                                               \
    /* Assume initial capacity */                                                               \
    int capacity = 16;                                                                          \
    List_##T *result = alloc(List_##T);                                                         \
    *result = (List_##T) {                                                                      \
        ._parent = (IEnumerable_##T) {                                                          \
            .GetEnumerator = ListGetEnumerator_##T                                              \
        },                                                                                      \
        .Count = 0,                                                                             \
        .Capacity = capacity,                                                                   \
        .Values = alloc_array(T, capacity)                                                      \
    };                                                                                          \
    for (IEnumerator_##T *e = source->GetEnumerator(source); e->MoveNext(e); ++result->Count) { \
        if (result->Count == capacity) {                                                        \
            List_##T##_EnsureCapacity(result, capacity * 2);                                    \
        }                                                                                       \
        result->Values[result->Count] = e->Current;                                             \
    }                                                                                           \
    List_##T##_TrimExcess(result);                                                              \
    return result;                                                                              \
}

#pragma endregion
#endif