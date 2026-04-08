#ifndef COLLECTIONS_GENERIC_HEAP_IMPLEMENTATIONS
#define COLLECTIONS_GENERIC_HEAP_IMPLEMENTATIONS

#include "Collections/Generic/HeapT.h"

#define HEAP_IMPLEMENT(T)                                               \
Heap_##T new(Heap_##T)(int capacity, int type, int (*comparer)(T, T))   \
{                                                                       \
    auto allocinit(Heap_##T, result) {                                  \
        .Count = 0,                                                     \
        .Capacity = capacity,                                           \
        .Comparer = comparer,                                           \
        .Type = type,                                                   \
        ._values = alloc_array(T, capacity)                             \
    };                                                                  \
    return result;                                                      \
}                                                                       \
void Heap_##T##_Push(Heap_##T source, T item)                           \
{                                                                       \
    if (source->Count == source->Capacity) {                            \
        Heap_##T##_EnsureCapacity(source, source->Capacity * 2);        \
    }                                                                   \
    source->_values[source->Count] = item;                              \
    source->Count += 1;                                                 \
    for(int i = source->Count - 1, k = (i - 1) / source->Type;          \
        source->Comparer(source->_values[k], source->_values[i]) > 0;   \
        i = k, k = (i - 1) / source->Type)                              \
    {                                                                   \
        T tmp = source->_values[k];                                     \
        source->_values[k] = source->_values[i];                        \
        source->_values[i] = tmp;                                       \
    }                                                                   \
}                                                                       \
T Heap_##T##_Pop(Heap_##T source)                                       \
{                                                                       \
    T result = source->_values[0];                                      \
    source->Count -= 1;                                                 \
    source->_values[0] = source->_values[source->Count];                \
    for (int i = 0, k; i < source->Count; i = i * source->Type + k) {   \
        T min = source->_values[i];                                     \
        k = 0;                                                          \
        for (int n = 1; n <= source->Type && i * source->Type + n < source->Count; ++n) {  \
            T cur = source->_values[i * source->Type + n];              \
            if (source->Comparer(min, cur) > 0) {                       \
                min = cur;                                              \
                k = n;                                                  \
            }                                                           \
        }                                                               \
        if (k != 0) {                                                   \
            T tmp = source->_values[i * source->Type + k];              \
            source->_values[i * source->Type + k] = source->_values[i]; \
            source->_values[i] = tmp;                                   \
        } else break;                                                   \
    }                                                                   \
    return result;                                                      \
}                                                                       \
T Heap_##T##_Peek(Heap_##T source)                                      \
{                                                                       \
    return source->_values[0];                                          \
}                                                                       \
void Heap_##T##_Clear(Heap_##T source)                                  \
{                                                                       \
    source->Count = 0;                                                  \
}                                                                       \
bool Heap_##T##_TryPop(Heap_##T source, T* out)                         \
{                                                                       \
    if (source->Count == 0) return false;                               \
    if (out) *out = Heap_##T##_Pop(source);                             \
    return true;                                                        \
}                                                                       \
bool Heap_##T##_TryPeek(Heap_##T source, T* out)                        \
{                                                                       \
    if (source->Count == 0) return false;                               \
    if (out) *out = Heap_##T##_Peek(source);                            \
    return true;                                                        \
}                                                                       \
void Heap_##T##_TrimExcess(Heap_##T source)                             \
{                                                                       \
    if (source->Count < source->Capacity - source->Capacity / 10) {     \
        source->_values = realloc(source->_values,                      \
            sizeof(T) * source->Count);                                 \
        source->Capacity = source->Count;                               \
    }                                                                   \
}                                                                       \
void Heap_##T##_EnsureCapacity(Heap_##T source, int capacity)           \
{                                                                       \
    if (source->Capacity < capacity) {                                  \
        source->_values = realloc(source->_values,                      \
            sizeof(T) * capacity);                                      \
        source->Capacity = capacity;                                    \
    }                                                                   \
}

#endif