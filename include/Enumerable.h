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

typedef bool PredicateFunc(object);

/// @brief Filters a sequence based on a predicate.
/// @param source Enumerable to filter.
/// @param filter Condition that the elements must fulfill to be taken from the sequence.
/// @return A new enumerable.
IEnumerable* Enumerable_Where(const IEnumerable* source, PredicateFunc* filter);

typedef object SelectorFunc(object);

/// @brief Projects each element of a sequence into a new form.
/// @param source Enumerable to project.
/// @param selector Function to apply to each element.
/// @return A new enumerable.
IEnumerable* Enumerable_Select(const IEnumerable* source, SelectorFunc* selector);

typedef IEnumerable* SelectManyFunc(object);

/// @brief Projects each element of the collection into a new collection, then flattens the result.
/// @param source Source enumerable to project the elements of.
/// @param selector Selector function for the resulting collection.
/// @return A new enumerable.
IEnumerable* Enumerable_SelectMany(const IEnumerable* source, SelectManyFunc* selector);

typedef object AggregateFunc(object, object);

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

/// @brief Returns the element at the given zero-based index of the sequence.
/// @param source Enumerable to extract the element from.
/// @param index Index of the element.
/// @return The element at the specified index.
object Enumerable_ElementAt(const IEnumerable* source, int index);

/// @brief Searches for the given element in a sequence and returns the zero-based index of this element.
/// @param source Enumerable to search in.
/// @param item Item to search for.
/// @return The zero-based index of this item in the sequence.
int Enumerable_IndexOf(const IEnumerable* source, object item);

/// @brief Returns the first element of the sequence that satisfies the condition.
/// @param source Enumerable to search in.
/// @param predicate Condition to satisfy.
/// @return The first element that satisfies the predicate.
object Enumerable_FirstOrDefault(const IEnumerable* source, PredicateFunc* predicate);

/// @brief Appends an element to the end of a sequence.
/// @param source Enumerable to append the item to.
/// @param item Item to append.
/// @return A new enumerable.
IEnumerable* Enumerable_Append(const IEnumerable* source, object item);

/// @brief Prepends an element to the start of a sequence.
/// @param source Enumerable to prepend the item to.
/// @param item Item to prepend.
/// @return A new enumerable.
IEnumerable* Enumerable_Prepend(const IEnumerable* source, object item);

/// @brief Concatenates two sequences.
/// @param first The first sequence to concatenate.
/// @param second The second sequence to concatenate.
/// @return A new enumerable.
IEnumerable* Enumerable_Concat(const IEnumerable* first, const IEnumerable* second);

/// @brief Performs an action on each element of a collection.
/// @param source Enumerable to execute the action on.
/// @param action Action to execute on each item.
void Enumerable_ForEach(const IEnumerable* source, Action* action);

/// @brief Determines whether a sequence contains a specified element.
/// @param source Enumerable to search in.
/// @param item Item to search for.
bool Enumerable_Contains(const IEnumerable* source, object item);

/// @brief Returns a number that represents the amount of elements in the specified enumerable.
/// @param source Enumerable whose length to count.
/// @return A number representing the amount of items in source.
int Enumerable_Count(const IEnumerable* source);

/// @brief Determines whether two sequences are equal by comparing each element individually.
/// @param first First enumerable to compare.
/// @param second Second enumerable to compare.
/// @return True if every element from first is equal to second, false otherwise.
bool Enumerable_SequenceEqual(const IEnumerable* first, const IEnumerable* second);

#endif