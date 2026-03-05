#include "Array.h"

typedef struct array_enumerator_s {
    IEnumerator _parent;
    int _currentIndex;
    Array* _array;
} ArrayEnumerator;

static bool ArrayMoveNext(IEnumerator *This)
{
    ArrayEnumerator *e = (ArrayEnumerator*)This;
    if (e->_currentIndex < e->_array->Length) {
        This->Current = e->_array->Values[e->_currentIndex];
        e->_currentIndex += 1;
        return true;
    }
    return false;
}

static void ArrayReset(IEnumerator *This)
{
    ((ArrayEnumerator*)This)->_currentIndex = 0;
    This->Current = NULL;
}

static void ArrayDispose(IEnumerator *This)
{
    free(This);
}

static IEnumerator* ArrayGetEnumerator(const IEnumerable* This)
{
    ArrayEnumerator* result = bare(ArrayEnumerator);
    *result = (ArrayEnumerator) {
        ._parent = (IEnumerator) {
            .MoveNext = ArrayMoveNext,
            .Reset = ArrayReset,
            .Dispose = ArrayDispose
        },
        ._currentIndex = 0,
        ._array = (Array*)This
    };
    return (IEnumerator*)result;
}

static int DefaultEqualityComparer(object left, object right)
{
    if (left < right) return 1;
    if (left > right) return -1;
    return 0;
}

object Array__ctor(int maxLength)
{
    Array* result = bare(Array);
    *result = (Array) {
        .MaxLength = maxLength,
        .Length = 0,
        ._parent = (IEnumerable) {
            .GetEnumerator = ArrayGetEnumerator
        }
    };
    if (maxLength > 0) {
        result->Values = new_array(object, maxLength);
    }
    return result;
}

Array* Enumerable_ToArray(IEnumerable* source)
{
    // Assume initial capacity
    int maxLength = 32;
    IEnumerator* e = source->GetEnumerator(source);
    Array* result = bare(Array);
    *result = (Array) {
        ._parent = (IEnumerable) {
            .GetEnumerator = ArrayGetEnumerator
        },
        .Length = 0,
        .MaxLength = maxLength,
        .Values = new_array(object, maxLength)
    };
    for (int i = 0; e->MoveNext(e); ++i) {
        if (i >= result->MaxLength) {
            Array_Resize(result, result->MaxLength * 2);
        }
        result->Values[i] = e->Current;
        result->Length += 1;
    }
    Array_Resize(result, result->Length);
    return result;
}

void Array_Clear(Array* source)
{
    for (int i = 0; i < source->MaxLength; ++i) {
        source->Values[i] = NULL;
    }
    source->Length = 0;
}

void Array_CopyTo(Array* source, Array* dest)
{
    if (dest->MaxLength < source->Length) Array_Resize(dest, source->Length);
    for (dest->Length = 0; dest->Length < source->Length; ++dest->Length) {
        dest->Values[dest->Length] = source->Values[dest->Length];
    }
}

void Array_Fill(Array* source, object item)
{
    for (int i = 0; i < source->MaxLength; ++i) {
        source->Values[i] = item;
    }
    source->Length = source->MaxLength;
}

void Array_FillConstructible(Array* source, ctor* item_ctor)
{
    for (int i = 0; i < source->MaxLength; ++i) {
        source->Values[i] = item_ctor();
    }
    source->Length = source->MaxLength;
}

void Array_Resize(Array* source, int newMaxLength)
{
    if (newMaxLength <= 0) return;
    source->Values = realloc(source->Values, newMaxLength);
}

void Array_Sort(Array* source, Comparer* comparer)
{
    if (comparer == NULL) comparer = DefaultEqualityComparer;
    qsort(source->Values, source->Length, sizeof(object), comparer);
}
