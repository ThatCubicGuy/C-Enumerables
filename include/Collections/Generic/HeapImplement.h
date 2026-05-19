#ifndef COLLECTIONS_GENERIC_HEAP_IMPLEMENTATIONS
#define COLLECTIONS_GENERIC_HEAP_IMPLEMENTATIONS
#include "HeapT.h"
#define HEAP_IMPLEMENT(T)                                               \
Heap(T) new(Heap(T))(int capacity, int type, int (*comparer)(T, T))     \
{                                                                       \
    Heap(T) result = meminit(Heap(T)) {                                 \
        .Count = 0,                                                     \
        .Capacity = capacity,                                           \
        .Comparer = comparer,                                           \
        .Type = type,                                                   \
        ._items = arralloc(T, capacity)                                 \
    };                                                                  \
    return result;                                                      \
}                                                                       \
void Heap_##T##_Destroy(Heap(T) *source)                                \
{                                                                       \
    ThrowIfNull(source, *source);                                       \
    memfree((*source)->_items);                                         \
    memfree(*source);                                                   \
    *source = NULL;                                                     \
}                                                                       \
void Heap_##T##_Push(Heap(T) source, T item)                            \
{                                                                       \
    ThrowIfNull(source);                                                \
    if (source->Count >= source->Capacity) {                            \
        Heap_##T##_EnsureCapacity(source, source->Capacity * 2);        \
    }                                                                   \
    source->_items[source->Count] = item;                               \
    source->Count += 1;                                                 \
    for(int i = source->Count - 1, k = (i - 1) / source->Type;          \
        source->Comparer(source->_items[k], source->_items[i]) > 0;     \
        i = k, k = (i - 1) / source->Type)                              \
    {                                                                   \
        T tmp = source->_items[k];                                      \
        source->_items[k] = source->_items[i];                          \
        source->_items[i] = tmp;                                        \
    }                                                                   \
}                                                                       \
T Heap_##T##_Pop(Heap(T) source)                                        \
{                                                                       \
    ThrowIfNull(source);                                                \
    T result = source->_items[0];                                       \
    source->Count -= 1;                                                 \
    source->_items[0] = source->_items[source->Count];                  \
    for (int i = 0, k; i < source->Count; i = i * source->Type + k) {   \
        T min = source->_items[i];                                      \
        k = 0;                                                          \
        for (int n = 1; n <= source->Type && i * source->Type + n < source->Count; ++n) { \
            T cur = source->_items[i * source->Type + n];               \
            if (source->Comparer(min, cur) > 0) {                       \
                min = cur;                                              \
                k = n;                                                  \
            }                                                           \
        }                                                               \
        if (k != 0) {                                                   \
            T tmp = source->_items[i * source->Type + k];               \
            source->_items[i * source->Type + k] = source->_items[i];   \
            source->_items[i] = tmp;                                    \
        } else break;                                                   \
    }                                                                   \
    return result;                                                      \
}                                                                       \
T Heap_##T##_Peek(Heap(T) source)                                       \
{                                                                       \
    ThrowIfNull(source);                                                \
    return source->_items[0];                                           \
}                                                                       \
void Heap_##T##_Clear(Heap(T) source)                                   \
{                                                                       \
    ThrowIfNull(source);                                                \
    source->Count = 0;                                                  \
}                                                                       \
bool Heap_##T##_TryPop(Heap(T) source, T* out)                          \
{                                                                       \
    ThrowIfNull(source);                                                \
    if (source->Count == 0) return false;                               \
    if (out) *out = Heap_##T##_Pop(source);                             \
    return true;                                                        \
}                                                                       \
bool Heap_##T##_TryPeek(Heap(T) source, T* out)                         \
{                                                                       \
    ThrowIfNull(source);                                                \
    if (source->Count == 0) return false;                               \
    if (out) *out = Heap_##T##_Peek(source);                            \
    return true;                                                        \
}                                                                       \
void Heap_##T##_TrimExcess(Heap(T) source)                              \
{                                                                       \
    ThrowIfNull(source);                                                \
    if (source->Count > 0 &&                                            \
        source->Count < source->Capacity - source->Capacity / 10) {     \
        source->_items = memresize(source->_items, source->Count);      \
        source->Capacity = source->Count;                               \
    }                                                                   \
}                                                                       \
void Heap_##T##_EnsureCapacity(Heap(T) source, int capacity)            \
{                                                                       \
    ThrowIfNull(source);                                                \
    if (source->Capacity < capacity) {                                  \
        source->_items = memresize(source->_items, capacity);           \
        source->Capacity = capacity;                                    \
    }                                                                   \
}

#endif