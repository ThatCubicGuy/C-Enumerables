#include "System.h"
#include "Keywords.h"
#define b(PTR) ((byte*)PTR)

const struct tag_IEqualityComparer_object ObjectEquator[1] = {{
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
    return (((size_t)obj & 0xFF) * 1 +
            ((size_t)obj & 0xFF00) * 3821 +
            ((size_t)obj & 0xFF0000) * 52147 +
            ((size_t)obj & 0xFF000000) * 300463)
        * 1047997;
}
