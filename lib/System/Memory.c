#include "Keywords.h"
#include "System.h"

TAG(HeapTrace) {
    size_t alloc_count;
    void* allocs[256];
} HeapTrace = {
    .alloc_count = 0,
    .allocs = {0}
};

void* memalloc_(size_t size)
{
    // Special functionality of memalloc_ - return last allocation if size is 0
    if (size == 0) return HeapTrace.alloc_count > 0 ? HeapTrace.allocs[HeapTrace.alloc_count - 1] : throwe(new(Exception)("No last allocation to get!"));
    HeapTrace.allocs[HeapTrace.alloc_count] = malloc(size);
    return HeapTrace.allocs[HeapTrace.alloc_count] ? HeapTrace.allocs[HeapTrace.alloc_count++] : throwe(new(OutOfMemoryException)("Not enough memory to allocate block of %d bytes", size));
}

void* zeroalloc_(size_t size)
{
    if (size == 0) throw new(Exception)("Cannot allocate block of size 0! (zeroalloc)");
    HeapTrace.allocs[HeapTrace.alloc_count] = calloc(1, size);
    return HeapTrace.allocs[HeapTrace.alloc_count] ? HeapTrace.allocs[HeapTrace.alloc_count++] : throwe(new(OutOfMemoryException)("Not enough memory to allocate block of %d bytes", size));
}

void* memresize_(void* object, size_t new_size)
{
    if (new_size == 0) throw new(Exception)("Cannot allocate block of size 0! (memresize)");
    void* last_alloc = realloc(object, new_size);
    return last_alloc ? HeapTrace.allocs[HeapTrace.alloc_count++] = last_alloc : throwe(new(OutOfMemoryException)("Not enough memory to allocate block of %d bytes", new_size));
}

void memfree_(void* object)
{
    free(object);
    return;
    for (size_t i = 0; i < HeapTrace.alloc_count; ++i) {
        if (HeapTrace.allocs[i] == object) {
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
    for (size_t i = 0; i < size; ++i) {
        ((byte*)dest)[i] = ((byte*)source)[i];
    }
}

// Frees every single allocation of the program.
void MEMDESTROY()
{
    for (size_t i = 0; i < HeapTrace.alloc_count; ++i) {
        free(HeapTrace.allocs[i]);
        HeapTrace.allocs[i] = NULL;
    }
    HeapTrace.alloc_count = 0;
}
