#include "Keywords.h"
#include "System.h"

void* last_alloc = NULL;

void* memalloc_(size_t size)
{
    // Special functionality of memalloc_ - return last allocation if size is 0
    if (size == 0) return last_alloc;
    last_alloc = malloc(size);
    return last_alloc ? last_alloc : throwe(new(OutOfMemoryException)("Not enough memory to allocate block of %d bytes", size));
}

void* zeroalloc_(size_t size)
{
    if (size == 0) throw new(Exception)("Cannot allocate block of size 0! (zeroalloc)");
    last_alloc = calloc(1, size);
    return last_alloc ? last_alloc : throwe(new(OutOfMemoryException)("Not enough memory to allocate block of %d bytes", size));
}

void* memresize_(void* object, size_t new_size)
{
    if (new_size == 0) throw new(Exception)("Cannot allocate block of size 0! (memresize)");
    last_alloc = realloc(object, new_size);
    return last_alloc ? last_alloc : throwe(new(OutOfMemoryException)("Not enough memory to allocate block of %d bytes", new_size));
}

void memfree_(void* object)
{
    free(object);
}

void memcopy_(void* dest, const void* source, size_t size)
{
    for (size_t i = 0; i < size; ++i) {
        ((byte*)dest)[i] = ((byte*)source)[i];
    }
}
