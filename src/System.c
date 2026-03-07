#include "Defines.h"

bool ValueEquator(size_t itemSize, object item1ref, object item2ref)
{
    for (int i = 0; i < itemSize; ++i) {
        if (((char*)item1ref)[i] != ((char*)item2ref)[i]) return false;
    }
    return true;
}

bool ReferenceEquator(object item1, object item2)
{
    return item1 == item2;
}

int ReferenceComparer(object left, object right)
{
    if (left < right) return 1;
    if (left > right) return -1;
    return 0;
}

void MemCopy(void* dest, const void* source, size_t size_memb)
{
    for (int i = 0; i < size_memb; ++i) {
        ((byte*)dest)[i] = ((byte*)source)[i];
    }
}

void MemCopyToNull(void* dest, const void* source)
{
    int i;
    for (i = 0; ((byte*)source)[i]; ++i) {
        ((byte*)dest)[i] = ((byte*)source)[i];
    }
    ((byte*)dest)[i] = 0;
}