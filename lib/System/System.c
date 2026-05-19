#include "System.h"
#include "Keywords.h"
#define b(PTR) ((byte*)PTR)

const TAG(IEqualityComparer(object)) ObjectEquator[1] = {{
    .Equals = object_ReferenceEquals,
    .GetHashCode = object_GetHashCode
}};

bool object_Equals(size_t size, object left, object right)
{
    for (size_t i = 0; i < size; ++i) {
        if (b(left)[i] != b(right)[i]) return false;
    }

    return true;
}

bool object_ReferenceEquals(object left, object right)
{
    return left == right;
}

size_t object_GetHashCode(object obj)
{
    size_t hash = ((size_t)obj >> 4);

    hash ^= hash >> 30;
    hash *= 0xbf58476d1ce4e5b9ul;

    hash ^= hash >> 27;
    hash *= 0x94d049bb133111ebul;

    hash ^= hash >> 31;

    return hash;
}
