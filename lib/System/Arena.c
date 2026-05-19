#include "Keywords.h"
#include "System.h"
#if ALLOC_TYPE == ARENA_ALLOC

#define ARENA_MEMORY_SIZE 16777216uL // 16 MB
#define ARENA_LIST_SIZE 16384uL // 16 KB

typedef TAG(Allocation) {
    void* address;
    size_t size;
} Allocation;

static TAG(ArenaStatic) {
    void* Start;
    size_t Tail, AllocCount, EmptySpaceCount;
    Allocation LastAlloc;
    Allocation EmptySpaces[ARENA_LIST_SIZE / 256]; // 1 KB
    Allocation Allocs[ARENA_LIST_SIZE]; // 256 KB
} Arena = {
    .Start = NULL,
    .LastAlloc = NULL,
    .Allocs = {0},
    .EmptySpaces = {0},
    .EmptySpaceCount = 0,
    .AllocCount = 0,
    .Tail = 0,
};

static void ARENA_INIT(void)
{
    DEBUG_WRITELINE("Called ARENA_INIT (Initial size: %zu B)", ARENA_MEMORY_SIZE);
    if (Arena.Start) {
        fprintf(stderr, "ERROR: Attempt to reinitialize arena!\n");
        exit(EXIT_FAILURE);
    }
    Arena.Start = malloc(ARENA_MEMORY_SIZE);
    if (!Arena.Start) {
        fprintf(stderr, "ERROR: Cannot allocate arena!");
        exit(EXIT_OUT_OF_MEMORY);
    }
}

static void ARENA_DESTROY(void)
{
    DEBUG_WRITELINE("Called ARENA_DESTROY (Current arena size: %zu B)", Arena.Tail);
    if (!Arena.Start) {
        fprintf(stderr, "WARNING: Attempt to deallocate uninitialized arena!");
        return;
    }
    free(Arena.Start);
    Arena = (TAG(ArenaStatic)) {
        .Start = NULL,
        .AllocCount = 0,
        .Allocs = {0},
        .Tail = 0,
    };
}

static Allocation ArenaAlloc(size_t block_size)
{
    TRACE_ASSERT(block_size > 0, "ERROR: Cannot allocate block of zero bytes!\n");
    TRACE_ASSERT(Arena.Tail + block_size < ARENA_MEMORY_SIZE, "ERROR: Allocation of size %zu B exceeds maximum arena memory!\n", block_size);
    if (!Arena.Start) ARENA_INIT();
    for (size_t i = 0; i < Arena.EmptySpaceCount; ++i) {

        // If we find a suitable empty space, perfect!
        if (Arena.EmptySpaces[i].size >= block_size) {

            // Save the allocation for returning
            Allocation result = Arena.EmptySpaces[i];

            // Adjust the empty space's metadata
            Arena.EmptySpaces[i].address += block_size;
            Arena.EmptySpaces[i].size -= block_size;

            // Remove the empty space if it's now filled
            if (Arena.EmptySpaces[i].size == 0) {
                Arena.EmptySpaceCount -= 1;
                for (size_t j = i; j < Arena.EmptySpaceCount; ++j) {
                    Arena.EmptySpaces[j] = Arena.EmptySpaces[j + 1];
                }
            }
            return result;
        }
    }

    // Increase the stack pointer and return the new space
    Arena.Tail += block_size;
    return Arena.Allocs[Arena.AllocCount++] = (Allocation) {
        .address = Arena.Start + Arena.Tail - block_size,
        .size = block_size,
    };
}

static Allocation ArenaDealloc(void* obj)
{
    for (size_t i = 0; i < Arena.AllocCount; ++i) {
        if (Arena.Allocs[i].address == obj) {

            // Mark the space as freed
            Arena.EmptySpaces[Arena.EmptySpaceCount++] = (Allocation) {
                .address = obj,
                .size = Arena.Allocs[i].size
            };
            // Compress the allocations array
            Arena.AllocCount -= 1;
            for (size_t j = i; j < Arena.AllocCount; ++j) {
                Arena.Allocs[j] = Arena.Allocs[j + 1];
            }
            if (Arena.AllocCount == 0) ARENA_DESTROY();
            return Arena.EmptySpaces[Arena.EmptySpaceCount - 1];
        }
    }

    throw new(Exception)("ERROR: Attempt to dealloc unallocated arena memory!\n");
    return default(Allocation);
}

void* memalloc_(size_t block_size)
{
    DEBUG_WRITELINE("Called memalloc with size %zu B. Current stack size: %zu B", block_size, Arena.Tail);
    if (block_size == 0) return Arena.LastAlloc.address;
    Arena.LastAlloc = ArenaAlloc(block_size);
    return Arena.LastAlloc.address;
}

void* zeroalloc_(size_t block_size)
{
    DEBUG_WRITELINE("Called zeroalloc with size %zu B. Current stack size: %zu B", block_size, Arena.Tail);
    Arena.LastAlloc = ArenaAlloc(block_size);
    for (size_t i = 0; i < block_size; ++i) {
        ((byte*)Arena.LastAlloc.address)[i] = 0;
    }
    return Arena.LastAlloc.address;
}

void* memresize_(void* old_address, size_t new_size)
{
    DEBUG_WRITELINE("Called memresize on %p with new size %zu B. Current stack size: %zu B", old_address, new_size, Arena.Tail);
    Allocation previous = ArenaDealloc(old_address);
    Arena.LastAlloc = ArenaAlloc(new_size);
    for (size_t i = 0; i < previous.size; ++i) {
        ((byte*)Arena.LastAlloc.address)[i] = ((byte*)previous.address)[i];
    }
    return Arena.LastAlloc.address;
}

void memcopy_(void* dest, const void* source, size_t size)
{
    DEBUG_WRITELINE("Called memcopy from %p to %p with size %zu B. Current stack size: %zu B", source, dest, size, Arena.Tail);
    for (size_t i = 0; i < size; ++i) {
        ((byte*)dest)[i] = ((byte*)source)[i];
    }
}

void memfree_(void* address)
{
    if (!address) return;
    DEBUG_WRITELINE("Called memfree on %p. Current stack size: %zu B", address, Arena.Tail);
    ArenaDealloc(address);
}

int main(int argc, char** argv)
{
    ARENA_INIT(void);
    int ret_code = Main(argc, argv);
    ARENA_DESTROY(void);
    return ret_code;
}

#endif
