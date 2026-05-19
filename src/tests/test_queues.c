#include "Tests.h"
#include "Keywords.h"
#include "Collections/Generic/QueueT.h"
#include "Collections/Generic/QueueImplement.h"
#include "Collections/Generic/EnumerableT.h"
#include <stdio.h>

ENUMERABLE_DEFINE(int)
QUEUE_DEFINE(int)

#define TRYPEEK(QUEUE, OUT) do { if (Queue_int_TryPeek(QUEUE, &OUT)) { fprintf(output, "Peek "#QUEUE": %d\n", OUT); } else fprintf(output, "Peek failed! Count: %d\n", QUEUE->Count); } while (0)
#define TRYDEQUEUE(QUEUE, OUT) do { if (Queue_int_TryDequeue(QUEUE, &OUT)) { fprintf(output, "Dequeue "#QUEUE": %d\n", OUT); } else fprintf(output, "Dequeue failed! Count: %d\n", QUEUE->Count); } while (0)

void test_queues(FILE* output)
{
    int result;
    Queue(int) queue = new(Queue(int))(2);
    Queue_int_Enqueue(queue, 4);
    Queue_int_Enqueue(queue, 5);
    Queue_int_Enqueue(queue, 1);
    Queue_int_Enqueue(queue, 2);
    Queue_int_Enqueue(queue, 6);
    Queue_int_Enqueue(queue, 7);
    foreach (int item in queue) {
        fprintf(output, "Item: %d\n", item);
    }
    TRYDEQUEUE(queue, result);
    foreach (int item in queue) {
        fprintf(output, "Item: %d\n", item);
    }
    TRYPEEK(queue, result);
    foreach (int item in queue) {
        fprintf(output, "Item: %d\n", item);
    }
    fprintf(output, "Clear queue...\n");
    Queue_int_Clear(queue);
    foreach (int item in queue) {
        fprintf(output, "Item: %d\n", item);
    }
    TRYDEQUEUE(queue, result);
    TRYPEEK(queue, result);
    fprintf(output, "Add 57...\n");
    Queue_int_Enqueue(queue, 57);
    TRYPEEK(queue, result);
    TRYDEQUEUE(queue, result);
}

QUEUE_IMPLEMENT(int)