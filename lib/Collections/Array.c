#include "Collections/Array.h"

typedef TAG(ArrayEnumerator) {
    IMPL(IEnumerator);
    int _currentIndex;
    Array _array;
} *ArrayEnumerator;

static bool ArrayMoveNext(IEnumerator This)
{
    ArrayEnumerator e = (ArrayEnumerator)This;
    if (e->_currentIndex < e->_array->Length) {
        This->Current = (e->_array->Values + e->_currentIndex * e->_array->_memberSize);
        e->_currentIndex += 1;
        return true;
    }
    return false;
}

static void ArrayReset(IEnumerator This)
{
    ((ArrayEnumerator)This)->_currentIndex = 0;
    This->Current = NULL;
}

static void ArrayDispose(IEnumerator This)
{
    memfree(This);
}

IEnumerator ArrayGetEnumerator(const IEnumerable This)
{
    auto result =  meminit(ArrayEnumerator) {
        .MoveNext = ArrayMoveNext,
        .Reset = ArrayReset,
        .Dispose = ArrayDispose,
        ._currentIndex = 0,
        ._array = (Array)This
    };
    return (IEnumerator)result;
}

Array Array__ctor(int memberSize, int maxLength)
{
    if (maxLength < 0) return NULL;
    Array result = memalloc(Array);
    if (maxLength == 0) {
        *result = default(TAG(Array));
        return result;
    }
    *result = init(Array) {
        .GetEnumerator = ArrayGetEnumerator,
        .MaxLength = maxLength,
        .Length = 0,
        .Values = arralloc(byte, maxLength * memberSize),
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

void Array_CopyTo(Array source, void* dest)
{
    for (int i = 0; i < source->Length; ++i) {
        for (int j = 0; j < source->_memberSize; ++j) {
            ((byte*)dest + i * source->_memberSize)[j] =
                ((byte*)source + i * source->_memberSize)[j];
        }
    }
}

void Array_Destroy(Array* source)
{
    memfree((*source)->Values);
    memfree(*source);
    source = NULL;
}

void Array_Fill(Array source, object itemRef)
{
    throw new(Exception)("Bruh moment: Not implemented!");
    for (int i = 0; i < source->MaxLength; ++i) {
        memcopy(source->Values + i * source->_memberSize, itemRef, source->_memberSize);
    }
    source->Length = source->MaxLength;
}

object Array_Get(Array source, int index)
{
    return source->Values + index * source->_memberSize;
}

void Array_Set(Array source, int index, object itemRef)
{
    throw new(Exception)("Bruh moment: Not implemented!");
    memcopy(source->Values + index * source->_memberSize, itemRef, source->_memberSize);
    if (source->Length <= index) source->Length = index + 1;
}

void Array_Resize(Array source, int newMaxLength)
{
    if (newMaxLength <= 0) return;
    source->Values = memresize_(source->Values, newMaxLength * source->_memberSize);
}

void Array_Sort(Array source, int (*comparer)(object, object))
{
    // if (comparer == NULL) comparer = ReferenceComparer;
    for (int gap = source->Length / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < source->Length; ++i) {
            for (int j = i; j >= gap && comparer(Array_Get(source, j - gap), Array_Get(source, j)) > 0; j -= gap) {
                object tmp = Array_Get(source, j - gap);
                Array_Set(source, j - gap, Array_Get(source, j));
                Array_Set(source, j, tmp);
            }
        }
    }
}
