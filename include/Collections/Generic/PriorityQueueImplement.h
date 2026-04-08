#ifndef COLLECTIONS_GENERIC_PRIORITYQUEUE_IMPLEMENTATIONS
#define COLLECTIONS_GENERIC_PRIORITYQUEUE_IMPLEMENTATIONS

#include "Collections/Generic/PriorityQueueT.h"

#define PRIORITYQUEUE_IMPLEMENT(TPriority, TElement)                            \
typedef struct KeyValuePair_##TPriority##_##TElement##_s {                      \
    TPriority Key;                                                              \
    TElement Value;                                                             \
} KVP_##TPriority##_##TElement;                                                 \
static void heapify_##TPriority##_##TElement(PriorityQueue_##TPriority##_##TElement src)                                                            \
{                                                                               \
    for (int i = 0; i < src->Count; ++i) {                                      \
        for(int j = i, k = (j - 1) / 4;                                         \
            src->Comparer(src->_values[k].Key, src->_values[j].Key) > 0;        \
            j = k, k = (j - 1) / 4)                                             \
        {                                                                       \
            KVP_##TPriority##_##TElement tmp = src->_values[k];                 \
            src->_values[k] = src->_values[i];                                  \
            src->_values[i] = tmp;                                              \
        }                                                                       \
    }                                                                           \
}                                                                               \
static void reheapify_##TPriority##_##TElement(PriorityQueue_##TPriority##_##TElement src)                                                          \
{                                                                               \
    for (int i = 0, k; i < src->Count; i = 4*i + k) {                           \
        TPriority min = src->_values[i].Key;                                    \
        k = 0;                                                                  \
        for (int n = 1; n <= 4 && i * 4 + n < src->Count; ++n) {                \
            TPriority cur = src->_values[4*i + n].Key;                          \
            if (src->Comparer(min, cur) > 0) {                                  \
                min = cur;                                                      \
                k = n;                                                          \
            }                                                                   \
        }                                                                       \
        if (k != 0) {                                                           \
            KVP_##TPriority##_##TElement tmp = src->_values[4*i + k];           \
            src->_values[4*i + k] = src->_values[i];                            \
            src->_values[i] = tmp;                                              \
        } else return;                                                          \
    }                                                                           \
}                                                                               \
PriorityQueue_##TPriority##_##TElement new(PriorityQueue_##TPriority##_##TElement)(int capacity, int (*comparer)(TPriority, TPriority))             \
{                                                                               \
    auto allocinit(PriorityQueue_##TPriority##_##TElement, result) {            \
        .Count = 0,                                                             \
        .Capacity = capacity,                                                   \
        .Comparer = comparer,                                                   \
        ._values = alloc_array(KVP_##TPriority##_##TElement, capacity)          \
    };                                                                          \
    return result;                                                              \
}                                                                               \
void PriorityQueue_##TPriority##_##TElement##_Clear(PriorityQueue_##TPriority##_##TElement source)                                                  \
{                                                                               \
    source->Count = 0;                                                          \
}                                                                               \
TElement PriorityQueue_##TPriority##_##TElement##_Dequeue(PriorityQueue_##TPriority##_##TElement source)                                            \
{                                                                               \
    TElement result = source->_values[0].Value;                                 \
    source->Count -= 1;                                                         \
    source->_values[0] = source->_values[source->Count];                        \
    reheapify_##TPriority##_##TElement(source);                                 \
    return result;                                                              \
}                                                                               \
void PriorityQueue_##TPriority##_##TElement##_Enqueue(PriorityQueue_##TPriority##_##TElement source, TElement item, TPriority priority)             \
{                                                                               \
    if (source->Count == source->Capacity) {                                    \
        PriorityQueue_##TPriority##_##TElement##_EnsureCapacity(                \
            source, source->Capacity * 2);                                      \
    }                                                                           \
    source->_values[source->Count] = (KVP_##TPriority##_##TElement){priority,item};\
    source->Count += 1;                                                         \
    for(int i = source->Count - 1, k = (i - 1) / 4;                             \
        source->Comparer(source->_values[k].Key, source->_values[i].Key) > 0;   \
        i = k, k = (i - 1) / 4)                                                 \
    {                                                                           \
        KVP_##TPriority##_##TElement tmp = source->_values[k];                  \
        source->_values[k] = source->_values[i];                                \
        source->_values[i] = tmp;                                               \
    }                                                                           \
}                                                                               \
TElement PriorityQueue_##TPriority##_##TElement##_DequeueEnqueue(PriorityQueue_##TPriority##_##TElement source, TElement item, TPriority priority)  \
{                                                                               \
    TElement result = source->_values[0].Value;                                 \
    source->_values[0] = (KVP_##TPriority##_##TElement) {priority, item};       \
    heapify_##TPriority##_##TElement(source);                                   \
    return result;                                                              \
}                                                                               \
TElement PriorityQueue_##TPriority##_##TElement##_EnqueueDequeue(PriorityQueue_##TPriority##_##TElement source, TElement item, TPriority priority)  \
{                                                                               \
    if (source->Comparer(priority, source->_values[0].Key) < 0) return item;    \
    TElement result = source->_values[0].Value;                                 \
    source->_values[0] = (KVP_##TPriority##_##TElement) {priority, item};       \
    heapify_##TPriority##_##TElement(source);                                   \
    return result;                                                              \
}                                                                               \
TElement PriorityQueue_##TPriority##_##TElement##_Peek(PriorityQueue_##TPriority##_##TElement source)                                               \
{                                                                               \
    return source->_values[0].Value;                                            \
}                                                                               \
bool PriorityQueue_##TPriority##_##TElement##_TryDequeue(PriorityQueue_##TPriority##_##TElement source, TElement* out, TPriority* out_p)            \
{                                                                               \
    if (source->Count == 0) return false;                                       \
    if (out_p) *out_p = source->_values[0].Key;                                 \
    if (out) *out = PriorityQueue_##TPriority##_##TElement##_Dequeue(source);   \
    return true;                                                                \
}                                                                               \
bool PriorityQueue_##TPriority##_##TElement##_TryPeek(PriorityQueue_##TPriority##_##TElement source, TElement* out, TPriority* out_p)               \
{                                                                               \
    if (source->Count == 0) return false;                                       \
    if (out) *out = PriorityQueue_##TPriority##_##TElement##_Peek(source);      \
    if (out_p) *out_p = source->_values[0].Key;                                 \
    return true;                                                                \
}                                                                               \
void PriorityQueue_##TPriority##_##TElement##_TrimExcess(PriorityQueue_##TPriority##_##TElement source)                                             \
{                                                                               \
    if (source->Count < source->Capacity - source->Capacity / 10) {             \
        source->_values = realloc(source->_values,                              \
            sizeof(KVP_##TPriority##_##TElement) * source->Count);              \
        source->Capacity = source->Count;                                       \
    }                                                                           \
}                                                                               \
void PriorityQueue_##TPriority##_##TElement##_EnsureCapacity(PriorityQueue_##TPriority##_##TElement source, int capacity)                           \
{                                                                               \
    if (source->Capacity < capacity) {                                          \
        source->_values = realloc(source->_values,                              \
            sizeof(KVP_##TPriority##_##TElement) * capacity);                   \
        source->Capacity = capacity;                                            \
    }                                                                           \
}

#endif