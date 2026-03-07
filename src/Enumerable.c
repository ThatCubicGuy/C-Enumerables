#include "Collections/Enumerable.h"
#include "Defines.h"

#pragma region Helpers

typedef struct compound_enumerator_s {
    struct enumerator_s _parent;
    IEnumerator _currentEnumerator;
    IEnumerable _baseEnumerable;
} *CompoundEnumerator;

static void ModifiedReset(IEnumerator This)
{
    This->Current = NULL;
    CompoundEnumerator e = (CompoundEnumerator)This;
    e->_currentEnumerator->Reset(e->_currentEnumerator);
}

static void ModifiedDispose(IEnumerator This)
{
    CompoundEnumerator e = (CompoundEnumerator)This;
    e->_currentEnumerator->Dispose(e->_currentEnumerator);
    free(This);
}

#pragma endregion

#pragma region Where

typedef const struct where_enumerable_s {
    struct enumerable_s _parent;
    IEnumerable _baseEnumerable;
    PredicateFunc* _filter;
} *WhereEnumerable;

static bool WhereMoveNext(IEnumerator This)
{
    CompoundEnumerator where = (CompoundEnumerator)This;
    while (where->_currentEnumerator->MoveNext(where->_currentEnumerator)) {
        This->Current = where->_currentEnumerator->Current;
        if (((WhereEnumerable)where->_baseEnumerable)->_filter(This->Current)) return true;
    }

    return false;
}

static IEnumerator GetWhereEnumerator(IEnumerable This)
{
    WhereEnumerable where = (WhereEnumerable)This;

    CompoundEnumerator allocinit(compound_enumerator_s, result) {
        ._parent = (struct enumerator_s) {
            .MoveNext = WhereMoveNext,
            .Reset = ModifiedReset,
            .Dispose = ModifiedDispose
        },
        ._currentEnumerator = where->_baseEnumerable->GetEnumerator(where->_baseEnumerable),
        ._baseEnumerable = (IEnumerable)where
    };

    return (IEnumerator)result;
}

/// @brief Filters a sequence based on a predicate.
/// @param source Enumerable to filter.
/// @param filter Condition that the elements must fulfill to be taken from the sequence.
/// @return A new enumerable.
IEnumerable Enumerable_Where(IEnumerable source, PredicateFunc* filter)
{
    WhereEnumerable allocinit(where_enumerable_s, result) {
        ._parent = (struct enumerable_s) {
            .GetEnumerator = GetWhereEnumerator
        },
        ._filter = filter,
        ._baseEnumerable = source
    };
    return (IEnumerable)result;
}

#pragma endregion

#pragma region Select

typedef const struct select_enumerable_s {
    struct enumerable_s _parent;
    IEnumerable _baseEnumerable;
    SelectorFunc* _selector;
} *SelectEnumerable;

static bool SelectMoveNext(IEnumerator This)
{
    CompoundEnumerator select = (CompoundEnumerator)This;
    if (select->_currentEnumerator->MoveNext(select->_currentEnumerator)) {
        This->Current = ((SelectEnumerable)select->_baseEnumerable)->_selector(select->_currentEnumerator->Current);
        return true;
    }

    return false;
}

static IEnumerator GetSelectEnumerator(IEnumerable This)
{
    SelectEnumerable select = (SelectEnumerable)This;

    CompoundEnumerator allocinit(compound_enumerator_s, result) {
        ._parent = (struct enumerator_s) {
            .MoveNext = SelectMoveNext,
            .Reset = ModifiedReset,
            .Dispose = ModifiedDispose
        },
        ._currentEnumerator = select->_baseEnumerable->GetEnumerator(select->_baseEnumerable),
        ._baseEnumerable = (IEnumerable)select
    };

    return (IEnumerator)result;
}

/// @brief Projects each element of a sequence into a new form.
/// @param source Enumerable to project.
/// @param selector Function to apply to each element.
/// @return A new enumerable.
IEnumerable Enumerable_Select(IEnumerable source, SelectorFunc* selector)
{
    SelectEnumerable allocinit(select_enumerable_s, result) {
        ._parent = (struct enumerable_s) {
            .GetEnumerator = GetSelectEnumerator
        },
        ._selector = selector,
        ._baseEnumerable = source
    };
    return (IEnumerable)result;
}

#pragma endregion

#pragma region SelectMany

typedef const struct select_many_enumerable_s {
    struct enumerable_s _parent;
    IEnumerable _baseEnumerable;
    SelectManyFunc* _selector;
} *SelectManyEnumerable;

typedef struct select_many_enumerator_s {
    struct enumerator_s _parent;
    IEnumerable _baseEnumerable;
    IEnumerator _innerEnumerator;
    IEnumerator _outerEnumerator;
} *SelectManyEnumerator;

static bool SelectManyMoveNext(IEnumerator This)
{
    SelectManyEnumerator selectMany = (SelectManyEnumerator)This;

    if (selectMany->_innerEnumerator != NULL && selectMany->_innerEnumerator->MoveNext(selectMany->_innerEnumerator)) {
        This->Current = selectMany->_innerEnumerator->Current;
        return true;
    } else if (selectMany->_outerEnumerator->MoveNext(selectMany->_outerEnumerator)) {
        if (selectMany->_innerEnumerator != NULL) selectMany->_innerEnumerator->Dispose(selectMany->_innerEnumerator);
        IEnumerable result = ((SelectManyEnumerable)selectMany->_baseEnumerable)->_selector(selectMany->_outerEnumerator->Current);
        selectMany->_innerEnumerator = result->GetEnumerator(result);
        return SelectManyMoveNext(This);
    }

    return false;
}

static void SelectManyReset(IEnumerator This)
{
    SelectManyEnumerator selectMany = (SelectManyEnumerator)This;
    selectMany->_innerEnumerator = NULL;
    selectMany->_outerEnumerator->Reset(selectMany->_outerEnumerator);
}

static void SelectManyDispose(IEnumerator This)
{
    SelectManyEnumerator selectMany = (SelectManyEnumerator)This;
    if (selectMany->_innerEnumerator != NULL) selectMany->_innerEnumerator->Dispose(selectMany->_innerEnumerator);
    selectMany->_outerEnumerator->Dispose(selectMany->_outerEnumerator);
    free(This);
}

static IEnumerator GetSelectManyEnumerator(IEnumerable This)
{
    SelectManyEnumerable selectMany = (SelectManyEnumerable)This;

    SelectManyEnumerator allocinit(select_many_enumerator_s, result) {
        ._parent = (struct enumerator_s) {
            .MoveNext = SelectManyMoveNext,
            .Reset = SelectManyReset,
            .Dispose = SelectManyDispose
        },
        ._outerEnumerator = selectMany->_baseEnumerable->GetEnumerator(selectMany->_baseEnumerable),
        ._baseEnumerable = (IEnumerable)selectMany
    };

    return (IEnumerator)result;
}

/// @brief Projects each element of the collection into a new collection, then flattens the result.
/// @param source Source enumerable to project the elements of.
/// @param selector Selector function for the resulting collection.
/// @return A new enumerable.
IEnumerable Enumerable_SelectMany(IEnumerable source, SelectManyFunc* selector)
{
    SelectManyEnumerable allocinit(select_many_enumerable_s, result) {
        ._parent = (struct enumerable_s) {
            .GetEnumerator = GetSelectManyEnumerator,
        },
        ._selector = selector,
        ._baseEnumerable = source
    };
    return (IEnumerable)result;
}

#pragma endregion

#pragma region Take / Skip

typedef const struct limited_enumerable_s {
    struct enumerable_s _parent;
    IEnumerable _baseEnumerable;
    int Count;
} *LimitedEnumerable;

typedef struct limited_enumerator_s {
    struct compound_enumerator_s _parent;
    int Count;
} *LimitedEnumerator;

static void LimitedReset(IEnumerator This)
{
    ((LimitedEnumerator)This)->Count = ((LimitedEnumerable)((CompoundEnumerator)This)->_baseEnumerable)->Count;
    ModifiedReset(This);
}

static bool TakeMoveNext(IEnumerator This)
{
    if (((LimitedEnumerator)This)->Count == 0) return false;
    ((LimitedEnumerator)This)->Count -= 1;
    CompoundEnumerator modified = (CompoundEnumerator)This;
    if (modified->_currentEnumerator->MoveNext(modified->_currentEnumerator)) {
        This->Current = modified->_currentEnumerator->Current;
        return true;
    }
    return false;
}

static IEnumerator GetTakeEnumerator(IEnumerable This)
{
    LimitedEnumerable limited = (LimitedEnumerable)This;

    LimitedEnumerator allocinit(limited_enumerator_s, result) {
        ._parent = (struct compound_enumerator_s) {
            ._parent = (struct enumerator_s) {
                .MoveNext = TakeMoveNext,
                .Reset = LimitedReset,
                .Dispose = ModifiedDispose,
            },
            ._currentEnumerator = limited->_baseEnumerable->GetEnumerator(limited->_baseEnumerable),
            ._baseEnumerable = (IEnumerable)limited,
        },
        .Count = limited->Count
    };

    return (IEnumerator)result;
}

static bool SkipMoveNext(IEnumerator This)
{
    CompoundEnumerator modified = (CompoundEnumerator)This;
    while (((LimitedEnumerator)This)->Count > 0) {
        ((LimitedEnumerator)This)->Count -= 1;
        modified->_currentEnumerator->MoveNext(modified->_currentEnumerator);
    }
    if (modified->_currentEnumerator->MoveNext(modified->_currentEnumerator)) {
        This->Current = modified->_currentEnumerator->Current;
        return true;
    }
    return false;
}

static IEnumerator GetSkipEnumerator(IEnumerable This)
{
    LimitedEnumerable limited = (LimitedEnumerable)This;

    LimitedEnumerator allocinit(limited_enumerator_s, result) {
        ._parent = (struct compound_enumerator_s) {
            ._parent = (struct enumerator_s) {
                .MoveNext = SkipMoveNext,
                .Reset = LimitedReset,
                .Dispose = ModifiedDispose,
            },
            ._currentEnumerator = limited->_baseEnumerable->GetEnumerator(limited->_baseEnumerable),
            ._baseEnumerable = (IEnumerable)limited,
        },
        .Count = limited->Count
    };

    return (IEnumerator)result;
}

/// @brief Returns the first count items from a sequence.
/// @param source Enumerable to take items from.
/// @param count Amount of items to take.
/// @return A new enumerable.
IEnumerable Enumerable_Take(IEnumerable source, int count)
{
    LimitedEnumerable allocinit(limited_enumerable_s, result) {
        ._parent = (struct enumerable_s) {
            .GetEnumerator = GetTakeEnumerator
        },
        ._baseEnumerable = source,
        .Count = count
    };

    return (IEnumerable)result;
}

/// @brief Skips the first count items from a sequence before returning the rest.
/// @param source Enumerable to skip items from.
/// @param count Amount of items to skip.
/// @return A new enumerable.
IEnumerable Enumerable_Skip(IEnumerable source, int count)
{
    LimitedEnumerable allocinit(limited_enumerable_s, result) {
        ._parent = (struct enumerable_s) {
            .GetEnumerator = GetSkipEnumerator
        },
        ._baseEnumerable = source,
        .Count = count
    };

    return (IEnumerable)result;
}

#pragma endregion

#pragma region Append / Prepend

typedef const struct extend_enumerable_s {
    struct enumerable_s _parent;
    IEnumerable _baseEnumerable;
    object _added;
} *ExtendEnumerable;

typedef struct extend_enumerator_s {
    struct compound_enumerator_s _parent;
    bool _hasEnumeratedExtra;
} *ExtendEnumerator;

static void ExtendReset(IEnumerator This)
{
    ((ExtendEnumerator)This)->_hasEnumeratedExtra = false;
    ModifiedReset(This);
}

static bool AppendMoveNext(IEnumerator This)
{
    CompoundEnumerator modified = (CompoundEnumerator)This;
    if (modified->_currentEnumerator->MoveNext(modified->_currentEnumerator)) {
        This->Current = modified->_currentEnumerator->Current;
        return true;
    } else if (!((ExtendEnumerator)This)->_hasEnumeratedExtra) {
        This->Current = ((ExtendEnumerable)modified->_baseEnumerable)->_added;
        ((ExtendEnumerator)This)->_hasEnumeratedExtra = true;
        return true;
    }

    return false;
}

static IEnumerator GetAppendEnumerator(IEnumerable This)
{
    ExtendEnumerable extend = (ExtendEnumerable)This;

    ExtendEnumerator allocinit(extend_enumerator_s, result) {
        ._parent = (struct compound_enumerator_s) {
            ._parent = (struct enumerator_s) {
                .MoveNext = AppendMoveNext,
                .Reset = ExtendReset,
                .Dispose = ModifiedDispose
            },
            ._currentEnumerator = extend->_baseEnumerable->GetEnumerator(extend->_baseEnumerable),
            ._baseEnumerable = (IEnumerable)extend
        },
        ._hasEnumeratedExtra = false
    };

    return (IEnumerator)result;
}

static bool PrependMoveNext(IEnumerator This)
{
    CompoundEnumerator modified = (CompoundEnumerator)This;
    if (!((ExtendEnumerator)This)->_hasEnumeratedExtra) {
        This->Current = ((ExtendEnumerable)modified->_baseEnumerable)->_added;
        ((ExtendEnumerator)This)->_hasEnumeratedExtra = true;
        return true;
    } else if (modified->_currentEnumerator->MoveNext(modified->_currentEnumerator)) {
        This->Current = modified->_currentEnumerator->Current;
        return true;
    }

    return false;
}

static IEnumerator GetPrependEnumerator(IEnumerable This)
{
    ExtendEnumerable extend = (ExtendEnumerable)This;

    ExtendEnumerator allocinit(extend_enumerator_s, result) {
        ._parent = (struct compound_enumerator_s) {
            ._parent = (struct enumerator_s) {
                .MoveNext = PrependMoveNext,
                .Reset = ExtendReset,
                .Dispose = ModifiedDispose
            },
            ._currentEnumerator = extend->_baseEnumerable->GetEnumerator(extend->_baseEnumerable),
            ._baseEnumerable = (IEnumerable)extend
        },
        ._hasEnumeratedExtra = false
    };

    return (IEnumerator)result;
}

/// @brief Appends an element to the end of a sequence.
/// @param source Enumerable to append the item to.
/// @param item Item to append.
/// @return A new enumerable.
IEnumerable Enumerable_Append(IEnumerable source, object item)
{
    ExtendEnumerable allocinit(extend_enumerable_s, result) {
        ._parent = (struct enumerable_s) {
            .GetEnumerator = GetAppendEnumerator
        },
        ._baseEnumerable = source,
        ._added = item
    };

    return (IEnumerable)result;
}

/// @brief Prepends an element to the start of a sequence.
/// @param source Enumerable to prepend the item to.
/// @param item Item to prepend.
/// @return A new enumerable.
IEnumerable Enumerable_Prepend(IEnumerable source, object item)
{
    ExtendEnumerable allocinit(extend_enumerable_s, result) {
        ._parent = (struct enumerable_s) {
            .GetEnumerator = GetPrependEnumerator
        },
        ._baseEnumerable = source,
        ._added = item
    };

    return (IEnumerable)result;
}

#pragma endregion

#pragma region Concat

typedef const struct concat_enumerable_s {
    struct enumerable_s _parent;
    IEnumerable _firstEnumerable;
    IEnumerable _secondEnumerable;
} *ConcatEnumerable;

typedef struct concat_enumerator_s {
    struct compound_enumerator_s _parent;
    bool _startedSecondEnumeration;
} *ConcatEnumerator;

static bool ConcatMoveNext(IEnumerator This)
{
    CompoundEnumerator modified = (CompoundEnumerator)This;
    if (modified->_currentEnumerator->MoveNext(modified->_currentEnumerator)) {
        This->Current = modified->_currentEnumerator->Current;
        return true;
    } else if (!((ConcatEnumerator)This)->_startedSecondEnumeration) {
        IEnumerable secondEnum = ((ConcatEnumerable)modified->_baseEnumerable)->_secondEnumerable;
        modified->_currentEnumerator = secondEnum->GetEnumerator(secondEnum);
        ((ConcatEnumerator)This)->_startedSecondEnumeration = true;
        return ConcatMoveNext(This);
    }

    return false;
}

static void ConcatReset(IEnumerator This)
{
    ((ConcatEnumerator)This)->_startedSecondEnumeration = false;
    ModifiedReset(This);
}

static IEnumerator GetConcatEnumerator(IEnumerable This)
{
    ConcatEnumerable concat = (ConcatEnumerable)This;

    ConcatEnumerator allocinit(concat_enumerator_s, result) {
        ._parent = (struct compound_enumerator_s) {
            ._parent = (struct enumerator_s) {
                .MoveNext = ConcatMoveNext,
                .Reset = ConcatReset,
                .Dispose = ModifiedDispose
            },
            ._currentEnumerator = concat->_firstEnumerable->GetEnumerator(concat->_firstEnumerable),
            ._baseEnumerable = (IEnumerable)concat
        },
        ._startedSecondEnumeration = false
    };

    return (IEnumerator)result;
}

/// @brief Concatenates two sequences.
/// @param first The first sequence to concatenate.
/// @param second The second sequence to concatenate.
/// @return A new enumerable.
IEnumerable Enumerable_Concat(IEnumerable first, IEnumerable second)
{
    ConcatEnumerable allocinit(concat_enumerable_s, result) {
        ._parent = (struct enumerable_s) {
            .GetEnumerator = GetConcatEnumerator
        },
        ._firstEnumerable = first,
        ._secondEnumerable = second
    };

    return (IEnumerable)result;
}

#pragma endregion

#pragma region Non-Tunnels

/// @brief Returns the element at the given zero-based index of the sequence.
/// @param source Enumerable to extract the element from.
/// @param index Index of the element.
/// @return The element at the specified index.
object Enumerable_ElementAt(IEnumerable source, int index)
{
    IEnumerator e = source->GetEnumerator(source);
    while(e->MoveNext(e) && index > 0) { index -= 1; }
    object item = e->Current;
    e->Dispose(e);
    return item;
}

/// @brief Determines if any element of a sequence satisfies a condition.
/// @param source The enumerable to check.
/// @param predicate Condition to satisfy.
/// @return True if any element satisfies the condition, false otherwise.
bool Enumerable_Any(IEnumerable source, PredicateFunc* predicate)
{
    IEnumerator e = source->GetEnumerator(source);
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
bool Enumerable_All(IEnumerable source, PredicateFunc* predicate)
{
    IEnumerator e = source->GetEnumerator(source);
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
object Enumerable_Aggregate(IEnumerable source, AggregateFunc* aggregate)
{
    IEnumerator e = source->GetEnumerator(source);
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
int Enumerable_IndexOf(IEnumerable source, object item)
{
    IEnumerator e = source->GetEnumerator(source);
    
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
object Enumerable_FirstOrDefault(IEnumerable source, PredicateFunc* predicate)
{
    IEnumerator e = source->GetEnumerator(source);
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
void Enumerable_ForEach(IEnumerable source, Action* action)
{
    IEnumerator e = source->GetEnumerator(source);
    while (e->MoveNext(e)) action(e->Current);
    e->Dispose(e);
}

/// @brief Determines whether a sequence contains a specified element.
/// @param source Enumerable to search in.
/// @param item Item to search for.
bool Enumerable_Contains(IEnumerable source, object item)
{
    IEnumerator e = source->GetEnumerator(source);
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
int Enumerable_Count(IEnumerable source)
{
    IEnumerator e = source->GetEnumerator(source);
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
bool Enumerable_SequenceEqual(IEnumerable first, IEnumerable second)
{
    IEnumerator e1 = first->GetEnumerator(first);
    IEnumerator e2 = second->GetEnumerator(second);
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