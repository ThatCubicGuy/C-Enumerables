#ifndef COLLECTIONS_GENERIC_QUEUE_IMPLEMENTATIONS
#define COLLECTIONS_GENERIC_QUEUE_IMPLEMENTATIONS

#include "Collections/Generic/QueueT.h"

#define QUEUE_IMPLEMENT(T)                                                  \
typedef struct QueueEnumerator_##T##_s {                                    \
    struct IEnumerator_##T##_s _parent;                                     \
    Queue_##T _source;                                                      \
    int _currentIndex;                                                      \
} *QueueEnumerator_##T;                                                     \
static bool QueueMoveNext_##T(IEnumerator_##T This)                         \
{                                                                           \
    QueueEnumerator_##T e = (QueueEnumerator_##T)This;                      \
    if (e->_currentIndex > e->_source->_end) return false;                  \
    if (e->_source->Count == 0) return false;                               \
    This->Current = e->_source->                                            \
        _values[e->_currentIndex % e->_source->Capacity];                   \
    e->_currentIndex += 1;                                                  \
    return true;                                                            \
}                                                                           \
static void QueueReset_##T(IEnumerator_##T This)                            \
{                                                                           \
    QueueEnumerator_##T e = ((QueueEnumerator_##T)This);                    \
    e->_currentIndex = e->_source->_start;                                  \
    This->Current = default(T);                                             \
}                                                                           \
static void QueueDispose_##T(IEnumerator_##T This)                          \
{                                                                           \
    free(This);                                                             \
}                                                                           \
static IEnumerator_##T QueueGetEnumerator_##T(const IEnumerable_##T This)   \
{                                                                           \
    QueueEnumerator_##T allocinit(QueueEnumerator_##T, result) {            \
        ._parent = (struct IEnumerator_##T##_s) {                           \
            .MoveNext = QueueMoveNext_##T,                                  \
            .Reset = QueueReset_##T,                                        \
            .Dispose = QueueDispose_##T                                     \
        },                                                                  \
        ._currentIndex = ((Queue_##T)This)->_start,                         \
        ._source = (Queue_##T)This                                          \
    };                                                                      \
    return base(result);                                                    \
}                                                                           \
Queue_##T Queue_##T##__ctor(int capacity)                           \
{                                                                   \
    Queue_##T allocinit(Queue_##T, result) {                        \
        ._parent = (struct IEnumerable_##T##_s) {                   \
            .GetEnumerator = QueueGetEnumerator_##T                 \
        },                                                          \
        .Count = 0,                                                 \
        .Capacity = capacity,                                       \
        ._start = 0,                                                \
        ._end = -1,                                                 \
        ._values = alloc_array(T, capacity)                         \
    };                                                              \
    return result;                                                  \
}                                                                   \
void Queue_##T##_Destroy(Queue_##T* source)                         \
{                                                                   \
    free((*source)->_values);                                       \
    free(*source);                                                  \
    *source = NULL;                                                 \
}                                                                   \
void Queue_##T##_EnsureCapacity(Queue_##T source, int capacity)     \
{                                                                   \
    if (source->Capacity < capacity) {                              \
        source->_values = realloc(source->_values,                  \
            sizeof(T) * capacity);                                  \
        source->Capacity = capacity;                                \
    }                                                               \
}                                                                   \
void Queue_##T##_Clear(Queue_##T source)                            \
{                                                                   \
    source->_start = 0;                                             \
    source->_end = -1;                                              \
    source->Count = 0;                                              \
}                                                                   \
void Queue_##T##_Enqueue(Queue_##T source, T item)                  \
{                                                                   \
    if (source->Count == source->Capacity) {                        \
        Queue_##T##_EnsureCapacity(source, source->Capacity * 2);   \
    }                                                               \
    source->Count += 1;                                             \
    source->_end += 1;                                              \
    source->_values[source->_end % source->Capacity] = item;        \
}                                                                   \
void Queue_##T##_EnqueueFirst(Queue_##T source, T item)	            \
{                                                                   \
    if (source->Count == source->Capacity) {                        \
        Queue_##T##_EnsureCapacity(source, source->Capacity * 2);   \
    }                                                               \
    source->Count += 1;                                             \
    source->_start -= 1;                                            \
    if (source->_start < 0) {                                       \
        source->_start += source->Capacity;                         \
        source->_end += source->Capacity;                           \
    }                                                               \
    source->_values[source->_start % source->Capacity] = item;      \
}                                                                   \
T Queue_##T##_Peek(Queue_##T source)                                \
{                                                                   \
    return source->_values[source->_start % source->Capacity];      \
}                                                                   \
T Queue_##T##_Dequeue(Queue_##T source)                             \
{                                                                   \
    T item = source->_values[source->_start % source->Capacity];    \
    source->_start += 1;                                            \
    source->Count -= 1;                                             \
    return item;                                                    \
}                                                                   \
T Queue_##T##_DequeueLast(Queue_##T source)                         \
{                                                                   \
    T item = source->_values[source->_end % source->Capacity];      \
    source->Count -= 1;                                             \
    source->_end -= 1;                                              \
    return item;                                                    \
}                                                                   \
bool Queue_##T##_TryPeek(Queue_##T source, T* out)                  \
{                                                                   \
    if (source->Count == 0) return false;                           \
    *out = Queue_##T##_Peek(source);                                \
    return true;                                                    \
}                                                                   \
bool Queue_##T##_TryDequeue(Queue_##T source, T* out)               \
{                                                                   \
    if (source->Count == 0) return false;                           \
    *out = Queue_##T##_Dequeue(source);                             \
    return true;                                                    \
}

#endif