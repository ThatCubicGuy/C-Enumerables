#include "Enumerable.h"
#include "Defines.h"
#include "List.h"
#include "LinkedList.h"

#pragma region Generics

typedef struct generic_enumerator_s {
    IEnumerator _parent;
    IEnumerator* _currentEnumerator;
    const IEnumerable* _baseEnumerable;
} ModifiedEnumerator;

static void ModifiedReset(IEnumerator *This)
{
    ModifiedEnumerator* e = (ModifiedEnumerator*)This;
    e->_currentEnumerator->Reset(e->_currentEnumerator);
}

static void ModifiedDispose(IEnumerator *This)
{
    ModifiedEnumerator* e = (ModifiedEnumerator*)This;
    e->_currentEnumerator->Dispose(e->_currentEnumerator);
    free(This);
}

#pragma endregion

#pragma region Where

typedef struct where_enumerable_s {
    IEnumerable _parent;
    const IEnumerable* _baseEnumerable;
    PredicateFunc* _filter;
} WhereEnumerable;

static bool WhereMoveNext(IEnumerator *This)
{
    ModifiedEnumerator* where = (ModifiedEnumerator*)This;
    while (where->_currentEnumerator->MoveNext(where->_currentEnumerator)) {
        This->Current = where->_currentEnumerator->Current;
        if (((const WhereEnumerable*)where->_baseEnumerable)->_filter(This->Current)) return true;
    }

    return false;
}

static IEnumerator* GetWhereEnumerator(const IEnumerable *This)
{
    const WhereEnumerable* where = (const WhereEnumerable*)This;

    ModifiedEnumerator* result = new(ModifiedEnumerator);

    *result = (ModifiedEnumerator) {
        ._parent = (IEnumerator) {
            .MoveNext = WhereMoveNext,
            .Reset = ModifiedReset,
            .Dispose = ModifiedDispose
        },
        ._currentEnumerator = where->_baseEnumerable->GetEnumerator(where->_baseEnumerable),
        ._baseEnumerable = (const IEnumerable*)where
    };

    return (IEnumerator*)result;
}

IEnumerable* Enumerable_Where(const IEnumerable* source, PredicateFunc* filter)
{
    WhereEnumerable* result = new(WhereEnumerable);
    *result = (WhereEnumerable) {
        ._parent = (IEnumerable) {
            .GetEnumerator = GetWhereEnumerator
        },
        ._filter = filter,
        ._baseEnumerable = source
    };
    return (IEnumerable*)result;
}

#pragma endregion

#pragma region Select

typedef struct select_enumerable_s {
    IEnumerable _parent;
    const IEnumerable* _baseEnumerable;
    SelectorFunc* _selector;
} SelectEnumerable;

static bool SelectMoveNext(IEnumerator *This)
{
    ModifiedEnumerator* select = (ModifiedEnumerator*)This;
    if (select->_currentEnumerator->MoveNext(select->_currentEnumerator)) {
        This->Current = ((const SelectEnumerable*)select->_baseEnumerable)->_selector(select->_currentEnumerator->Current);
        return true;
    }

    return false;
}

static IEnumerator* GetSelectEnumerator(const IEnumerable *This)
{
    const SelectEnumerable* select = (const SelectEnumerable*)This;

    ModifiedEnumerator* result = new(ModifiedEnumerator);

    *result = (ModifiedEnumerator) {
        ._parent = (IEnumerator) {
            .MoveNext = SelectMoveNext,
            .Reset = ModifiedReset,
            .Dispose = ModifiedDispose
        },
        ._currentEnumerator = select->_baseEnumerable->GetEnumerator(select->_baseEnumerable),
        ._baseEnumerable = (const IEnumerable*)select
    };

    return (IEnumerator*)result;
}

/// @brief Projects each element of a sequence into a new form.
/// @param source Enumerable to project.
/// @param selector Function to apply to each element.
/// @return A new enumerable.
IEnumerable* Enumerable_Select(const IEnumerable* source, SelectorFunc* selector)
{
    SelectEnumerable* result = new(SelectEnumerable);
    *result = (SelectEnumerable) {
        ._parent = (IEnumerable) {
            .GetEnumerator = GetSelectEnumerator
        },
        ._selector = selector,
        ._baseEnumerable = source
    };
    return (IEnumerable*)result;
}

#pragma endregion

#pragma region SelectMany

typedef struct select_many_enumerable_s {
    IEnumerable _parent;
    const IEnumerable* _baseEnumerable;
    SelectManyFunc* _selector;
} SelectManyEnumerable;

typedef struct select_many_enumerator_s {
    IEnumerator _parent;
    const IEnumerable* _baseEnumerable;
    IEnumerator* _innerEnumerator;
    IEnumerator* _outerEnumerator;
} SelectManyEnumerator;

static bool SelectManyMoveNext(IEnumerator *This)
{
    SelectManyEnumerator* selectMany = (SelectManyEnumerator*)This;

    if (selectMany->_innerEnumerator != NULL && selectMany->_innerEnumerator->MoveNext(selectMany->_innerEnumerator)) {
        This->Current = selectMany->_innerEnumerator->Current;
        return true;
    } else if (selectMany->_outerEnumerator->MoveNext(selectMany->_outerEnumerator)) {
        if (selectMany->_innerEnumerator != NULL) selectMany->_innerEnumerator->Dispose(selectMany->_innerEnumerator);
        IEnumerable* result = ((const SelectManyEnumerable*)selectMany->_baseEnumerable)->_selector(selectMany->_outerEnumerator->Current);
        selectMany->_innerEnumerator = result->GetEnumerator(result);
        return SelectManyMoveNext(This);
    }

    return false;
}

static void SelectManyReset(IEnumerator *This)
{
    SelectManyEnumerator* selectMany = (SelectManyEnumerator*)This;
    selectMany->_innerEnumerator = NULL;
    selectMany->_outerEnumerator->Reset(selectMany->_outerEnumerator);
}

static void SelectManyDispose(IEnumerator *This)
{
    SelectManyEnumerator* selectMany = (SelectManyEnumerator*)This;
    if (selectMany->_innerEnumerator != NULL) selectMany->_innerEnumerator->Dispose(selectMany->_innerEnumerator);
    selectMany->_outerEnumerator->Dispose(selectMany->_outerEnumerator);
    free(This);
}

static IEnumerator* GetSelectManyEnumerator(const IEnumerable* This)
{
    const SelectManyEnumerable* selectMany = (const SelectManyEnumerable*)This;

    SelectManyEnumerator* result = new(SelectManyEnumerator);

    *result = (SelectManyEnumerator) {
        ._parent = (IEnumerator) {
            .MoveNext = SelectManyMoveNext,
            .Reset = SelectManyReset,
            .Dispose = SelectManyDispose
        },
        ._outerEnumerator = selectMany->_baseEnumerable->GetEnumerator(selectMany->_baseEnumerable),
        ._baseEnumerable = (const IEnumerable*)selectMany
    };

    return (IEnumerator*)result;
}

/// @brief Projects each element of the collection into a new collection, then flattens the result.
/// @param source Source enumerable to project the elements of.
/// @param selector Selector function for the resulting collection.
/// @return A new enumerable.
IEnumerable* Enumerable_SelectMany(const IEnumerable* source, SelectManyFunc* selector)
{
    SelectManyEnumerable* result = new(SelectManyEnumerable);
    *result = (SelectManyEnumerable) {
        ._parent = (IEnumerable) {
            .GetEnumerator = GetSelectManyEnumerator,
        },
        ._selector = selector,
        ._baseEnumerable = source
    };
    return (IEnumerable*)result;
}

#pragma endregion

#pragma region Take / Skip

typedef struct take_skip_enumerable_s {
    IEnumerable _parent;
    const IEnumerable* _baseEnumerable;
    int Count;
} LimitedEnumerable;


typedef struct take_skip_enumerator_s {
    ModifiedEnumerator _parent;
    int Count;
} LimitedEnumerator;

static void LimitedReset(IEnumerator *This)
{
    ((LimitedEnumerator*)This)->Count = ((LimitedEnumerable*)((ModifiedEnumerator*)This)->_baseEnumerable)->Count;
    ModifiedReset(This);
}

static bool TakeMoveNext(IEnumerator *This)
{
    if (((LimitedEnumerator*)This)->Count == 0) return false;
    ((LimitedEnumerator*)This)->Count -= 1;
    ModifiedEnumerator* modified = (ModifiedEnumerator*)This;
    if (modified->_currentEnumerator->MoveNext(modified->_currentEnumerator)) {
        This->Current = modified->_currentEnumerator->Current;
        return true;
    }
    return false;
}

static IEnumerator* GetTakeEnumerator(const IEnumerable *This)
{
    const LimitedEnumerable* limited = (const LimitedEnumerable*)This;

    LimitedEnumerator* result = new(LimitedEnumerator);

    *result = (LimitedEnumerator) {
        ._parent = (ModifiedEnumerator) {
            ._parent = (IEnumerator) {
                .MoveNext = TakeMoveNext,
                .Reset = LimitedReset,
                .Dispose = ModifiedDispose,
            },
            ._currentEnumerator = limited->_baseEnumerable->GetEnumerator(limited->_baseEnumerable),
            ._baseEnumerable = (const IEnumerable*)limited,
        },
        .Count = limited->Count
    };

    return (IEnumerator*)result;
}

static bool SkipMoveNext(IEnumerator *This)
{
    ModifiedEnumerator* modified = (ModifiedEnumerator*)This;
    while (((LimitedEnumerator*)This)->Count > 0) {
        ((LimitedEnumerator*)This)->Count -= 1;
        modified->_currentEnumerator->MoveNext(modified->_currentEnumerator);
    }
    if (modified->_currentEnumerator->MoveNext(modified->_currentEnumerator)) {
        This->Current = modified->_currentEnumerator->Current;
        return true;
    }
    return false;
}

static IEnumerator* GetSkipEnumerator(const IEnumerable *This)
{
    const LimitedEnumerable* limited = (const LimitedEnumerable*)This;

    LimitedEnumerator* result = new(LimitedEnumerator);

    *result = (LimitedEnumerator) {
        ._parent = (ModifiedEnumerator) {
            ._parent = (IEnumerator) {
                .MoveNext = SkipMoveNext,
                .Reset = LimitedReset,
                .Dispose = ModifiedDispose,
            },
            ._currentEnumerator = limited->_baseEnumerable->GetEnumerator(limited->_baseEnumerable),
            ._baseEnumerable = (const IEnumerable*)limited,
        },
        .Count = limited->Count
    };

    return (IEnumerator*)result;
}

/// @brief Returns the first count items from a sequence.
/// @param source Enumerable to take items from.
/// @param count Amount of items to take.
/// @return A new enumerable.
IEnumerable* Enumerable_Take(const IEnumerable* source, int count)
{
    LimitedEnumerable* result = new(LimitedEnumerable);

    *result = (LimitedEnumerable) {
        ._parent = (IEnumerable) {
            .GetEnumerator = GetTakeEnumerator
        },
        ._baseEnumerable = source,
        .Count = count
    };

    return (IEnumerable*)result;
}

/// @brief Skips the first count items from a sequence before returning the rest.
/// @param source Enumerable to skip items from.
/// @param count Amount of items to skip.
/// @return A new enumerable.
IEnumerable* Enumerable_Skip(const IEnumerable* source, int count)
{
    LimitedEnumerable* result = new(LimitedEnumerable);

    *result = (LimitedEnumerable) {
        ._parent = (IEnumerable) {
            .GetEnumerator = GetSkipEnumerator
        },
        ._baseEnumerable = source,
        .Count = count
    };

    return (IEnumerable*)result;
}

#pragma endregion

#pragma region Non-Tunnels

object Enumerable_ElementAt(const IEnumerable* source, int index)
{
    IEnumerator* e = source->GetEnumerator(source);
    while(e->MoveNext(e) && index > 0) { index -= 1; }
    object item = e->Current;
    e->Dispose(e);
    return item;
}

/// @brief Determines if any element of a sequence satisfies a condition.
/// @param source The enumerable to check.
/// @param predicate Condition to satisfy.
/// @return True if any element satisfies the condition, false otherwise.
bool Enumerable_Any(const IEnumerable* source, PredicateFunc* predicate)
{
    IEnumerator* e = source->GetEnumerator(source);
    while(e->MoveNext(e)) {
        if (predicate(e->Current)) {
            e->Dispose(e);
            return true;
        }
    }

    e->Dispose(e);
    return false;
}

/// @brief Determines if all elements of a sequence satisfy a condition.
/// @param source The enumerable to check.
/// @param predicate Condition to satisfy.
/// @return True if all elements satisfy the condition, false otherwise.
bool Enumerable_All(const IEnumerable* source, PredicateFunc* predicate)
{
    IEnumerator* e = source->GetEnumerator(source);
    while(e->MoveNext(e)) {
        if (!predicate(e->Current)) {
            e->Dispose(e);
            return false;
        }
    }

    e->Dispose(e);
    return true;
}

/// @brief Applies an accumulator function over a sequence.
/// @param source Enumerable to aggregate.
/// @param aggregate Accumulator function to apply.
/// @return The final accumulation value.
object Enumerable_Aggregate(const IEnumerable* source, AggregateFunc* aggregate)
{
    IEnumerator* e = source->GetEnumerator(source);
    if (!e->MoveNext(e)) {
        e->Dispose(e);
        return NULL;
    }
    object current = e->Current;
    while(e->MoveNext(e)) {
        current = aggregate(current, e->Current);
    }
    e->Dispose(e);
    return current;
}

/// @brief Searches for the given element in a sequence and returns the zero-based index of this element.
/// @param source Enumerable to search in.
/// @param item Item to search for.
/// @return The zero-based index of this item in the sequence.
int Enumerable_IndexOf(const IEnumerable* source, const object item)
{
    IEnumerator* e = source->GetEnumerator(source);
    
    for (int i = 0; e->MoveNext(e); ++i) {
        if (e->Current == item) {
            e->Dispose(e);
            return i;
        }
    }

    e->Dispose(e);
    return -1;
}

/// @brief Returns the first element of the sequence that satisfies the condition.
/// @param source Enumerable to search in.
/// @param predicate Condition to satisfy.
/// @return The first element that satisfies the predicate.
object Enumerable_FirstOrDefault(const IEnumerable* source, PredicateFunc* predicate)
{
    IEnumerator* e = source->GetEnumerator(source);
    while (e->MoveNext(e)) {
        if (predicate(e->Current)) {
            object item = e->Current;
            e->Dispose(e);
            return item;
        }
    }
    return NULL;
}

#pragma endregion