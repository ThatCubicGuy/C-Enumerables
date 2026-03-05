#ifndef ENUMERABLE
#define ENUMERABLE

#include "Defines.h"

typedef struct enumerator_s {
    bool (*MoveNext)(struct enumerator_s* This);
    void (*Reset)(struct enumerator_s* This);
    object Current;
} IEnumerator;

typedef struct enumerable_s {
    IEnumerator* (*GetEnumerator)(struct enumerable_s* This);
} IEnumerable;

typedef object SelectorFunc(object);

IEnumerable* Enumerable_Select(IEnumerable* source, SelectorFunc* selector);

typedef bool FilterFunc(object);

IEnumerable* Enumerable_Where(IEnumerable* source, FilterFunc* filter);

#endif