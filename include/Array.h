#ifndef GENERIC_ARRAY
#define GENERIC_ARRAY
#include "Defines.h"
#include "Enumerable.h"

typedef struct generic_array_s {
    IEnumerable _parent;
    int Length;
    int MaxLength;
    object* Values;
} Array;

object Array__ctor(int maxLength);

Array* Enumerable_ToArray(IEnumerable* source);

void Array_Clear(Array* source);

void Array_CopyTo(Array* source, Array* dest);

void Array_Fill(Array* source, object item);

void Array_FillConstructible(Array* source, ctor* item_ctor);

void Array_Resize(Array* source, int newMaxLength);

void Array_Sort(Array* source, Comparer* comparer);

#endif