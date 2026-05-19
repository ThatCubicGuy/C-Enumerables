#include "Tests.h"
#include "Collections/Generic/HeapT.h"
#include "Collections/Generic/EnumerableT.h"

ENUMERABLE_DEFINE(int)
HEAP_DEFINE(int)

#define TRYPEEK(HEAP, OUT) do { if (Heap_int_TryPeek(HEAP, &OUT)) { fprintf(output, "Peek "#HEAP": %d\n", OUT); } else fprintf(output, "Peek failed! Count: %d\n", HEAP->Count); } while (0)
#define TRYPOP(HEAP, OUT) do { if (Heap_int_TryPop(HEAP, &OUT)) { fprintf(output, "Popping "#HEAP": %d\n", OUT); } else fprintf(output, "Pop failed! Count: %d\n", HEAP->Count); } while (0)

static int ascending(int left, int right)
{
    if (left < right) return -1;
    if (left > right) return 1;
    return 0;
}

static int descending(int left, int right)
{
    if (left > right) return -1;
    if (left < right) return 1;
    return 0;
}

void test_heaps(FILE* output)
{
    var heap = new(Heap(int))(16, 2, descending);
    Heap_int_Push(heap, 7);
    Heap_int_Push(heap, 4);
    Heap_int_Push(heap, 9);
    Heap_int_Push(heap, 2);
    Heap_int_Push(heap, 5);
    Heap_int_Push(heap, 1);
    fprintf(output, "Pop heap while there's still items:\n");
    int result;
    while (Heap_int_TryPop(heap, &result)) {
        fprintf(output, "Item: %d\n", result);
    }
    fprintf(output, "Clear heap and then add some more numbers:\n");
    Heap_int_Push(heap, 17);
    Heap_int_Push(heap, 54);
    Heap_int_Push(heap, 29);
    Heap_int_Push(heap, 82);
    Heap_int_Push(heap, 35);
    Heap_int_Push(heap, 91);
    while (Heap_int_TryPop(heap, &result)) {
        fprintf(output, "Item: %d\n", result);
    }
    var quat = new(Heap(int))(100, 4, ascending);
    int src[20] = { 4, 2, 0, 12, 654, 33, 66, 23, 15, 99, 85, 40, 17, 192, 193, 49, 48, 37, 2147483647, 73 };
    int cnt = 20;
    for (int i = 0; i < cnt; ++i) Heap_int_Push(quat, src[i]);
    Heap_int_TrimExcess(quat);
    fprintf(output, "\nNow for some more fun. Quaternary minheap (capacity %d):\n", quat->Capacity);
    while (Heap_int_TryPop(quat, &result)) {
        fprintf(output, "Item: %d\n", result);
    }
}

#include "Collections/Generic/HeapImplement.h"
HEAP_IMPLEMENT(int)