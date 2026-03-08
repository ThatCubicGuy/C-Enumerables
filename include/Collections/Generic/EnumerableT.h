#ifndef COLLECTIONS_GENERIC_ENUMERABLE
#define COLLECTIONS_GENERIC_ENUMERABLE

#include "Defines.h"

#pragma region Define

/**
 * @brief Enumerates an IEnumerable<T> and sets var to
 * the current enumeration value before executing code.
 */
#define foreach(T, var, source, code) do {                  \
    IEnumerable_##T __src = (IEnumerable_##T)source;        \
    IEnumerator_##T __e = (__src)->GetEnumerator(__src);    \
    while (__e->MoveNext(__e)) {                            \
        T var = __e->Current;                               \
        code;                                               \
    }                                                       \
    __e->Dispose(__e);                                      \
} while(0)

#define ENUMERABLE_DEFINE(T)                                                                            \
typedef struct IEnumerator_##T##_s {                                                                    \
    bool (*MoveNext)(struct IEnumerator_##T##_s* This);                                                 \
    void (*Reset)(struct IEnumerator_##T##_s* This);                                                    \
    void (*Dispose)(struct IEnumerator_##T##_s* This);                                                  \
    T Current;                                                                                          \
} *IEnumerator_##T;                                                                                     \
typedef const struct IEnumerable_##T##_s {                                                              \
    IEnumerator_##T (*GetEnumerator)(const struct IEnumerable_##T##_s* This);                           \
} *IEnumerable_##T;                                                                                     \
/**                                                                                                     \
 * @brief Filters a sequence based on a predicate.                                                      \
 * @param source Enumerable to filter.                                                                  \
 * @param filter Condition that the elements must fulfill to be taken from the sequence.                \
 * @return A new enumerable.                                                                            \
 * @pure                                                                                                \
 */                                                                                                     \
IEnumerable_##T Enumerable_##T##_Where(IEnumerable_##T source, bool (*filter)(T));                      \
/**                                                                                                     \
 * @brief Determines if any element of a sequence satisfies a condition.                                \
 * @param source The enumerable to check.                                                               \
 * @param predicate Condition to satisfy.                                                               \
 * @return True if any element satisfies the condition, false otherwise.                                \
 * @pure                                                                                                \
 */                                                                                                     \
bool Enumerable_##T##_Any(IEnumerable_##T source, bool (*predicate)(T));                                \
/**                                                                                                     \
 * @brief Determines if all elements of a sequence satisfy a condition.                                 \
 * @param source The enumerable to check.                                                               \
 * @param predicate Condition to satisfy.                                                               \
 * @return True if all elements satisfy the condition, false otherwise.                                 \
 * @pure                                                                                                \
 */                                                                                                     \
bool Enumerable_##T##_All(IEnumerable_##T source, bool (*predicate)(T));                                \
/**                                                                                                     \
 * @brief Returns the first count items from a sequence.                                                \
 * @param source Enumerable to take items from.                                                         \
 * @param count Amount of items to take.                                                                \
 * @return A new enumerable.                                                                            \
 * @pure                                                                                                \
 */                                                                                                     \
IEnumerable_##T Enumerable_##T##_Take(IEnumerable_##T source, int count);                               \
/**                                                                                                     \
 * @brief Skips the first count items from a sequence before returning the rest.                        \
 * @param source Enumerable to skip items from.                                                         \
 * @param count Amount of items to skip.                                                                \
 * @return A new enumerable.                                                                            \
 * @pure                                                                                                \
 */                                                                                                     \
IEnumerable_##T Enumerable_##T##_Skip(IEnumerable_##T source, int count);                               \
/**                                                                                                     \
 * @brief Returns the element at the given zero-based index of the sequence.                            \
 * @param source Enumerable to extract the element from.                                                \
 * @param index Index of the element.                                                                   \
 * @return The element at the specified index.                                                          \
 * @pure                                                                                                \
 */                                                                                                     \
T Enumerable_##T##_ElementAt(IEnumerable_##T source, int index);                                        \
/**                                                                                                     \
 * @brief Searches for the given element in a sequence                                                  \
 * and returns the zero-based index of this element.                                                    \
 * @param source Enumerable to search in.                                                               \
 * @param item Item to search for.                                                                      \
 * @return The zero-based index of this item in the sequence.                                           \
 * @pure                                                                                                \
 */                                                                                                     \
int Enumerable_##T##_IndexOf(IEnumerable_##T source, T item);                                           \
/**                                                                                                     \
 * @brief Returns the first element of the sequence that satisfies the condition.                       \
 * @param source Enumerable to search in.                                                               \
 * @param predicate Condition to satisfy.                                                               \
 * @return The first element that satisfies the predicate.                                              \
 * @pure                                                                                                \
 */                                                                                                     \
T Enumerable_##T##_FirstOrDefault(IEnumerable_##T source, bool (*predicate)(T));                        \
/**                                                                                                     \
 * @brief Appends an element to the end of a sequence.                                                  \
 * @param source Enumerable to append the item to.                                                      \
 * @param item Item to append.                                                                          \
 * @return A new enumerable.                                                                            \
 * @pure                                                                                                \
 */                                                                                                     \
IEnumerable_##T Enumerable_##T##_Append(IEnumerable_##T source, T item);                                \
/**                                                                                                     \
 * @brief Prepends an element to the start of a sequence.                                               \
 * @param source Enumerable to prepend the item to.                                                     \
 * @param item Item to prepend.                                                                         \
 * @return A new enumerable.                                                                            \
 * @pure                                                                                                \
 */                                                                                                     \
IEnumerable_##T Enumerable_##T##_Prepend(IEnumerable_##T source, T item);                               \
/**                                                                                                     \
 * @brief Concatenates two sequences.                                                                   \
 * @param first The first sequence to concatenate.                                                      \
 * @param second The second sequence to concatenate.                                                    \
 * @return A new enumerable.                                                                            \
 * @pure                                                                                                \
 */                                                                                                     \
IEnumerable_##T Enumerable_##T##_Concat(IEnumerable_##T first, IEnumerable_##T second);                 \
/**                                                                                                     \
 * @brief Determines whether a sequence contains a specified element.                                   \
 * @param source Enumerable to search in.                                                               \
 * @param item Item to search for.                                                                      \
 * @pure                                                                                                \
 */                                                                                                     \
bool Enumerable_##T##_Contains(IEnumerable_##T source, T item);                                         \
/**                                                                                                     \
 * @brief Returns a number that represents the amount of elements in the specified enumerable.          \
 * @param source Enumerable whose length to count.                                                      \
 * @return A number representing the amount of items in source.                                         \
 * @pure                                                                                                \
 */                                                                                                     \
int Enumerable_##T##_Count(IEnumerable_##T source);                                                     \
/**                                                                                                     \
 * @brief Determines whether two sequences are equal by comparing each element individually.            \
 * @param first First enumerable to compare.                                                            \
 * @param second Second enumerable to compare.                                                          \
 * @return True if every element from first is equal to second, false otherwise.                        \
 * @pure                                                                                                \
 */                                                                                                     \
bool Enumerable_##T##_SequenceEqual(IEnumerable_##T first, IEnumerable_##T second);                     \

#pragma region Converters

#define ENUMERABLE_DEFINE_INDEX(T)                                          \
/**                                                                         \
 * @brief Transforms a sequence by incorporating                            \
 * each element's index into a tuple.                                       \
 * @param source Enumerable to project.                                     \
 * @return A new enumerable.                                                \
 */                                                                         \
IEnumerable_int_##T Enumerable_##T##_Index(IEnumerable_##T source);

#define ENUMERABLE_DEFINE_SELECT(TSource, TResult)                          \
/**                                                                         \
 * @brief Projects each element of a sequence into a new form.              \
 * @param source Enumerable to project.                                     \
 * @param selector Function to apply to each element.                       \
 * @return A new enumerable.                                                \
 * @pure                                                                    \
 */                                                                         \
IEnumerable_##TResult Enumerable_##TSource##_Select_##TResult(IEnumerable_##TSource source, TResult (*selector)(TSource));

#define ENUMERABLE_DEFINE_SELECTINDEX(TSource, TResult)                     \
/**                                                                         \
 * @brief Projects each element of a sequence into a new form by            \
 * incorporating the element's index.                                       \
 * @param source Enumerable to project.                                     \
 * @param selector Function to apply to each element and its index.         \
 * @return A new enumerable.                                                \
 */                                                                         \
IEnumerable_##TResult Enumerable_##TSource##_SelectIndex_##TResult(IEnumerable_##TSource source, TResult (*selector)(TSource, int));

#define ENUMERABLE_DEFINE_SELECTMANY(TSource, TResult)                      \
/**                                                                         \
 * @brief Projects each element of the collection into a new collection,    \
 * then flattens the result into a single collection.                       \
 * @param source Source enumerable to project the elements of.              \
 * @param selector Selector function for the resulting collection.          \
 * @return A new enumerable.                                                \
 * @pure                                                                    \
 */                                                                         \
IEnumerable_##TResult Enumerable_##TSource##_SelectMany_##TResult(IEnumerable_##TSource source, IEnumerable_##TResult (*selector)(TSource));

#define ENUMERABLE_DEFINE_AGGREGATE(TSource, TAggregate)                    \
/**                                                                         \
 * @brief Applies an accumulator function over a sequence.                  \
 * @param source Enumerable to aggregate.                                   \
 * @param aggregate Accumulator function to apply.                          \
 * @return The final accumulation value.                                    \
 * @pure                                                                    \
 */                                                                         \
TAggregate Enumerable_##TSource##_Aggregate_##TAggregate(IEnumerable_##TSource source, TAggregate seed, TAggregate (*aggregate)(TAggregate, TSource));

#pragma endregion
#pragma endregion

#endif