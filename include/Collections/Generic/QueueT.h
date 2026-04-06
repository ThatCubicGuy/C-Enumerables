#ifndef COLLECTIONS_GENERIC_QUEUE
#define COLLECTIONS_GENERIC_QUEUE

#define QUEUE_DEFINE(T)                                     \
typedef struct Queue_##T##_s {                              \
    struct IEnumerable_##T##_s _parent;                     \
    int _start, _end;                                       \
    T* _values;                                             \
    int Capacity, Count;                                    \
} *Queue_##T;                                               \
Queue_##T Queue_##T##__ctor(int capacity);                  \
void Queue_##T##_Destroy(Queue_##T* source);                \
void Queue_##T##_Clear(Queue_##T source);                   \
void Queue_##T##_Enqueue(Queue_##T source, T item);         \
void Queue_##T##_EnqueueFirst(Queue_##T source, T item);    \
T Queue_##T##_Peek(Queue_##T source);                       \
T Queue_##T##_Dequeue(Queue_##T source);                    \
bool Queue_##T##_TryPeek(Queue_##T source, T* out);         \
bool Queue_##T##_TryDequeue(Queue_##T source, T* out);      \
void Queue_##T##_EnsureCapacity(Queue_##T source, int capacity);

#endif