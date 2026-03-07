#ifndef COLLECTIONS_GENERIC_ENUMERABLE_IMPLEMENTATIONS
#define COLLECTIONS_GENERIC_ENUMERABLE_IMPLEMENTATIONS
#include "Collections/Generic/EnumerableT.h"

#pragma region Implement

#define ENUMERABLE_IMPLEMENT(T)                                                                         \
typedef IEnumerator_##T GetEnumerator_##T(IEnumerable_##T This);                                        \
typedef struct generic_compound_enumerator_##T##_s {                                                    \
    struct generic_enumerator_##T##_s _parent;                                                          \
    IEnumerator_##T _currentEnumerator;                                                                 \
    IEnumerable_##T _baseEnumerable;                                                                    \
} *CompoundEnumerator_##T;                                                                              \
static void CompoundReset_##T(IEnumerator_##T This)                                                     \
{                                                                                                       \
    This->Current = default(T);                                                                         \
    CompoundEnumerator_##T e = (CompoundEnumerator_##T)This;                                            \
    e->_currentEnumerator->Reset(e->_currentEnumerator);                                                \
}                                                                                                       \
static void CompoundDispose_##T(IEnumerator_##T This)                                                   \
{                                                                                                       \
    CompoundEnumerator_##T e = (CompoundEnumerator_##T)This;                                            \
    e->_currentEnumerator->Dispose(e->_currentEnumerator);                                              \
    free(This);                                                                                         \
}                                                                                                       \
typedef const struct generic_where_enumerable_##T##_s {                                                 \
    struct generic_enumerable_##T##_s _parent;                                                          \
    IEnumerable_##T _baseEnumerable;                                                                    \
    bool (*_filter)(T);                                                                                 \
} *WhereEnumerable_##T;                                                                                 \
static bool WhereMoveNext_##T(IEnumerator_##T This)                                                     \
{                                                                                                       \
    CompoundEnumerator_##T where = (CompoundEnumerator_##T)This;                                        \
    while (where->_currentEnumerator->MoveNext(where->_currentEnumerator)) {                            \
        This->Current = where->_currentEnumerator->Current;                                             \
        if (((WhereEnumerable_##T)where->_baseEnumerable)->_filter(This->Current)) {                    \
            return true;                                                                                \
        }                                                                                               \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static IEnumerator_##T GetWhereEnumerator_##T(IEnumerable_##T This)                                     \
{                                                                                                       \
    WhereEnumerable_##T where = (WhereEnumerable_##T)This;                                              \
    CompoundEnumerator_##T result = alloc(struct generic_compound_enumerator_##T##_s);                  \
    init(struct generic_compound_enumerator_##T##_s, result) {                                          \
        ._parent = (struct generic_enumerator_##T##_s) {                                                \
            .MoveNext = WhereMoveNext_##T,                                                              \
            .Reset = CompoundReset_##T,                                                                 \
            .Dispose = CompoundDispose_##T                                                              \
        },                                                                                              \
        ._currentEnumerator = where->_baseEnumerable->GetEnumerator(where->_baseEnumerable),            \
        ._baseEnumerable = (IEnumerable_##T)where                                                       \
    };                                                                                                  \
    return (IEnumerator_##T)result;                                                                     \
}                                                                                                       \
\
IEnumerable_##T Enumerable_##T##_Where(IEnumerable_##T source, bool (*filter)(T))                       \
{                                                                                                       \
    WhereEnumerable_##T result = alloc(struct generic_where_enumerable_##T##_s);                        \
    init(struct generic_where_enumerable_##T##_s, result) {                                             \
        ._parent = (struct generic_enumerable_##T##_s) {                                                \
            .GetEnumerator = GetWhereEnumerator_##T                                                     \
        },                                                                                              \
        ._baseEnumerable = source,                                                                      \
        ._filter = filter                                                                               \
    };                                                                                                  \
    return (IEnumerable_##T)result;                                                                     \
}                                                                                                       \
\
typedef const struct generic_limited_enumerable_##T##_s {                                               \
    struct generic_enumerable_##T##_s _parent;                                                          \
    IEnumerable_##T _baseEnumerable;                                                                    \
    int Count;                                                                                          \
} *LimitedEnumerable_##T;                                                                               \
typedef struct generic_limited_enumerator_##T##_s {                                                     \
    struct generic_compound_enumerator_##T##_s _parent;                                                 \
    int Count;                                                                                          \
} *LimitedEnumerator_##T;                                                                               \
static void LimitedReset_##T(IEnumerator_##T This)                                                      \
{                                                                                                       \
    ((LimitedEnumerator_##T)This)->Count =                                                              \
        ((LimitedEnumerable_##T)((CompoundEnumerator_##T)This)->_baseEnumerable)->Count;                \
    CompoundReset_##T(This);                                                                            \
}                                                                                                       \
static bool TakeMoveNext_##T(IEnumerator_##T This)                                                      \
{                                                                                                       \
    if (((LimitedEnumerator_##T)This)->Count == 0) return false;                                        \
    ((LimitedEnumerator_##T)This)->Count -= 1;                                                          \
    CompoundEnumerator_##T compound = (CompoundEnumerator_##T)This;                                     \
    if (compound->_currentEnumerator->MoveNext(compound->_currentEnumerator)) {                         \
        This->Current = compound->_currentEnumerator->Current;                                          \
        return true;                                                                                    \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static IEnumerator_##T GetTakeEnumerator_##T(IEnumerable_##T This)                                      \
{                                                                                                       \
    LimitedEnumerable_##T limited = (LimitedEnumerable_##T)This;                                        \
    LimitedEnumerator_##T result = alloc(struct generic_limited_enumerator_##T##_s);                    \
    init(struct generic_limited_enumerator_##T##_s, result) {                                           \
        ._parent = (struct generic_compound_enumerator_##T##_s) {                                       \
            ._parent = (struct generic_enumerator_##T##_s) {                                            \
                .MoveNext = TakeMoveNext_##T,                                                           \
                .Reset = LimitedReset_##T,                                                              \
                .Dispose = CompoundDispose_##T,                                                         \
            },                                                                                          \
            ._currentEnumerator = limited->_baseEnumerable->                                            \
                GetEnumerator(limited->_baseEnumerable),                                                \
            ._baseEnumerable = (IEnumerable_##T)limited,                                                \
        },                                                                                              \
        .Count = limited->Count                                                                         \
    };                                                                                                  \
    return (IEnumerator_##T)result;                                                                     \
}                                                                                                       \
static bool SkipMoveNext_##T(IEnumerator_##T This)                                                      \
{                                                                                                       \
    CompoundEnumerator_##T compound = (CompoundEnumerator_##T)This;                                     \
    while (((LimitedEnumerator_##T)This)->Count > 0) {                                                  \
        ((LimitedEnumerator_##T)This)->Count -= 1;                                                      \
        compound->_currentEnumerator->MoveNext(compound->_currentEnumerator);                           \
    }                                                                                                   \
    if (compound->_currentEnumerator->MoveNext(compound->_currentEnumerator)) {                         \
        This->Current = compound->_currentEnumerator->Current;                                          \
        return true;                                                                                    \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static IEnumerator_##T GetSkipEnumerator_##T(IEnumerable_##T This)                                      \
{                                                                                                       \
    LimitedEnumerable_##T limited = (LimitedEnumerable_##T)This;                                        \
    LimitedEnumerator_##T result = alloc(struct generic_limited_enumerator_##T##_s);                    \
    init(struct generic_limited_enumerator_##T##_s, result) {                                           \
        ._parent = (struct generic_compound_enumerator_##T##_s) {                                       \
            ._parent = (struct generic_enumerator_##T##_s) {                                            \
                .MoveNext = SkipMoveNext_##T,                                                           \
                .Reset = LimitedReset_##T,                                                              \
                .Dispose = CompoundDispose_##T,                                                         \
            },                                                                                          \
            ._currentEnumerator = limited->_baseEnumerable->                                            \
                GetEnumerator(limited->_baseEnumerable),                                                \
            ._baseEnumerable = (IEnumerable_##T)limited,                                                \
        },                                                                                              \
        .Count = limited->Count                                                                         \
    };                                                                                                  \
    return (IEnumerator_##T)result;                                                                     \
}                                                                                                       \
\
IEnumerable_##T Enumerable_##T##_Take(IEnumerable_##T source, int count)                                \
{                                                                                                       \
    LimitedEnumerable_##T result = alloc(struct generic_limited_enumerable_##T##_s);                    \
    init(struct generic_limited_enumerable_##T##_s, result) {                                           \
        ._parent = (struct generic_enumerable_##T##_s) {                                                \
            .GetEnumerator = GetTakeEnumerator_##T                                                      \
        },                                                                                              \
        ._baseEnumerable = source,                                                                      \
        .Count = count                                                                                  \
    };                                                                                                  \
    return (IEnumerable_##T)result;                                                                     \
}                                                                                                       \
\
IEnumerable_##T Enumerable_##T##_Skip(IEnumerable_##T source, int count)                                \
{                                                                                                       \
    LimitedEnumerable_##T result = alloc(struct generic_limited_enumerable_##T##_s);                    \
    init(struct generic_limited_enumerable_##T##_s, result) {                                           \
        ._parent = (struct generic_enumerable_##T##_s) {                                                \
            .GetEnumerator = GetSkipEnumerator_##T                                                      \
        },                                                                                              \
        ._baseEnumerable = source,                                                                      \
        .Count = count                                                                                  \
    };                                                                                                  \
    return (IEnumerable_##T)result;                                                                     \
}                                                                                                       \
typedef const struct generic_extend_enumerable_##T##_s {                                                \
    struct generic_enumerable_##T##_s _parent;                                                          \
    IEnumerable_##T _baseEnumerable;                                                                    \
    T _added;                                                                                           \
} *ExtendEnumerable_##T;                                                                                \
typedef struct generic_extend_enumerator_##T##_s {                                                      \
    struct generic_compound_enumerator_##T##_s _parent;                                                 \
    bool _hasEnumeratedExtra;                                                                           \
} *ExtendEnumerator_##T;                                                                                \
static void ExtendReset_##T(IEnumerator_##T This)                                                       \
{                                                                                                       \
    ((ExtendEnumerator_##T)This)->_hasEnumeratedExtra = false;                                          \
    CompoundReset_##T(This);                                                                            \
}                                                                                                       \
static bool AppendMoveNext_##T(IEnumerator_##T This)                                                    \
{                                                                                                       \
    CompoundEnumerator_##T compound = (CompoundEnumerator_##T)This;                                     \
    if (compound->_currentEnumerator->MoveNext(compound->_currentEnumerator)) {                         \
        This->Current = compound->_currentEnumerator->Current;                                          \
        return true;                                                                                    \
    } else if (!((ExtendEnumerator_##T)This)->_hasEnumeratedExtra) {                                    \
        This->Current = ((ExtendEnumerable_##T)compound->_baseEnumerable)->_added;                      \
        ((ExtendEnumerator_##T)This)->_hasEnumeratedExtra = true;                                       \
        return true;                                                                                    \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static IEnumerator_##T GetAppendEnumerator_##T(IEnumerable_##T This)                                    \
{                                                                                                       \
    ExtendEnumerable_##T extend = (ExtendEnumerable_##T)This;                                           \
    ExtendEnumerator_##T result = alloc(struct generic_extend_enumerator_##T##_s);                      \
    init(struct generic_extend_enumerator_##T##_s, result) {                                            \
        ._parent = (struct generic_compound_enumerator_##T##_s) {                                       \
            ._parent = (struct generic_enumerator_##T##_s) {                                            \
                .MoveNext = AppendMoveNext_##T,                                                         \
                .Reset = ExtendReset_##T,                                                               \
                .Dispose = CompoundDispose_##T                                                          \
            },                                                                                          \
            ._currentEnumerator = extend->_baseEnumerable->                                             \
                GetEnumerator(extend->_baseEnumerable),                                                 \
            ._baseEnumerable = (IEnumerable_##T)extend                                                  \
        },                                                                                              \
        ._hasEnumeratedExtra = false                                                                    \
    };                                                                                                  \
    return (IEnumerator_##T)result;                                                                     \
}                                                                                                       \
static bool PrependMoveNext_##T(IEnumerator_##T This)                                                   \
{                                                                                                       \
    CompoundEnumerator_##T compound = (CompoundEnumerator_##T)This;                                     \
    if (!((ExtendEnumerator_##T)This)->_hasEnumeratedExtra) {                                           \
        This->Current = ((ExtendEnumerable_##T)compound->_baseEnumerable)->_added;                      \
        ((ExtendEnumerator_##T)This)->_hasEnumeratedExtra = true;                                       \
        return true;                                                                                    \
    } else if (compound->_currentEnumerator->MoveNext(compound->_currentEnumerator)) {                  \
        This->Current = compound->_currentEnumerator->Current;                                          \
        return true;                                                                                    \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static IEnumerator_##T GetPrependEnumerator_##T(IEnumerable_##T This)                                   \
{                                                                                                       \
    ExtendEnumerable_##T extend = (ExtendEnumerable_##T)This;                                           \
    ExtendEnumerator_##T result = alloc(struct generic_extend_enumerator_##T##_s);                      \
    init(struct generic_extend_enumerator_##T##_s, result) {                                            \
        ._parent = (struct generic_compound_enumerator_##T##_s) {                                       \
            ._parent = (struct generic_enumerator_##T##_s) {                                            \
                .MoveNext = PrependMoveNext_##T,                                                        \
                .Reset = ExtendReset_##T,                                                               \
                .Dispose = CompoundDispose_##T                                                          \
            },                                                                                          \
            ._currentEnumerator = extend->_baseEnumerable->                                             \
                GetEnumerator(extend->_baseEnumerable),                                                 \
            ._baseEnumerable = (IEnumerable_##T)extend                                                  \
        },                                                                                              \
        ._hasEnumeratedExtra = false                                                                    \
    };                                                                                                  \
    return (IEnumerator_##T)result;                                                                     \
}                                                                                                       \
\
IEnumerable_##T Enumerable_##T##_Append(IEnumerable_##T source, T item)                                 \
{                                                                                                       \
    ExtendEnumerable_##T result = alloc(struct generic_extend_enumerable_##T##_s);                      \
    init(struct generic_extend_enumerable_##T##_s, result) {                                            \
        ._parent = (struct generic_enumerable_##T##_s) {                                                \
            .GetEnumerator = GetAppendEnumerator_##T                                                    \
        },                                                                                              \
        ._baseEnumerable = source,                                                                      \
        ._added = item                                                                                  \
    };                                                                                                  \
    return (IEnumerable_##T)result;                                                                     \
}                                                                                                       \
\
IEnumerable_##T Enumerable_##T##_Prepend(IEnumerable_##T source, T item)                                \
{                                                                                                       \
    ExtendEnumerable_##T result = alloc(struct generic_extend_enumerable_##T##_s);                      \
    init(struct generic_extend_enumerable_##T##_s, result) {                                            \
        ._parent = (struct generic_enumerable_##T##_s) {                                                \
            .GetEnumerator = GetPrependEnumerator_##T                                                   \
        },                                                                                              \
        ._baseEnumerable = source,                                                                      \
        ._added = item                                                                                  \
    };                                                                                                  \
    return (IEnumerable_##T)result;                                                                     \
}                                                                                                       \
\
typedef const struct generic_concat_enumerable_##T##_s {                                                \
    struct generic_enumerable_##T##_s _parent;                                                          \
    IEnumerable_##T _firstEnumerable;                                                                   \
    IEnumerable_##T _secondEnumerable;                                                                  \
} *ConcatEnumerable_##T;                                                                                \
typedef struct generic_concat_enumerator_##T##_s {                                                      \
    struct generic_compound_enumerator_##T##_s _parent;                                                 \
    bool _startedSecondEnumeration;                                                                     \
} *ConcatEnumerator_##T;                                                                                \
static bool ConcatMoveNext_##T(IEnumerator_##T This)                                                    \
{                                                                                                       \
    CompoundEnumerator_##T compound = (CompoundEnumerator_##T)This;                                     \
    if (compound->_currentEnumerator->MoveNext(compound->_currentEnumerator)) {                         \
        This->Current = compound->_currentEnumerator->Current;                                          \
        return true;                                                                                    \
    } else if (!((ConcatEnumerator_##T)This)->_startedSecondEnumeration) {                              \
        IEnumerable_##T secondEnum =                                                                    \
            ((ConcatEnumerable_##T)compound->_baseEnumerable)->_secondEnumerable;                       \
        compound->_currentEnumerator = secondEnum->GetEnumerator(secondEnum);                           \
        ((ConcatEnumerator_##T)This)->_startedSecondEnumeration = true;                                 \
        return ConcatMoveNext_##T(This);                                                                \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static void ConcatReset_##T(IEnumerator_##T This)                                                       \
{                                                                                                       \
    ((ConcatEnumerator_##T)This)->_startedSecondEnumeration = false;                                    \
    CompoundReset_##T(This);                                                                            \
}                                                                                                       \
static IEnumerator_##T GetConcatEnumerator_##T(IEnumerable_##T This)                                    \
{                                                                                                       \
    ConcatEnumerable_##T concat = (ConcatEnumerable_##T)This;                                           \
    ConcatEnumerator_##T result = alloc(struct generic_concat_enumerator_##T##_s);                      \
    init(struct generic_concat_enumerator_##T##_s, result) {                                            \
        ._parent = (struct generic_compound_enumerator_##T##_s) {                                       \
            ._parent = (struct generic_enumerator_##T##_s) {                                            \
                .MoveNext = ConcatMoveNext_##T,                                                         \
                .Reset = ConcatReset_##T,                                                               \
                .Dispose = CompoundDispose_##T                                                          \
            },                                                                                          \
            ._currentEnumerator = concat->_firstEnumerable->                                            \
                GetEnumerator(concat->_firstEnumerable),                                                \
            ._baseEnumerable = (IEnumerable_##T)concat                                                  \
        },                                                                                              \
        ._startedSecondEnumeration = false                                                              \
    };                                                                                                  \
    return (IEnumerator_##T)result;                                                                     \
}                                                                                                       \
\
IEnumerable_##T Enumerable_##T##_Concat(IEnumerable_##T first, IEnumerable_##T second)                  \
{                                                                                                       \
    ConcatEnumerable_##T result = alloc(struct generic_concat_enumerable_##T##_s);                      \
    init(struct generic_concat_enumerable_##T##_s, result) {                                            \
        ._parent = (struct generic_enumerable_##T##_s) {                                                \
            .GetEnumerator = GetConcatEnumerator_##T                                                    \
        },                                                                                              \
        ._firstEnumerable = first,                                                                      \
        ._secondEnumerable = second                                                                     \
    };                                                                                                  \
    return (IEnumerable_##T)result;                                                                     \
}                                                                                                       \
\
T Enumerable_##T##_ElementAt(IEnumerable_##T source, int index)                                         \
{                                                                                                       \
    IEnumerator_##T e = source->GetEnumerator(source);                                                  \
    while(e->MoveNext(e) && index > 0) { index -= 1; }                                                  \
    T item = e->Current;                                                                                \
    e->Dispose(e);                                                                                      \
    return item;                                                                                        \
}                                                                                                       \
\
bool Enumerable_##T##_Any(IEnumerable_##T source, bool (*predicate)(T))                                 \
{                                                                                                       \
    IEnumerator_##T e = source->GetEnumerator(source);                                                  \
    while(e->MoveNext(e)) {                                                                             \
        if (predicate(e->Current)) {                                                                    \
            e->Dispose(e);                                                                              \
            return true;                                                                                \
        }                                                                                               \
    }                                                                                                   \
    e->Dispose(e);                                                                                      \
    return false;                                                                                       \
}                                                                                                       \
\
bool Enumerable_##T##_All(IEnumerable_##T source, bool (*predicate)(T))                                 \
{                                                                                                       \
    IEnumerator_##T e = source->GetEnumerator(source);                                                  \
    while(e->MoveNext(e)) {                                                                             \
        if (!predicate(e->Current)) {                                                                   \
            e->Dispose(e);                                                                              \
            return false;                                                                               \
        }                                                                                               \
    }                                                                                                   \
    e->Dispose(e);                                                                                      \
    return true;                                                                                        \
}                                                                                                       \
\
int Enumerable_##T##_IndexOf(IEnumerable_##T source, T item)                                            \
{                                                                                                       \
    IEnumerator_##T e = source->GetEnumerator(source);                                                  \
    for (int i = 0; e->MoveNext(e); ++i) {                                                              \
        if (ValueEquator(sizeof(T), &e->Current, &item)) {                                              \
            e->Dispose(e);                                                                              \
            return i;                                                                                   \
        }                                                                                               \
    }                                                                                                   \
    e->Dispose(e);                                                                                      \
    return -1;                                                                                          \
}                                                                                                       \
\
T Enumerable_##T##_FirstOrDefault(IEnumerable_##T source, bool (*predicate)(T))                         \
{                                                                                                       \
    IEnumerator_##T e = source->GetEnumerator(source);                                                  \
    while (e->MoveNext(e)) {                                                                            \
        if (predicate(e->Current)) {                                                                    \
            T item = e->Current;                                                                        \
            e->Dispose(e);                                                                              \
            return item;                                                                                \
        }                                                                                               \
    }                                                                                                   \
    e->Dispose(e);                                                                                      \
    return default(T);                                                                                  \
}                                                                                                       \
\
bool Enumerable_##T##_Contains(IEnumerable_##T source, T item)                                          \
{                                                                                                       \
    IEnumerator_##T e = source->GetEnumerator(source);                                                  \
    while (e->MoveNext(e)) {                                                                            \
        if (ValueEquator(sizeof(T), &e->Current, &item)) {                                              \
            e->Dispose(e);                                                                              \
            return true;                                                                                \
        }                                                                                               \
    }                                                                                                   \
    e->Dispose(e);                                                                                      \
    return false;                                                                                       \
}                                                                                                       \
\
int Enumerable_##T##_Count(IEnumerable_##T source)                                                      \
{                                                                                                       \
    IEnumerator_##T e = source->GetEnumerator(source);                                                  \
    int i = 0;                                                                                          \
    while (e->MoveNext(e)) {                                                                            \
        i += 1;                                                                                         \
    }                                                                                                   \
    e->Dispose(e);                                                                                      \
    return i;                                                                                           \
}                                                                                                       \
\
bool Enumerable_##T##_SequenceEqual(IEnumerable_##T first, IEnumerable_##T second)        \
{                                                                                                       \
    IEnumerator_##T e1 = first->GetEnumerator(first);                                                  \
    IEnumerator_##T e2 = second->GetEnumerator(second);                                                \
    while (e1->MoveNext(e1) & e2->MoveNext(e2)) {                                                       \
        if (!ValueEquator(sizeof(T), &e1->Current, &e2->Current)) {                                     \
            e1->Dispose(e1);                                                                            \
            e2->Dispose(e2);                                                                            \
            return false;                                                                               \
        }                                                                                               \
    }                                                                                                   \
    if (e1->MoveNext(e1) || e2->MoveNext(e2)) {                                                         \
        e1->Dispose(e1);                                                                                \
        e2->Dispose(e2);                                                                                \
        return false;                                                                                   \
    }                                                                                                   \
    e1->Dispose(e1);                                                                                    \
    e2->Dispose(e2);                                                                                    \
    return true;                                                                                        \
}                                                                                                       \

#pragma region Converters

#define ENUMERABLE_IMPLEMENT_SELECT(TSource, TResult)                                                   \
typedef const struct generic_select_enumerable_##TSource##_to_##TResult##_s {                           \
    struct generic_enumerable_##TResult##_s _parent;                                                    \
    IEnumerable_##TSource _baseEnumerable;                                                              \
    TResult (*_selector)(TSource);                                                                      \
} *SelectEnumerable_##TSource##_##TResult;                                                              \
typedef struct generic_select_enumerator_##TSource##_to_##TResult##_s {                                 \
    struct generic_enumerator_##TResult##_s _parent;                                                    \
    IEnumerator_##TSource _currentEnumerator;                                                           \
    SelectEnumerable_##TSource##_##TResult _baseEnumerable;                                             \
} *SelectEnumerator_##TSource##_##TResult;                                                              \
static bool SelectMoveNext_##TSource##_##TResult(IEnumerator_##TResult This)                            \
{                                                                                                       \
    SelectEnumerator_##TSource##_##TResult select = (SelectEnumerator_##TSource##_##TResult)This;       \
    if (select->_currentEnumerator->MoveNext(select->_currentEnumerator)) {                             \
        This->Current = ((SelectEnumerable_##TSource##_##TResult)select->_baseEnumerable)->             \
            _selector(select->_currentEnumerator->Current);                                             \
        return true;                                                                                    \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static void SelectReset_##TSource##_##TResult(IEnumerator_##TResult This)                               \
{                                                                                                       \
    This->Current = (TResult)0;                                                                         \
    SelectEnumerator_##TSource##_##TResult e = (SelectEnumerator_##TSource##_##TResult)This;            \
    e->_currentEnumerator->Reset(e->_currentEnumerator);                                                \
}                                                                                                       \
static void SelectDispose_##TSource##_##TResult(IEnumerator_##TResult This)                             \
{                                                                                                       \
    SelectEnumerator_##TSource##_##TResult e = (SelectEnumerator_##TSource##_##TResult)This;            \
    e->_currentEnumerator->Dispose(e->_currentEnumerator);                                              \
    free(This);                                                                                         \
}                                                                                                       \
static IEnumerator_##TResult GetSelectEnumerator_##TSource##_##TResult(IEnumerable_##TResult This)      \
{                                                                                                       \
    SelectEnumerable_##TSource##_##TResult select = (SelectEnumerable_##TSource##_##TResult)This;       \
    SelectEnumerator_##TSource##_##TResult result =                                                     \
        alloc(struct generic_select_enumerator_##TSource##_to_##TResult##_s);                           \
    init(struct generic_select_enumerator_##TSource##_to_##TResult##_s, result) {                       \
        ._parent = (struct generic_enumerator_##TResult##_s) {                                          \
            .MoveNext = SelectMoveNext_##TSource##_##TResult,                                           \
            .Reset = SelectReset_##TSource##_##TResult,                                                 \
            .Dispose = SelectDispose_##TSource##_##TResult                                              \
        },                                                                                              \
        ._currentEnumerator = select->_baseEnumerable->GetEnumerator(select->_baseEnumerable),          \
        ._baseEnumerable = select                                                                       \
    };                                                                                                 \
    return base(result);                                                                                \
}                                                                                                       \
\
IEnumerable_##TResult Enumerable_##TSource##_Select_##TResult(IEnumerable_##TSource source, TResult (*selector)(TSource))   \
{                                                                                                       \
    SelectEnumerable_##TSource##_##TResult result =                                                     \
        alloc(struct generic_select_enumerable_##TSource##_to_##TResult##_s);                           \
    init(struct generic_select_enumerable_##TSource##_to_##TResult##_s, result) {                       \
        ._parent = (struct generic_enumerable_##TResult##_s) {                                          \
            .GetEnumerator = GetSelectEnumerator_##TSource##_##TResult                                  \
        },                                                                                              \
        ._selector = selector,                                                                          \
        ._baseEnumerable = source                                                                       \
    };                                                                                                 \
    return base(result);                                                                                \
}

#define ENUMERABLE_IMPLEMENT_SELECTMANY(TSource, TResult)                                               \
typedef const struct generic_select_many_enumerable_##TSource##_to_##TResult##_s {                      \
    struct generic_enumerable_##TResult##_s _parent;                                                    \
    IEnumerable_##TSource _baseEnumerable;                                                              \
    IEnumerable_##TResult (*_selector)(TSource);                                                        \
} *SelectManyEnumerable_##TSource##_##TResult;                                                          \
typedef struct generic_select_many_enumerator_##TSource##_to_##TResult##_s {                            \
    struct generic_enumerator_##TResult##_s _parent;                                                    \
    IEnumerator_##TSource _innerEnumerator;                                                             \
    IEnumerator_##TResult _outerEnumerator;                                                             \
    SelectManyEnumerable_##TSource##_##TResult _baseEnumerable;                                         \
} *SelectManyEnumerator_##TSource##_##TResult;                                                          \
static bool SelectManyMoveNext_##TSource##_##TResult(IEnumerator_##TResult This)                        \
{                                                                                                       \
    SelectManyEnumerator_##TSource##_##TResult selectMany =                                             \
        (SelectManyEnumerator_##TSource##_##TResult)This;                                               \
    if (selectMany->_innerEnumerator != NULL &&                                                         \
        selectMany->_innerEnumerator->MoveNext(selectMany->_innerEnumerator)) {                         \
        This->Current = selectMany->_innerEnumerator->Current;                                          \
        return true;                                                                                    \
    } else if (selectMany->_outerEnumerator->MoveNext(selectMany->_outerEnumerator)) {                  \
        if (selectMany->_innerEnumerator != NULL) {                                                     \
            selectMany->_innerEnumerator->Dispose(selectMany->_innerEnumerator);                        \
        }                                                                                               \
        IEnumerable_##TResult result = ((SelectManyEnumerable_##TSource##_##TResult)selectMany->        \
            _baseEnumerable)->_selector(selectMany->_outerEnumerator->Current);                         \
        selectMany->_innerEnumerator = result->GetEnumerator(result);                                   \
        return SelectManyMoveNext_##TSource##_##TResult(This);                                          \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static void SelectManyReset_##TSource##_##TResult(IEnumerator_##TResult This)                           \
{                                                                                                       \
    SelectManyEnumerator_##TSource##_##TResult selectMany =                                             \
        (SelectManyEnumerator_##TSource##_##TResult)This;                                               \
    if (selectMany->_innerEnumerator != NULL)                                                           \
        selectMany->_innerEnumerator->Dispose(selectMany->_innerEnumerator);                            \
    selectMany->_innerEnumerator = NULL;                                                                \
    selectMany->_outerEnumerator->Reset(selectMany->_outerEnumerator);                                  \
}                                                                                                       \
static void SelectManyDispose_##TSource##_##TResult(IEnumerator_##TResult This)                         \
{                                                                                                       \
    SelectManyEnumerator_##TSource##_##TResult selectMany =                                             \
        (SelectManyEnumerator_##TSource##_##TResult)This;                                               \
    if (selectMany->_innerEnumerator != NULL)                                                           \
        selectMany->_innerEnumerator->Dispose(selectMany->_innerEnumerator);                            \
    selectMany->_outerEnumerator->Dispose(selectMany->_outerEnumerator);                                \
    free(This);                                                                                         \
}                                                                                                       \
static IEnumerator_##TResult GetSelectManyEnumerator_##TSource##_##TResult(IEnumerable_##TResult This)  \
{                                                                                                       \
    SelectManyEnumerable_##TSource##_##TResult selectMany =                                             \
        (SelectManyEnumerable_##TSource##_##TResult)This;                                               \
    SelectManyEnumerator_##TSource##_##TResult result =                                                 \
        alloc(struct generic_select_many_enumerator_##TSource##_to_##TResult##_s);                      \
    init(struct generic_select_many_enumerator_##TSource##_to_##TResult##_s, result) {                  \
        ._parent = (struct generic_enumerator_##TResult##_s) {                                          \
            .MoveNext = SelectManyMoveNext_##TSource##_##TResult,                                       \
            .Reset = SelectManyReset_##TSource##_##TResult,                                             \
            .Dispose = SelectManyDispose_##TSource##_##TResult                                          \
        },                                                                                              \
        ._outerEnumerator = selectMany->_baseEnumerable->GetEnumerator(selectMany->_baseEnumerable),    \
        ._baseEnumerable = selectMany                                                                   \
    };                                                                                                  \
    return base(result);                                                                                \
}                                                                                                       \
\
IEnumerable_##TResult Enumerable_##TSource##_SelectMany_##TResult(IEnumerable_##TSource source, IEnumerable_##TResult (*selector)(TSource)) \
{                                                                                                       \
    SelectManyEnumerable_##TSource##_##TResult result =                                                 \
        alloc(struct generic_select_many_enumerable_##TSource##_to_##TResult##_s);                      \
    init(struct generic_select_many_enumerable_##TSource##_to_##TResult##_s, result) {                  \
        ._parent = (struct generic_enumerable_##TResult##_s) {                                          \
            .GetEnumerator = GetSelectManyEnumerator_##TSource##_##TResult,                             \
        },                                                                                              \
        ._selector = selector,                                                                          \
        ._baseEnumerable = source                                                                       \
    };                                                                                                  \
    return base(result);                                                                                \
}

#define ENUMERABLE_IMPLEMENT_AGGREGATE(TSource, TAggregate)         \
TAggregate Enumerable_##TSource##_Aggregate_##TAggregate(IEnumerable_##TSource source, TAggregate seed, TAggregate (*aggregate)(TAggregate, TSource))   \
{                                                                   \
    IEnumerator_##TSource e = source->GetEnumerator(source);        \
    while(e->MoveNext(e)) {                                         \
        seed = aggregate(seed, e->Current);                         \
    }                                                               \
    e->Dispose(e);                                                  \
    return seed;                                                    \
}

#pragma endregion
#pragma endregion

#endif