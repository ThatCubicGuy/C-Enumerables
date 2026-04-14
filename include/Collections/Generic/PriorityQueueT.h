#ifndef COLLECTIONS_GENERIC_PRIORITYQUEUE
#define COLLECTIONS_GENERIC_PRIORITYQUEUE

#define PriorityQueue(TPriority, TElement) _PriorityQueue_##TPriority##_##TElement

#define PRIORITYQUEUE_DEFINE(TPriority, TElement)           \
typedef struct KeyValuePair_##TPriority##_##TElement##_s KVP_##TPriority##_##TElement; \
typedef struct _PriorityQueue_##TPriority##_##TElement##_s {\
    KVP_##TPriority##_##TElement* _values;  \
    int Count, Capacity;                    \
    int (*Comparer)(TPriority, TPriority);  \
} *PriorityQueue(TPriority,TElement);       \
PriorityQueue(TPriority,TElement) new(PriorityQueue(TPriority,TElement))(int capacity, int (*comparer)(TPriority, TPriority));\
/**                                                                                                                                                 \
 * @brief Removes and returns the item with the lowest                                                                                              \
 * priority inside the PriorityQueue<TElement, TPriority>.                                                                                          \
 * @param source Queue to remove an item from.                                                                                                      \
 * @return Element with the lowest priority from the queue.                                                                                         \
 */                                                                                                                                                 \
TElement PriorityQueue_##TPriority##_##TElement##_Dequeue(PriorityQueue(TPriority,TElement) source);                                           \
/**                                                                                                                                                 \
 * @brief Adds an item to the Queue with an associated priority.                                                                                    \
 * @param source Queue to add an item to.                                                                                                           \
 * @param item Item to add to the queue.                                                                                                            \
 * @param priority Priority associated with the item to add.                                                                                        \
 */                                                                                                                                                 \
void PriorityQueue_##TPriority##_##TElement##_Enqueue(PriorityQueue(TPriority,TElement) source, TElement item, TPriority priority);            \
/**                                                                                                                                                 \
 * @brief Removes the item with the lowest priority then                                                                                            \
 * immediately adds another item with an associated priority.                                                                                       \
 * @param source Queue to dequeue+enqueue.                                                                                                          \
 * @param item Item to add to the queue.                                                                                                            \
 * @param priority Priority associated with the item to add.                                                                                        \
 * @return Element with the lowest priority from the queue.                                                                                         \
 */                                                                                                                                                 \
TElement PriorityQueue_##TPriority##_##TElement##_DequeueEnqueue(PriorityQueue(TPriority,TElement) source, TElement item, TPriority priority); \
/**                                                                                                                                                 \
 * @brief Adds an item with an associated priority then immediately                                                                                 \
 * removes the item with the lowest priority from the queue.                                                                                        \
 * @param source Queue to enqueue+dequeue.                                                                                                          \ 
 * @param item Item to add to the queue.                                                                                                            \
 * @param priority Priority associated with the item to add.                                                                                        \
 * @return Element with the lowest priority from the queue.                                                                                         \
 */                                                                                                                                                 \
TElement PriorityQueue_##TPriority##_##TElement##_EnqueueDequeue(PriorityQueue(TPriority,TElement) source, TElement item, TPriority priority); \
/**                                                                                                                                                 \
 * @brief Returns the item with the lowest priority inside                                                                                          \
 * the PriorityQueue<TElement, TPriority> without removing it.                                                                                      \
 * @param source Queue to peek.                                                                                                                     \
 * @return Element with the lowest priority from the queue.                                                                                         \
 */                                                                                                                                                 \
TElement PriorityQueue_##TPriority##_##TElement##_Peek(PriorityQueue(TPriority,TElement) source);                                              \
/**                                                                                                                                                 \
 * @brief Tries getting and removing the item with the lowest                                                                                       \
 * priority inside the PriorityQueue<TElement, TPriority>, then                                                                                     \
 * places the result in `out` if the operation succeeds.                                                                                            \
 * @param source Queue to try dequeueing.                                                                                                           \
 * @param out Result of the dequeue, if successful.                                                                                                 \
 * @param out_p Priority of the first item, if successful.                                                                                          \
 * @return True if the operation succeeded, false otherwise.                                                                                        \
 */                                                                                                                                                 \
bool PriorityQueue_##TPriority##_##TElement##_TryDequeue(PriorityQueue(TPriority,TElement) source, TElement* out, TPriority* out_p);           \
/**                                                                                                                                                 \
 * @brief Tries getting the item with the lowest priority inside                                                                                    \
 * the PriorityQueue<TElement, TPriority> without removing it,                                                                                      \
 * then places the result in `out` if the operation succeeds.                                                                                       \
 * @param source Queue to try peeking.                                                                                                              \
 * @param out Result of the peek, if successful.                                                                                                    \
 * @param out_p Priority of the first item, if successful.                                                                                          \
 * @return True if the operation succeeded, false otherwise.                                                                                        \
 */                                                                                                                                                 \
bool PriorityQueue_##TPriority##_##TElement##_TryPeek(PriorityQueue(TPriority,TElement) source, TElement* out, TPriority* out_p);              \
/**                                                                                                                                                 \
 * @brief Sets the capacity of the queue to the amount of elements,                                                                                 \
 * if that amount is less than 90% of current capacity.                                                                                             \
 * @param source Queue to trim the excess of.                                                                                                       \
 */                                                                                                                                                 \
void PriorityQueue_##TPriority##_##TElement##_TrimExcess(PriorityQueue(TPriority,TElement) source);                                            \
/**                                                                                                                                                 \
 * @brief Ensures the given queue has the capacity                                                                                                  \
 * to hold a given amount of items.                                                                                                                 \
 * @param source Queue to ensure the capacity for.                                                                                                  \
 * @param capacity Capacity to ensure.                                                                                                              \
 */                                                                                                                                                 \
void PriorityQueue_##TPriority##_##TElement##_EnsureCapacity(PriorityQueue(TPriority,TElement) source, int capacity);

#endif