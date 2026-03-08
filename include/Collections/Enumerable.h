#ifndef COLLECTIONS_ENUMERABLE
#define COLLECTIONS_ENUMERABLE

#include "Defines.h"

/**
 * @brief Enumerates an IEnumerable and sets var to
 * the current enumeration value before executing code.
 */
#define foreach_ref(T, var, source, code) do {          \
    IEnumerable __src = (IEnumerable)source;            \
    IEnumerator __e = (__src)->GetEnumerator(__src);    \
    while (__e->MoveNext(__e)) {                        \
        T var = (T)__e->Current;                        \
        code;                                           \
    }                                                   \
    __e->Dispose(__e);                                  \
} while(0)

/**
 * @brief Enumerates an IEnumerable and casts the
 * current enumeration value to a T pointer before
 * dereferencing it into var and executing code.
 */
#define foreach_as(T, var, source, code) do {           \
    IEnumerable __src = (IEnumerable)source;            \
    IEnumerator __e = (__src)->GetEnumerator(__src);    \
    while (__e->MoveNext(__e)) {                        \
        T var = *(T*)__e->Current;                      \
        code;                                           \
    }                                                   \
    __e->Dispose(__e);                                  \
} while(0)

typedef struct IEnumerator_s {
    bool (*MoveNext)(struct IEnumerator_s* This);
    void (*Reset)(struct IEnumerator_s* This);
    void (*Dispose)(struct IEnumerator_s* This);
    object Current;
} *IEnumerator;

typedef const struct IEnumerable_s {
    const IEnumerator (*GetEnumerator)(const struct IEnumerable_s* This);
} *IEnumerable;

typedef bool PredicateFunc(object);

/// @brief Filters a sequence based on a predicate.
/// @param source Enumerable to filter.
/// @param filter Condition that the elements must fulfill to be taken from the sequence.
/// @return A new enumerable.
IEnumerable Enumerable_Where(IEnumerable source, PredicateFunc* filter);

typedef object SelectorFunc(object);

/// @brief Projects each element of a sequence into a new form.
/// @param source Enumerable to project.
/// @param selector Function to apply to each element.
/// @return A new enumerable.
IEnumerable Enumerable_Select(IEnumerable source, SelectorFunc* selector);

typedef object SelectIndexFunc(object, int);

IEnumerable Enumerable_SelectIndex(IEnumerable source, SelectIndexFunc* selector);

typedef IEnumerable SelectManyFunc(object);

/// @brief Projects each element of the collection into a new collection, then flattens the result.
/// @param source Source enumerable to project the elements of.
/// @param selector Selector function for the resulting collection.
/// @return A new enumerable.
IEnumerable Enumerable_SelectMany(IEnumerable source, SelectManyFunc* selector);

typedef object AggregateFunc(object, object);

/// @brief Applies an accumulator function over a sequence.
/// @param source Enumerable to aggregate.
/// @param aggregate Accumulator function to apply.
/// @return The final accumulation value.
object Enumerable_Aggregate(IEnumerable source, AggregateFunc* aggregate);

/// @brief Determines if any element of a sequence satisfies a condition.
/// @param source The enumerable to check.
/// @param predicate Condition to satisfy.
/// @return True if any element satisfies the condition, false otherwise.
bool Enumerable_Any(IEnumerable source, PredicateFunc* predicate);

/// @brief Determines if all elements of a sequence satisfy a condition.
/// @param source The enumerable to check.
/// @param predicate Condition to satisfy.
/// @return True if all elements satisfy the condition, false otherwise.
bool Enumerable_All(IEnumerable source, PredicateFunc* predicate);

/// @brief Returns the first count items from a sequence.
/// @param source Enumerable to take items from.
/// @param count Amount of items to take.
/// @return A new enumerable.
IEnumerable Enumerable_Take(IEnumerable source, int count);

/// @brief Skips the first count items from a sequence before returning the rest.
/// @param source Enumerable to skip items from.
/// @param count Amount of items to skip.
/// @return A new enumerable.
IEnumerable Enumerable_Skip(IEnumerable source, int count);

/// @brief Returns the element at the given zero-based index of the sequence.
/// @param source Enumerable to extract the element from.
/// @param index Index of the element.
/// @return The element at the specified index.
object Enumerable_ElementAt(IEnumerable source, int index);

/// @brief Searches for the given element in a sequence and returns the zero-based index of this element.
/// @param source Enumerable to search in.
/// @param item Item to search for.
/// @return The zero-based index of this item in the sequence.
int Enumerable_IndexOf(IEnumerable source, object item);

/// @brief Returns the first element of the sequence that satisfies the condition.
/// @param source Enumerable to search in.
/// @param predicate Condition to satisfy.
/// @return The first element that satisfies the predicate.
object Enumerable_FirstOrDefault(IEnumerable source, PredicateFunc* predicate);

/// @brief Appends an element to the end of a sequence.
/// @param source Enumerable to append the item to.
/// @param item Item to append.
/// @return A new enumerable.
IEnumerable Enumerable_Append(IEnumerable source, object item);

/// @brief Prepends an element to the start of a sequence.
/// @param source Enumerable to prepend the item to.
/// @param item Item to prepend.
/// @return A new enumerable.
IEnumerable Enumerable_Prepend(IEnumerable source, object item);

/// @brief Concatenates two sequences.
/// @param first The first sequence to concatenate.
/// @param second The second sequence to concatenate.
/// @return A new enumerable.
IEnumerable Enumerable_Concat(IEnumerable first, IEnumerable second);

/// @brief Performs an action on each element of a collection.
/// @param source Enumerable to execute the action on.
/// @param action Action to execute on each item.
void Enumerable_ForEach(IEnumerable source, Action* action);

/// @brief Determines whether a sequence contains a specified element.
/// @param source Enumerable to search in.
/// @param item Item to search for.
bool Enumerable_Contains(IEnumerable source, object item);

/// @brief Returns a number that represents the amount of elements in the specified enumerable.
/// @param source Enumerable whose length to count.
/// @return A number representing the amount of items in source.
int Enumerable_Count(IEnumerable source);

/// @brief Determines whether two sequences are equal by comparing each element individually.
/// @param first First enumerable to compare.
/// @param second Second enumerable to compare.
/// @return True if every element from first is equal to second, false otherwise.
bool Enumerable_SequenceEqual(IEnumerable first, IEnumerable second);

#endif