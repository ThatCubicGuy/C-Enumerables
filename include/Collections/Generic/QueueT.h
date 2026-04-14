#ifndef COLLECTIONS_GENERIC_QUEUE
#define COLLECTIONS_GENERIC_QUEUE

#define Queue(T) _Queue_##T

#define QUEUE_DEFINE(T)                                     \
typedef struct _Queue_##T##_s {                             \
    struct _IEnumerable_##T##_s _parent;                    \
    int _start, _end;                                       \
    T* _values;                                             \
    int Capacity, Count;                                    \
} *Queue(T);                                                \
Queue(T) new(Queue(T))(int capacity);                       \
void Queue_##T##_Destroy(Queue(T)* source);                 \
void Queue_##T##_Clear(Queue(T) source);                    \
void Queue_##T##_Enqueue(Queue(T) source, T item);          \
void Queue_##T##_EnqueueFirst(Queue(T) source, T item);     \
T Queue_##T##_Peek(Queue(T) source);                        \
T Queue_##T##_Dequeue(Queue(T) source);                     \
bool Queue_##T##_TryPeek(Queue(T) source, T* out);          \
bool Queue_##T##_TryDequeue(Queue(T) source, T* out);       \
void Queue_##T##_EnsureCapacity(Queue(T) source, int capacity);

#endif