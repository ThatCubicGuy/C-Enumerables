#include "Collections/Array.h"

typedef struct array_enumerator_s {
    IEnumerator _parent;
    int _currentIndex;
    Array _array;
} *ArrayEnumerator;

static bool ArrayMoveNext(IEnumerator *This)
{
    ArrayEnumerator e = (ArrayEnumerator)This;
    if (e->_currentIndex < e->_array->Length) {
        This->Current = (e->_array->Values + e->_currentIndex * e->_array->_memberSize);
        e->_currentIndex += 1;
        return true;
    }
    return false;
}

static void ArrayReset(IEnumerator *This)
{
    ((ArrayEnumerator)This)->_currentIndex = 0;
    This->Current = NULL;
}

static void ArrayDispose(IEnumerator *This)
{
    free(This);
}

IEnumerator* ArrayGetEnumerator(const IEnumerable* This)
{
    ArrayEnumerator result = alloc(struct array_enumerator_s);
    init(struct array_enumerator_s, result) {
        ._parent = (IEnumerator) {
            .MoveNext = ArrayMoveNext,
            .Reset = ArrayReset,
            .Dispose = ArrayDispose
        },
        ._currentIndex = 0,
        ._array = (Array)This
    };
    return (IEnumerator*)result;
}

Array Array__ctor(int memberSize, int maxLength)
{
    if (maxLength < 0) return NULL;
    Array result = alloc(struct array_s);
    if (maxLength == 0) {
        *result = default(struct array_s);
        return result;
    }
    init(struct array_s, result) {
        ._parent = (IEnumerable) {
            .GetEnumerator = ArrayGetEnumerator
        },
        .MaxLength = maxLength,
        .Length = 0,
        .Values = alloc_array(byte, maxLength * memberSize),
        ._memberSize = memberSize
    };
    return result;
}

void Array_Clear(Array source)
{
    for (int i = 0; i < source->Length; ++i) {
        for (int j = 0; j < source->_memberSize; ++j) {
            ((byte*)source->Values + i * source->_memberSize)[j] = 0;
        }
    }
    source->Length = 0;
}

void Array_CopyTo(Array source, Array dest)
{
    Array_Clear(dest);
    dest->_memberSize = source->_memberSize;
    Array_Resize(dest, dest->MaxLength);
    if (dest->MaxLength < source->Length) Array_Resize(dest, source->Length);
    for (dest->Length = 0; dest->Length < source->Length; ++dest->Length) {
        for (int j = 0; j < dest->_memberSize; ++j) {
            ((byte*)dest->Values + dest->Length * dest->_memberSize)[j] =
                ((byte*)source->Values + dest->Length * source->_memberSize)[j];
        }
    }
}

void Array_Destroy(Array* source)
{
    free((*source)->Values);
    free(*source);
    source = NULL;
}

void Array_Fill(Array source, object itemRef)
{
    for (int i = 0; i < source->MaxLength; ++i) {
        MemCopy(source->Values + i * source->_memberSize, itemRef, source->_memberSize);
    }
    source->Length = source->MaxLength;
}

object Array_Get(Array source, int index)
{
    return source->Values + index * source->_memberSize;
}

void Array_Resize(Array source, int newMaxLength)
{
    if (newMaxLength <= 0) return;
    source->Values = realloc(source->Values, newMaxLength * source->_memberSize);
}

void Array_Set(Array source, int index, object itemRef)
{
    MemCopy(source->Values + index * source->_memberSize, itemRef, source->_memberSize);
}

void Array_Sort(Array source, Comparer* comparer)
{
    if (comparer == NULL) comparer = ReferenceComparer;
    qsort(source->Values, source->Length, source->_memberSize, comparer);
}
