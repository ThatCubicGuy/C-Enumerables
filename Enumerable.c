#include "Enumerable.h"
#include "Defines.h"
#include "List.h"
#include "LinkedList.h"

bool Enumerator_MoveNext(IEnumerator enumerator)
{
    return enumerator.MoveNext(&enumerator);
}

#pragma region Select

typedef struct select_enumerable_s {
    IEnumerable _parent;
    IEnumerable* _baseEnumerable;
    SelectorFunc* _selector;
} SelectEnumerable;

typedef struct select_enumerator_s {
    IEnumerator _parent;
    IEnumerator *_baseEnumerator;
    SelectorFunc* _selector;
} SelectEnumerator;

static bool SelectMoveNext(IEnumerator *This)
{
    SelectEnumerator* SE = (SelectEnumerator*)This;
    if (SE->_baseEnumerator->MoveNext(SE->_baseEnumerator)) {
        This->Current = SE->_selector(SE->_baseEnumerator->Current);
        return true;
    }

    return false;
}

static IEnumerator* GetSelectEnumerator(IEnumerable *This)
{
    SelectEnumerable *SE = (SelectEnumerable*)This;
    SelectEnumerator* result = new(SelectEnumerator);
    IEnumerator* originalEnum = SE->_baseEnumerable->GetEnumerator(SE->_baseEnumerable);
    *result = (SelectEnumerator) {
        ._parent = (IEnumerator) {
            .MoveNext = SelectMoveNext,
            .Reset = originalEnum->Reset,
            .Current = originalEnum->Current
        },
        ._baseEnumerator = originalEnum,
        ._selector = SE->_selector
    };
    return (IEnumerator*)result;
}

IEnumerable* Enumerable_Select(IEnumerable* source, SelectorFunc* selector)
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

#pragma region Where

typedef struct where_enumerable_s {
    IEnumerable _parent;
    IEnumerable* _baseEnumerable;
    FilterFunc* _filter;
} WhereEnumerable;

typedef struct where_enumerator_s {
    IEnumerator _parent;
    IEnumerator *_baseEnumerator;
    FilterFunc* _filter;
} WhereEnumerator;

static bool WhereMoveNext(IEnumerator *This)
{
    WhereEnumerator* WE = (WhereEnumerator*)This;
    while (WE->_baseEnumerator->MoveNext(WE->_baseEnumerator)) {
        This->Current = WE->_baseEnumerator->Current;
        if (!WE->_filter(This->Current)) continue;
        return true;
    }
    return false;
}

static IEnumerator* GetWhereEnumerator(IEnumerable *This)
{
    WhereEnumerable *WE = (WhereEnumerable*)This;
    WhereEnumerator* result = new(WhereEnumerator);
    IEnumerator* originalEnum = WE->_baseEnumerable->GetEnumerator(WE->_baseEnumerable);
    *result = (WhereEnumerator) {
        ._parent = (IEnumerator) {
            .MoveNext = WhereMoveNext,
            .Reset = originalEnum->Reset,
            .Current = originalEnum->Current
        },
        ._baseEnumerator = originalEnum,
        ._filter = WE->_filter
    };
    return (IEnumerator*)result;
}

IEnumerable* Enumerable_Where(IEnumerable* source, FilterFunc* filter)
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
