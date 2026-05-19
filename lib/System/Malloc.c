#include "Keywords.h"
#include "System.h"
#if ALLOC_TYPE == LIBC_MALLOC

void* last_alloc;

void* memalloc_(size_t block_size)
{
    DEBUG_WRITELINE("Called memalloc with size %zu B", block_size);
    if (block_size == 0) return last_alloc;
    last_alloc = malloc(block_size);
    return last_alloc ? last_alloc : throwe(new(OutOfMemoryException)(block_size));
}

void* zeroalloc_(size_t block_size)
{
    DEBUG_WRITELINE("Called zeroalloc with size %zu B", block_size);
    last_alloc = calloc(1, block_size);
    return last_alloc ? last_alloc : throwe(new(OutOfMemoryException)(block_size));
}

void* memresize_(void* old_address, size_t new_size)
{
    DEBUG_WRITELINE("Called memresize on %p with new size %zu B", old_address, new_size);
    last_alloc = realloc(old_address, new_size);
    return last_alloc ? last_alloc : throwe(new(OutOfMemoryException)(new_size));
}

void memcopy_(void* dest, const void* source, size_t size)
{
    DEBUG_WRITELINE("Called memcopy from %p to %p with size %zu B", source, dest, size);
    for (size_t i = 0; i < size; ++i) {
        ((byte*)dest)[i] = ((byte*)source)[i];
    }
}

void memfree_(void* address)
{
    if (!address) return;
    DEBUG_WRITELINE("Called memfree on %p", address);
    free(address);
}

int main(int argc, char** argv)
{
    return Main(argc, argv);
}

#endif
