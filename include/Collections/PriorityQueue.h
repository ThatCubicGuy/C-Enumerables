#ifndef COLLECTIONS_PRIORITYQUEUE
#define COLLECTIONS_PRIORITYQUEUE

#include "Defines.h"
#include "Enumerable.h"

typedef TAG(PriorityQueue) {
    IMPL(IEnumerable);
    // Amount of items in the priority queue.
    int Count;
} PriorityQueue;

void Enqueue(PriorityQueue source, int priority);

object Dequeue(PriorityQueue source);

bool TryDequeue(PriorityQueue source, object* out);

object Peek(PriorityQueue source);

bool TryPeek(PriorityQueue source, object* out);



#endif