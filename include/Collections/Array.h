#ifndef COLLECTIONS_ARRAY
#define COLLECTIONS_ARRAY
#include "Keywords.h"
#include "Enumerable.h"
#pragma region Defines

/**
 * @brief Managed array with variable amount of items
 * stored contiguously in memory.
 */
typedef TAG(Array) {
    IMPL(IEnumerable);
    // Amount of items contained in the array.
    int Length;
    // Maximum amount of items that this array can hold without resizing.
    int MaxLength;
    // Size of each member, in bytes.
    int _memberSize;
    // Values contained in this array.
    void* Values;
} *Array;

Array new(Array)(int memberSize, int maxLength);

void Array_Clear(Array source);

/**
 * @brief Copies the elements of an Array to a
 * one-dimensional built-in array.
 */
void Array_CopyTo(Array source, void* dest);

void Array_Destroy(Array* source);

void Array_Fill(Array source, object itemRef);

/**
 * @brief Returns a reference to the given element in the array.
 * @param source Array to get the item from.
 * @param index Index of the item to get.
 */
object Array_Get(Array source, int index);

/**
 * @brief Sets the value at the index to the value at the given reference.
 * @param source Array to set the item in.
 * @param index Index at which to set the item.
 * @param valueRef Reference to the value which should be set.
 */
void Array_Set(Array source, int index, object valueRef);

void Array_Resize(Array source, int newMaxLength);

void Array_Sort(Array source, int (*comparer)(object, object));

#pragma region Generics

#ifdef COLLECTIONS_GENERIC_ENUMERABLE
#ifndef ENUMERABLE_TO_ARRAY_DEFINED
#define ENUMERABLE_TO_ARRAY_DEFINED
#define ENUMERABLE_TO_ARRAY_DEFINE(T) Array Enumerable_##T##_ToArray(IEnumerable(T) source);
#define ENUMERABLE_TO_ARRAY_IMPLEMENT(T)                            \
Array Enumerable_##T##_ToArray(IEnumerable(T) source)               \
{                                                                   \
    /* Assume initial capacity to avoid useless enumeration */      \
    int maxLength = 16;                                             \
    auto result = meminit(Array) {                                  \
        .GetEnumerator = ArrayGetEnumerator,                        \
        .Length = 0,                                                \
        .MaxLength = maxLength,                                     \
        .Values = arralloc(T, maxLength)                            \
    };                                                              \
    foreach (T item in source) {                                    \
        if (result->Length >= result->MaxLength) {                  \
            Array_Resize(result, result->MaxLength * 2);            \
        }                                                           \
        ((T*)result->Values)[result->Length] = item;                \
        result->Length += 1;                                        \
    }                                                               \
    Array_Resize(result, result->Length);                           \
    return result;                                                  \
}
#endif
#else
#define ENUMERABLE_TO_ARRAY_DEFINE(T)
#define ENUMERABLE_TO_ARRAY_IMPLEMENT(T)
#endif
#define ARRAY_DEFINE(T)                                     \
Array Array_##T##__ctor(int maxLength);                     \
T Array_##T##_Get(Array source, int index);                 \
void Array_##T##_Set(Array source, int index, T value);     \
int Array_##T##_IndexOf(Array source, T item);              \
void Array_##T##_Fill(Array source, T item);                \
void Array_##T##_Initialize(Array source);                  \
ENUMERABLE_TO_ARRAY_DEFINE(T)

#define new_array(T) Array_##T##__ctor

#pragma endregion

#pragma endregion

#pragma region Implement

IEnumerator ArrayGetEnumerator(IEnumerable This);

#define ARRAY_IMPLEMENT(T)                                                      \
Array Array_##T##__ctor(int maxLength)                                          \
{                                                                               \
    if (maxLength < 0) return NULL;                                             \
    if (maxLength == 0) return memalloc(Array);                                 \
    auto result = meminit(Array) {                                              \
        .GetEnumerator = ArrayGetEnumerator,                                    \
        .Length = 0,                                                            \
        .MaxLength = maxLength,                                                 \
        .Values = arralloc(T, maxLength),                                       \
        ._memberSize = sizeof(T)                                                \
    };                                                                          \
    return result;                                                              \
}                                                                               \
T Array_##T##_Get(Array source, int index)                                      \
{                                                                               \
    return ((T*)source->Values)[index];                                         \
}                                                                               \
void Array_##T##_Set(Array source, int index, T value)                          \
{                                                                               \
    ((T*)source->Values)[index] = value;                                        \
}                                                                               \
int Array_##T##_IndexOf(Array source, T item)                                   \
{                                                                               \
    for (int i = 0; i < source->Length; ++i) {                                  \
        if (equals(((T*)source->Values)[i], item)) {                            \
            return i;                                                           \
        }                                                                       \
    }                                                                           \
    return -1;                                                                  \
}                                                                               \
void Array_##T##_Fill(Array source, T item)                                     \
{                                                                               \
    for (int i = 0; i < source->MaxLength; ++i) {                               \
        ((T*)source->Values)[i] = item;                                         \
    }                                                                           \
}                                                                               \
void Array_##T##_Initialize(Array source)                                       \
{                                                                               \
    for (int i = 0; i < source->MaxLength; ++i) {                               \
        ((T*)source->Values)[i] = new(T)();                                     \
    }                                                                           \
    source->Length = source->MaxLength;                                         \
}                                                                               \
ENUMERABLE_TO_ARRAY_IMPLEMENT(T)

#pragma endregion

#endif