#ifndef COLLECTIONS_GENERIC_HEAP
#define COLLECTIONS_GENERIC_HEAP

#define HEAP_DEFINE(T)                              \
typedef struct Heap_##T##_s {                       \
    int Count, Capacity;                            \
    int (*Comparer)(T, T);                          \
    int Type;                                       \
    T* _values;                                     \
} *Heap_##T;                                        \
Heap_##T new(Heap_##T)(int capacity, int type, int (*comparer)(T, T));  \
void Heap_##T##_Push(Heap_##T source, T item);      \
T Heap_##T##_Pop(Heap_##T source);                  \
T Heap_##T##_Peek(Heap_##T source);                 \
void Heap_##T##_Clear(Heap_##T source);             \
bool Heap_##T##_TryPop(Heap_##T source, T* out);    \
bool Heap_##T##_TryPeek(Heap_##T source, T* out);   \
void Heap_##T##_TrimExcess(Heap_##T source);        \
void Heap_##T##_EnsureCapacity(Heap_##T source, int capacity);

#endif