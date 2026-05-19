#ifndef COLLECTIONS_GENERIC_LIST_IMPLEMENTATIONS
#define COLLECTIONS_GENERIC_LIST_IMPLEMENTATIONS
#include "ListT.h"
#define LIST_IMPLEMENT(T)                                                                       \
typedef TAG(ListEnumerator_##T) {                                                               \
    IMPL(IEnumerator(T));                                                                       \
    int _currentIndex;                                                                          \
    List(T) _list;                                                                              \
} *ListEnumerator_##T;                                                                          \
static bool ListMoveNext_##T(IEnumerator(T) This)                                               \
{                                                                                               \
    ListEnumerator_##T e = (ListEnumerator_##T)This;                                            \
    if (0 <= e->_currentIndex && e->_currentIndex < e->_list->Count) {                          \
        This->Current = e->_list->Values[e->_currentIndex];                                     \
        e->_currentIndex += 1;                                                                  \
        return true;                                                                            \
    }                                                                                           \
    return false;                                                                               \
}                                                                                               \
static void ListReset_##T(IEnumerator(T) This)                                                  \
{                                                                                               \
    ((ListEnumerator_##T)This)->_currentIndex = 0;                                              \
    This->Current = default(T);                                                                 \
}                                                                                               \
static void ListDispose_##T(IEnumerator(T) This)                                                \
{                                                                                               \
    memfree(This);                                                                              \
}                                                                                               \
static IEnumerator(T) ListGetEnumerator_##T(const IEnumerable(T) This)                          \
{                                                                                               \
    ListEnumerator_##T result = memalloc(ListEnumerator_##T);                                   \
    *result = init(ListEnumerator_##T) {                                                        \
        .MoveNext = ListMoveNext_##T,                                                           \
        .Reset = ListReset_##T,                                                                 \
        .Dispose = ListDispose_##T,                                                             \
        ._currentIndex = 0,                                                                     \
        ._list = (List(T))This                                                                  \
    };                                                                                          \
    return (IEnumerator(T))result;                                                              \
}                                                                                               \
List(T) new(List(T))(int capacity)                                                              \
{                                                                                               \
    if (capacity <= 0) throw new(ArgumentOutOfRangeException)(nameof(capacity), 1, 2147483647); \
    List(T) result = meminit(List(T)) {                                                         \
        .GetEnumerator = ListGetEnumerator_##T,                                                 \
        .Capacity = capacity,                                                                   \
        .Count = 0,                                                                             \
    };                                                                                          \
    if (capacity > 0) {                                                                         \
        result->Values = arralloc(T, capacity);                                                 \
    }                                                                                           \
    return result;                                                                              \
}                                                                                               \
void List_##T##_Destroy(List(T)* list)                                                          \
{                                                                                               \
    ThrowIfNull(list, *list);                                                                   \
    memfree((*list)->Values);                                                                   \
    memfree(*list);                                                                             \
    *list = NULL;                                                                               \
}                                                                                               \
List(T) Enumerable_##T##_ToList(IEnumerable(T) source)                                          \
{                                                                                               \
    ThrowIfNull(source);                                                                        \
    /* Assume initial capacity */                                                               \
    List(T) result = new(List(T))(16);                                                          \
    foreach (T item in source) {                                                                \
        List_##T##_Add(result, item);                                                           \
    }                                                                                           \
    List_##T##_TrimExcess(result);                                                              \
    return result;                                                                              \
}                                                                                               \
void List_##T##_EnsureCapacity(List(T) source, int capacity)                                    \
{                                                                                               \
    ThrowIfNull(source);                                                                        \
    if (source->Capacity < capacity) {                                                          \
        source->Values = memresize(source->Values, capacity);                                   \
        source->Capacity = capacity;                                                            \
    }                                                                                           \
}                                                                                               \
void List_##T##_TrimExcess(List(T) source)                                                      \
{                                                                                               \
    ThrowIfNull(source);                                                                        \
    if (source->Count > 0 && source->Count < source->Capacity * 0.9) {                          \
        source->Values = memresize(source->Values, source->Count);                              \
        source->Capacity = source->Count;                                                       \
    }                                                                                           \
}                                                                                               \
void List_##T##_Add(List(T) source, T item)                                                     \
{                                                                                               \
    ThrowIfNull(source);                                                                        \
    if (source->Count >= source->Capacity) {                                                    \
        List_##T##_EnsureCapacity(source, source->Capacity * 2);                                \
    }                                                                                           \
    source->Values[source->Count] = item;                                                       \
    source->Count += 1;                                                                         \
}                                                                                               \
void List_##T##_Remove(List(T) source, T item)                                                  \
{                                                                                               \
    ThrowIfNull(source);                                                                        \
    for (int i = 0; i < source->Count; ++i) {                                                   \
        if (equals(source->Values[i], item)) {                                                  \
            source->Count -= 1;                                                                 \
            for (int j = i; j < source->Count; ++j) {                                           \
                source->Values[j] = source->Values[j + 1];                                      \
            }                                                                                   \
            source->Values[source->Count] = default(T);                                         \
            return;                                                                             \
        }                                                                                       \
    }                                                                                           \
}                                                                                               \
void List_##T##_Clear(List(T) source)                                                           \
{                                                                                               \
    ThrowIfNull(source);                                                                        \
    for (int i = 0; i < source->Count; ++i) {                                                   \
        source->Values[i] = default(T);                                                         \
    }                                                                                           \
    source->Count = 0;                                                                          \
}                                                                                               \
void List_##T##_Insert(List(T) source, T item, int index)                                       \
{                                                                                               \
    ThrowIfNull(source);                                                                        \
    if (index < 0 || source->Count > index) {                                                   \
        throw new(IndexOutOfRangeException)(index, source->Count);                              \
    }                                                                                           \
    if (source->Count >= source->Capacity) {                                                    \
        List_##T##_EnsureCapacity(source, source->Capacity * 2);                                \
    }                                                                                           \
    for (int i = source->Count; i > index; --i) {                                               \
        source->Values[i] = source->Values[i - 1];                                              \
    }                                                                                           \
    source->Count += 1;                                                                         \
    source->Values[index] = item;                                                               \
}                                                                                               \
void List_##T##_ForEach(List(T) source, void (*action)(T*))                                     \
{                                                                                               \
    ThrowIfNull(source, action);                                                                \
    for (int i = 0; i < source->Count; ++i) {                                                   \
        action(&source->Values[i]);                                                             \
    }                                                                                           \
}                                                                                               \
void List_##T##_Sort(List(T) source, int (*comparer)(T, T))                                     \
{                                                                                               \
    ThrowIfNull(source);                                                                        \
    for (int gap = source->Count / 2; gap > 0; gap /= 2) {                                      \
        for (int i = gap; i < source->Count; ++i) {                                             \
            for (int j = i; j >= gap && comparer(                                               \
                source->Values[j - gap],source->Values[j]) > 0; j -= gap) {                     \
                T tmp = source->Values[j - gap];                                                \
                source->Values[j - gap] = source->Values[j];                                    \
                source->Values[j] = tmp;                                                        \
            }                                                                                   \
        }                                                                                       \
    }                                                                                           \
}

#endif