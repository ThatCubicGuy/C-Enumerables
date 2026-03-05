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
    This->Current = NULL;
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

    ModifiedEnumerator* result = bare(ModifiedEnumerator);

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

/// @brief Filters a sequence based on a predicate.
/// @param source Enumerable to filter.
/// @param filter Condition that the elements must fulfill to be taken from the sequence.
/// @return A new enumerable.
IEnumerable* Enumerable_Where(const IEnumerable* source, PredicateFunc* filter)
{
    WhereEnumerable* result = bare(WhereEnumerable);
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

    ModifiedEnumerator* result = bare(ModifiedEnumerator);

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
    SelectEnumerable* result = bare(SelectEnumerable);
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

    SelectManyEnumerator* result = bare(SelectManyEnumerator);

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
    SelectManyEnumerable* result = bare(SelectManyEnumerable);
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

    LimitedEnumerator* result = bare(LimitedEnumerator);

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

    LimitedEnumerator* result = bare(LimitedEnumerator);

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
    LimitedEnumerable* result = bare(LimitedEnumerable);

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
    LimitedEnumerable* result = bare(LimitedEnumerable);

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

#pragma region Append / Prepend

typedef struct extended_enumerable_s {
    IEnumerable _parent;
    const IEnumerable* _baseEnumerable;
    object _added;
} ExtendEnumerable;

typedef struct extended_enumerator_s {
    ModifiedEnumerator _parent;
    bool _hasEnumeratedExtra;
} ExtendEnumerator;

static void ExtendReset(IEnumerator *This)
{
    ((ExtendEnumerator*)This)->_hasEnumeratedExtra = false;
    ModifiedReset(This);
}

static bool AppendMoveNext(IEnumerator *This)
{
    ModifiedEnumerator* modified = (ModifiedEnumerator*)This;
    if (modified->_currentEnumerator->MoveNext(modified->_currentEnumerator)) {
        This->Current = modified->_currentEnumerator->Current;
        return true;
    } else if (!((ExtendEnumerator*)This)->_hasEnumeratedExtra) {
        This->Current = ((ExtendEnumerable*)modified->_baseEnumerable)->_added;
        ((ExtendEnumerator*)This)->_hasEnumeratedExtra = true;
        return true;
    }

    return false;
}

static IEnumerator* GetAppendEnumerator(const IEnumerable *This)
{
    const ExtendEnumerable* extend = (const ExtendEnumerable*)This;

    ExtendEnumerator* result = bare(ExtendEnumerator);

    *result = (ExtendEnumerator) {
        ._parent = (ModifiedEnumerator) {
            ._parent = (IEnumerator) {
                .MoveNext = AppendMoveNext,
                .Reset = ExtendReset,
                .Dispose = ModifiedDispose
            },
            ._currentEnumerator = extend->_baseEnumerable->GetEnumerator(extend->_baseEnumerable),
            ._baseEnumerable = (const IEnumerable*)extend
        },
        ._hasEnumeratedExtra = false
    };

    return (IEnumerator*)result;
}

static bool PrependMoveNext(IEnumerator *This)
{
    ModifiedEnumerator* modified = (ModifiedEnumerator*)This;
    if (!((ExtendEnumerator*)This)->_hasEnumeratedExtra) {
        This->Current = ((ExtendEnumerable*)modified->_baseEnumerable)->_added;
        ((ExtendEnumerator*)This)->_hasEnumeratedExtra = true;
        return true;
    } else if (modified->_currentEnumerator->MoveNext(modified->_currentEnumerator)) {
        This->Current = modified->_currentEnumerator->Current;
        return true;
    }

    return false;
}

static IEnumerator* GetPrependEnumerator(const IEnumerable *This)
{
    const ExtendEnumerable* extend = (const ExtendEnumerable*)This;

    ExtendEnumerator* result = bare(ExtendEnumerator);

    *result = (ExtendEnumerator) {
        ._parent = (ModifiedEnumerator) {
            ._parent = (IEnumerator) {
                .MoveNext = PrependMoveNext,
                .Reset = ExtendReset,
                .Dispose = ModifiedDispose
            },
            ._currentEnumerator = extend->_baseEnumerable->GetEnumerator(extend->_baseEnumerable),
            ._baseEnumerable = (const IEnumerable*)extend
        },
        ._hasEnumeratedExtra = false
    };

    return (IEnumerator*)result;
}

/// @brief Appends an element to the end of a sequence.
/// @param source Enumerable to append the item to.
/// @param item Item to append.
/// @return A new enumerable.
IEnumerable* Enumerable_Append(const IEnumerable* source, object item)
{
    ExtendEnumerable* result = bare(ExtendEnumerable);

    *result = (ExtendEnumerable) {
        ._parent = (IEnumerable) {
            .GetEnumerator = GetAppendEnumerator
        },
        ._baseEnumerable = source,
        ._added = item
    };

    return (IEnumerable*)result;
}

/// @brief Prepends an element to the start of a sequence.
/// @param source Enumerable to prepend the item to.
/// @param item Item to prepend.
/// @return A new enumerable.
IEnumerable* Enumerable_Prepend(const IEnumerable* source, object item)
{
    ExtendEnumerable* result = bare(ExtendEnumerable);

    *result = (ExtendEnumerable) {
        ._parent = (IEnumerable) {
            .GetEnumerator = GetPrependEnumerator
        },
        ._baseEnumerable = source,
        ._added = item
    };

    return (IEnumerable*)result;
}

#pragma endregion

#pragma region Concat

typedef struct concat_enumerable_s {
    IEnumerable _parent;
    const IEnumerable* _firstEnumerable;
    const IEnumerable* _secondEnumerable;
} ConcatEnumerable;

typedef struct concat_enumerator_s {
    ModifiedEnumerator _parent;
    bool _startedSecondEnumeration;
} ConcatEnumerator;

static bool ConcatMoveNext(IEnumerator *This)
{
    ModifiedEnumerator* modified = (ModifiedEnumerator*)This;
    if (modified->_currentEnumerator->MoveNext(modified->_currentEnumerator)) {
        This->Current = modified->_currentEnumerator->Current;
        return true;
    } else if (!((ConcatEnumerator*)This)->_startedSecondEnumeration) {
        const IEnumerable* secondEnum = ((ConcatEnumerable*)modified->_baseEnumerable)->_secondEnumerable;
        modified->_currentEnumerator = secondEnum->GetEnumerator(secondEnum);
        ((ConcatEnumerator*)This)->_startedSecondEnumeration = true;
        return ConcatMoveNext(This);
    }

    return false;
}

static void ConcatReset(IEnumerator *This)
{
    ((ConcatEnumerator*)This)->_startedSecondEnumeration = false;
    ModifiedReset(This);
}

static IEnumerator* GetConcatEnumerator(const IEnumerable *This)
{
    const ConcatEnumerable* concat = (const ConcatEnumerable*)This;

    ConcatEnumerator* result = bare(ConcatEnumerator);

    *result = (ConcatEnumerator) {
        ._parent = (ModifiedEnumerator) {
            ._parent = (IEnumerator) {
                .MoveNext = ConcatMoveNext,
                .Reset = ConcatReset,
                .Dispose = ModifiedDispose
            },
            ._currentEnumerator = concat->_firstEnumerable->GetEnumerator(concat->_firstEnumerable),
            ._baseEnumerable = (const IEnumerable*)concat
        },
        ._startedSecondEnumeration = false
    };

    return (IEnumerator*)result;
}

/// @brief Concatenates two sequences.
/// @param first The first sequence to concatenate.
/// @param second The second sequence to concatenate.
/// @return A new enumerable.
IEnumerable* Enumerable_Concat(const IEnumerable* first, const IEnumerable* second)
{
    ConcatEnumerable* result = bare(ConcatEnumerable);

    *result = (ConcatEnumerable) {
        ._parent = (IEnumerable) {
            .GetEnumerator = GetConcatEnumerator
        },
        ._firstEnumerable = first,
        ._secondEnumerable = second
    };

    return (IEnumerable*)result;
}

#pragma endregion

#pragma region Non-Tunnels

/// @brief Returns the element at the given zero-based index of the sequence.
/// @param source Enumerable to extract the element from.
/// @param index Index of the element.
/// @return The element at the specified index.
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
int Enumerable_IndexOf(const IEnumerable* source, object item)
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

    e->Dispose(e);
    return NULL;
}

/// @brief Performs an action on each element of a collection.
/// @param source Enumerable to execute the action on.
/// @param action Action to execute on each item.
void Enumerable_ForEach(const IEnumerable* source, Action* action)
{
    IEnumerator* e = source->GetEnumerator(source);
    while (e->MoveNext(e)) action(e->Current);
    e->Dispose(e);
}

/// @brief Determines whether a sequence contains a specified element.
/// @param source Enumerable to search in.
/// @param item Item to search for.
bool Enumerable_Contains(const IEnumerable* source, object item)
{
    IEnumerator* e = source->GetEnumerator(source);
    while (e->MoveNext(e)) {
        if (e->Current == item) {
            e->Dispose(e);
            return true;
        }
    }

    e->Dispose(e);
    return false;
}

/// @brief Returns a number that represents the amount of elements in the specified enumerable.
/// @param source Enumerable whose length to count.
/// @return A number representing the amount of items in source.
int Enumerable_Count(const IEnumerable* source)
{
    IEnumerator* e = source->GetEnumerator(source);
    int i = 0;
    while (e->MoveNext(e)) {
        i += 1;
    }
    e->Dispose(e);
    return i;
}

/// @brief Determines whether two sequences are equal by comparing each element individually.
/// @param first First enumerable to compare.
/// @param second Second enumerable to compare.
/// @return True if every element from first is equal to second, false otherwise.
bool Enumerable_SequenceEqual(const IEnumerable* first, const IEnumerable* second)
{
    IEnumerator* e1 = first->GetEnumerator(first);
    IEnumerator* e2 = second->GetEnumerator(second);
    while (e1->MoveNext(e1) && e2->MoveNext(e2)) {
        if (e1->Current != e2->Current) {
            e1->Dispose(e1);
            e2->Dispose(e2);
            return false;
        }
    }

    if (e1->MoveNext(e1) || e2->MoveNext(e2)) {
        e1->Dispose(e1);
        e2->Dispose(e2);
        return false;
    }

    e1->Dispose(e1);
    e2->Dispose(e2);
    return true;
}

#pragma endregion