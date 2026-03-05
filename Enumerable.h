#ifndef ENUMERABLE
#define ENUMERABLE

#include "Defines.h"

typedef struct enumerator_s {
    bool (*MoveNext)(struct enumerator_s* This);
    void (*Reset)(struct enumerator_s* This);
    void (*Dispose)(struct enumerator_s* This);
    object Current;
} IEnumerator;

typedef struct enumerable_s {
    IEnumerator* (*GetEnumerator)(const struct enumerable_s* This);
} IEnumerable;

typedef bool PredicateFunc(const object);

IEnumerable* Enumerable_Where(const IEnumerable* source, PredicateFunc* filter);

typedef object SelectorFunc(const object);

IEnumerable* Enumerable_Select(const IEnumerable* source, SelectorFunc* selector);

typedef IEnumerable* SelectManyFunc(const object);

/// @brief Projects each element of the collection into a new collection, then flattens the result.
/// @param source Source enumerable to project the elements of.
/// @param selector Selector function for the resulting collection.
/// @return A new enumerable.
IEnumerable* Enumerable_SelectMany(const IEnumerable* source, SelectManyFunc* selector);

typedef object AggregateFunc(const object, const object);

/// @brief Applies an accumulator function over a sequence.
/// @param source Enumerable to aggregate.
/// @param aggregate Accumulator function to apply.
/// @return The final accumulation value.
object Enumerable_Aggregate(const IEnumerable* source, AggregateFunc* aggregate);

/// @brief Determines if any element of a sequence satisfies a condition.
/// @param source The enumerable to check.
/// @param predicate Condition to satisfy.
/// @return True if any element satisfies the condition, false otherwise.
bool Enumerable_Any(const IEnumerable* source, PredicateFunc* predicate);

/// @brief Determines if all elements of a sequence satisfy a condition.
/// @param source The enumerable to check.
/// @param predicate Condition to satisfy.
/// @return True if all elements satisfy the condition, false otherwise.
bool Enumerable_All(const IEnumerable* source, PredicateFunc* predicate);

/// @brief Returns the first count items from a sequence.
/// @param source Enumerable to take items from.
/// @param count Amount of items to take.
/// @return A new enumerable.
IEnumerable* Enumerable_Take(const IEnumerable* source, int count);

/// @brief Skips the first count items from a sequence before returning the rest.
/// @param source Enumerable to skip items from.
/// @param count Amount of items to skip.
/// @return A new enumerable.
IEnumerable* Enumerable_Skip(const IEnumerable* source, int count);

/// @brief Searches for the given element in a sequence and returns the zero-based index of this element.
/// @param source Enumerable to search in.
/// @param item Item to search for.
/// @return The zero-based index of this item in the sequence.
int Enumerable_IndexOf(const IEnumerable* source, const object item);

/// @brief Returns the first element of the sequence that satisfies the condition.
/// @param source Enumerable to search in.
/// @param predicate Condition to satisfy.
/// @return The first element that satisfies the predicate.
object Enumerable_FirstOrDefault(const IEnumerable* source, PredicateFunc* predicate);

#endif