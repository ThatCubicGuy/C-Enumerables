#ifndef COLLECTIONS_GENERIC_HEAP
#define COLLECTIONS_GENERIC_HEAP
#include "Keywords.h"
#define Heap(T) CAT(Heap_,T)

#define HEAP_DEFINE(T)                              \
typedef TAG(Heap(T)) {                              \
    int Count, Capacity;                            \
    int (*Comparer)(T, T);                          \
    int Type;                                       \
    T* _items;                                      \
} *Heap(T);                                         \
Heap(T) new(Heap(T))(int capacity, int type, int (*comparer)(T, T));\
void Heap_##T##_Push(Heap(T) source, T item);       \
T Heap_##T##_Pop(Heap(T) source);                   \
T Heap_##T##_Peek(Heap(T) source);                  \
void Heap_##T##_Clear(Heap(T) source);              \
bool Heap_##T##_TryPop(Heap(T) source, T* out);     \
bool Heap_##T##_TryPeek(Heap(T) source, T* out);    \
void Heap_##T##_TrimExcess(Heap(T) source);         \
void Heap_##T##_EnsureCapacity(Heap(T) source, int capacity);

#endif