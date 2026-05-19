#include "Keywords.h"
#include "System.h"
#if ALLOC_TYPE == HEAP_ALLOC

// God I need a garbage collector...
#define HEAPTRACE_SIZE 16384ul

static TAG(HeapTrace) {
    size_t alloc_count;
    void* allocs[HEAPTRACE_SIZE];
} HeapTrace = {
    .alloc_count = 0,
    .allocs = {0},
};

void* memalloc_(size_t size)
{
    DEBUG_WRITELINE("Called memalloc with size %zu. Current heap allocations: %zu", size, HeapTrace.alloc_count);
    // Special functionality of memalloc_ - return last allocation if size is 0
    if (size == 0) return HeapTrace.alloc_count > 0 ? HeapTrace.allocs[HeapTrace.alloc_count - 1] : throwe(new(Exception)("No last allocation to get!"));
    ENVIRONMENT_FAILFAST_IF_NOT(HeapTrace.alloc_count < HEAPTRACE_SIZE - 1, "CATASTROPHIC FAILURE: HeapTrace overflow\nHeap index: %zu\n", HeapTrace.alloc_count);
    HeapTrace.allocs[HeapTrace.alloc_count] = malloc(size);
    for (size_t i = 0; i < HeapTrace.alloc_count; ++i) {
        ENVIRONMENT_FAILFAST_IF(HeapTrace.allocs[i] == HeapTrace.allocs[HeapTrace.alloc_count],
            "ERROR: Array box %zu allocated twice (new size: %zu)", i, size);
    }
    return HeapTrace.allocs[HeapTrace.alloc_count] ? HeapTrace.allocs[HeapTrace.alloc_count++] : throwe(new(OutOfMemoryException)(size));
}

void* zeroalloc_(size_t size)
{
    DEBUG_WRITELINE("Called zeroalloc with size %zu. Current heap allocations: %zu", size, HeapTrace.alloc_count);
    if (size == 0) throw new(Exception)("Cannot allocate block of size 0! (zeroalloc)");
    ENVIRONMENT_FAILFAST_IF_NOT(HeapTrace.alloc_count < HEAPTRACE_SIZE - 1, "CATASTROPHIC FAILURE: HeapTrace overflow\nHeap index: %zu\n", HeapTrace.alloc_count);
    HeapTrace.allocs[HeapTrace.alloc_count] = calloc(1, size);
    return HeapTrace.allocs[HeapTrace.alloc_count] ? HeapTrace.allocs[HeapTrace.alloc_count++] : throwe(new(OutOfMemoryException)(size));
}

void* memresize_(void* obj, size_t new_size)
{
    DEBUG_WRITELINE("Called memresize with new size %zu. Current heap allocations: %zu", new_size, HeapTrace.alloc_count);
    if (new_size == 0) throw new(Exception)("Cannot allocate block of size 0! (memresize)");
    ENVIRONMENT_FAILFAST_IF_NOT(HeapTrace.alloc_count > 0 && HeapTrace.alloc_count < HEAPTRACE_SIZE, "CATASTROPHIC FAILURE: HeapTrace overflow\nHeap index: %zu\n", HeapTrace.alloc_count);
    for (size_t i = 0; i < HeapTrace.alloc_count; ++i) {
        if (HeapTrace.allocs[i] == obj) {
            void* last_alloc = realloc(obj, new_size);
            return last_alloc ? HeapTrace.allocs[i] = last_alloc : throwe(new(OutOfMemoryException)(new_size));
        }
    }
    ENVIRONMENT_FAILFAST("Reallocation of unallocated memory!");
    return NULL;
}

void memfree_(void* obj)
{
    DEBUG_WRITELINE("Called memfree for location %p. Current heap allocations: %zu", obj, HeapTrace.alloc_count);
    if (obj == NULL) return;
    for (size_t i = 0; i < HeapTrace.alloc_count; ++i) {
        if (HeapTrace.allocs[i] == obj) {
            HeapTrace.alloc_count -= 1;
            free(HeapTrace.allocs[i]);
            for (size_t j = i; j < HeapTrace.alloc_count; ++j) {
                HeapTrace.allocs[j] = HeapTrace.allocs[j + 1];
            }
            HeapTrace.allocs[HeapTrace.alloc_count] = NULL;
            return;
        }
    }
    throw new(Exception)("Cannot free unallocated memory!");
}

void memcopy_(void* dest, const void* source, size_t size)
{
    DEBUG_WRITELINE("Called memcopy from %p to %p with size %zu. Current heap allocations: %zu", source, dest, size, HeapTrace.alloc_count);
    for (size_t i = 0; i < size; ++i) {
        ((byte*)dest)[i] = ((byte*)source)[i];
    }
}

// Frees every single allocation of the program.
void MEMDESTROY(void)
{
    for (size_t i = 0; i < HeapTrace.alloc_count; ++i) {
        free(HeapTrace.allocs[i]);
        HeapTrace.allocs[i] = NULL;
    }
    HeapTrace.alloc_count = 0;
}

// Finalize program after execution
int main(int argc, char** argv)
{
    int ret_code = Main(argc, argv);
    DEBUG_WRITELINE("Freeing %zu allocations leftover from program execution.", HeapTrace.alloc_count);
    MEMDESTROY();
    return ret_code;
}

#endif
