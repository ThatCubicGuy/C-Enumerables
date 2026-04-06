#include "Tests.h"
#include "Defines.h"
#include "Collections/Generic/QueueT.h"
#include "Collections/Generic/QueueImplement.h"
#include "Collections/Generic/EnumerableT.h"

ENUMERABLE_DEFINE(int)
QUEUE_DEFINE(int)

#define TRYPEEK(QUEUE, OUT) do { if (Queue_int_TryPeek(QUEUE, &OUT)) { printf("Peek "#QUEUE": %d\n", OUT); } else printf("Peek failed! Count: %d\n", QUEUE->Count); } while (0)
#define TRYDEQUEUE(QUEUE, OUT) do { if (Queue_int_TryDequeue(QUEUE, &OUT)) { printf("Dequeue "#QUEUE": %d\n", OUT); } else printf("Dequeue failed! Count: %d\n", QUEUE->Count); } while (0)

void test_queues(void)
{
    int result;
    Queue_int queue = new(Queue_int)(2);
    Queue_int_Enqueue(queue, 4);
    Queue_int_Enqueue(queue, 5);
    Queue_int_Enqueue(queue, 1);
    Queue_int_Enqueue(queue, 2);
    Queue_int_Enqueue(queue, 6);
    Queue_int_Enqueue(queue, 7);
    foreach (int, item, queue, {
        printf("Item: %d\n", item);
    });
    TRYDEQUEUE(queue, result);
    foreach (int, item, queue, {
        printf("Item: %d\n", item);
    });
    TRYPEEK(queue, result);
    foreach (int, item, queue, {
        printf("Item: %d\n", item);
    });
    printf("Clear queue...\n");
    Queue_int_Clear(queue);
    foreach (int, item, queue, {
        printf("Item: %d\n", item);
    });
    TRYDEQUEUE(queue, result);
    TRYPEEK(queue, result);
    printf("Add 57...\n");
    Queue_int_Enqueue(queue, 57);
    TRYPEEK(queue, result);
    TRYDEQUEUE(queue, result);
}

QUEUE_IMPLEMENT(int)