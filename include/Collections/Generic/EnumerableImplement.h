#ifndef GENERIC_ENUMERABLE_IMPLEMENTATIONS
#define GENERIC_ENUMERABLE_IMPLEMENTATIONS
#include "Collections/Generic/EnumerableT.h"

#pragma region Implement

#define ENUMERABLE_IMPLEMENT(T)                                                                         \
typedef IEnumerator_##T* GetEnumerator_##T(const IEnumerable_##T *This);                                \
typedef struct generic_compound_enumerator_##T##_s {                                                    \
    IEnumerator_##T _parent;                                                                            \
    IEnumerator_##T* _currentEnumerator;                                                                \
    const IEnumerable_##T* _baseEnumerable;                                                             \
} CompoundEnumerator_##T;                                                                               \
static void CompoundReset_##T(IEnumerator_##T *This)                                                    \
{                                                                                                       \
    This->Current = (T){0};                                                                             \
    CompoundEnumerator_##T* e = (CompoundEnumerator_##T*)This;                                          \
    e->_currentEnumerator->Reset(e->_currentEnumerator);                                                \
}                                                                                                       \
static void CompoundDispose_##T(IEnumerator_##T *This)                                                  \
{                                                                                                       \
    CompoundEnumerator_##T* e = (CompoundEnumerator_##T*)This;                                          \
    e->_currentEnumerator->Dispose(e->_currentEnumerator);                                              \
    free(This);                                                                                         \
}                                                                                                       \
typedef struct generic_where_enumerable_##T##_s {                                                       \
    IEnumerable_##T _parent;                                                                            \
    const IEnumerable_##T* _baseEnumerable;                                                             \
    bool (*_filter)(T);                                                                                 \
} WhereEnumerable_##T;                                                                                  \
static bool WhereMoveNext_##T(IEnumerator_##T *This)                                                    \
{                                                                                                       \
    CompoundEnumerator_##T* where = (CompoundEnumerator_##T*)This;                                      \
    while (where->_currentEnumerator->MoveNext(where->_currentEnumerator)) {                            \
        This->Current = where->_currentEnumerator->Current;                                             \
        if (((const WhereEnumerable_##T*)where->_baseEnumerable)->_filter(This->Current)) {             \
            return true;                                                                                \
        }                                                                                               \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static IEnumerator_##T* GetWhereEnumerator_##T(const IEnumerable_##T *This)                             \
{                                                                                                       \
    const WhereEnumerable_##T* where = (const WhereEnumerable_##T*)This;                                \
    CompoundEnumerator_##T* result = alloc(CompoundEnumerator_##T);                                     \
    *result = (CompoundEnumerator_##T) {                                                                \
        ._parent = (IEnumerator_##T) {                                                                  \
            .MoveNext = WhereMoveNext_##T,                                                              \
            .Reset = CompoundReset_##T,                                                                 \
            .Dispose = CompoundDispose_##T                                                              \
        },                                                                                              \
        ._currentEnumerator = where->_baseEnumerable->GetEnumerator(where->_baseEnumerable),            \
        ._baseEnumerable = (const IEnumerable_##T*)where                                                \
    };                                                                                                  \
    return (IEnumerator_##T*)result;                                                                    \
}                                                                                                       \
\
IEnumerable_##T* Enumerable_##T##_Where(const IEnumerable_##T* source, bool (*filter)(T))               \
{                                                                                                       \
    WhereEnumerable_##T* result = alloc(WhereEnumerable_##T);                                           \
    *result = (WhereEnumerable_##T) {                                                                   \
        ._parent = (IEnumerable_##T) {                                                                  \
            .GetEnumerator = GetWhereEnumerator_##T                                                     \
        },                                                                                              \
        ._baseEnumerable = source,                                                                      \
        ._filter = filter                                                                               \
    };                                                                                                  \
    return (IEnumerable_##T*)result;                                                                    \
}                                                                                                       \
\
typedef struct generic_take_skip_enumerable_##T##_s {                                                   \
    IEnumerable_##T _parent;                                                                            \
    const IEnumerable_##T* _baseEnumerable;                                                             \
    int Count;                                                                                          \
} LimitedEnumerable_##T;                                                                                \
typedef struct generic_take_skip_enumerator_##T##_s {                                                   \
    CompoundEnumerator_##T _parent;                                                                     \
    int Count;                                                                                          \
} LimitedEnumerator_##T;                                                                                \
static void LimitedReset_##T(IEnumerator_##T *This)                                                     \
{                                                                                                       \
    ((LimitedEnumerator_##T*)This)->Count =                                                             \
        ((LimitedEnumerable_##T*)((CompoundEnumerator_##T*)This)->_baseEnumerable)->Count;              \
    CompoundReset_##T(This);                                                                            \
}                                                                                                       \
static bool TakeMoveNext_##T(IEnumerator_##T *This)                                                     \
{                                                                                                       \
    if (((LimitedEnumerator_##T*)This)->Count == 0) return false;                                       \
    ((LimitedEnumerator_##T*)This)->Count -= 1;                                                         \
    CompoundEnumerator_##T* compound = (CompoundEnumerator_##T*)This;                                   \
    if (compound->_currentEnumerator->MoveNext(compound->_currentEnumerator)) {                         \
        This->Current = compound->_currentEnumerator->Current;                                          \
        return true;                                                                                    \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static IEnumerator_##T* GetTakeEnumerator_##T(const IEnumerable_##T *This)                              \
{                                                                                                       \
    const LimitedEnumerable_##T* limited = (const LimitedEnumerable_##T*)This;                          \
    LimitedEnumerator_##T* result = alloc(LimitedEnumerator_##T);                                       \
    *result = (LimitedEnumerator_##T) {                                                                 \
        ._parent = (CompoundEnumerator_##T) {                                                           \
            ._parent = (IEnumerator_##T) {                                                              \
                .MoveNext = TakeMoveNext_##T,                                                           \
                .Reset = LimitedReset_##T,                                                              \
                .Dispose = CompoundDispose_##T,                                                         \
            },                                                                                          \
            ._currentEnumerator = limited->_baseEnumerable->                                            \
                GetEnumerator(limited->_baseEnumerable),                                                \
            ._baseEnumerable = (const IEnumerable_##T*)limited,                                         \
        },                                                                                              \
        .Count = limited->Count                                                                         \
    };                                                                                                  \
    return (IEnumerator_##T*)result;                                                                    \
}                                                                                                       \
static bool SkipMoveNext_##T(IEnumerator_##T *This)                                                     \
{                                                                                                       \
    CompoundEnumerator_##T* compound = (CompoundEnumerator_##T*)This;                                   \
    while (((LimitedEnumerator_##T*)This)->Count > 0) {                                                 \
        ((LimitedEnumerator_##T*)This)->Count -= 1;                                                     \
        compound->_currentEnumerator->MoveNext(compound->_currentEnumerator);                           \
    }                                                                                                   \
    if (compound->_currentEnumerator->MoveNext(compound->_currentEnumerator)) {                         \
        This->Current = compound->_currentEnumerator->Current;                                          \
        return true;                                                                                    \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static IEnumerator_##T* GetSkipEnumerator_##T(const IEnumerable_##T *This)                              \
{                                                                                                       \
    const LimitedEnumerable_##T* limited = (const LimitedEnumerable_##T*)This;                          \
    LimitedEnumerator_##T* result = alloc(LimitedEnumerator_##T);                                       \
    *result = (LimitedEnumerator_##T) {                                                                 \
        ._parent = (CompoundEnumerator_##T) {                                                           \
            ._parent = (IEnumerator_##T) {                                                              \
                .MoveNext = SkipMoveNext_##T,                                                           \
                .Reset = LimitedReset_##T,                                                              \
                .Dispose = CompoundDispose_##T,                                                         \
            },                                                                                          \
            ._currentEnumerator = limited->_baseEnumerable->                                            \
                GetEnumerator(limited->_baseEnumerable),                                                \
            ._baseEnumerable = (const IEnumerable_##T*)limited,                                         \
        },                                                                                              \
        .Count = limited->Count                                                                         \
    };                                                                                                  \
    return (IEnumerator_##T*)result;                                                                    \
}                                                                                                       \
\
IEnumerable_##T* Enumerable_##T##_Take(const IEnumerable_##T* source, int count)                        \
{                                                                                                       \
    LimitedEnumerable_##T* result = alloc(LimitedEnumerable_##T);                                       \
    *result = (LimitedEnumerable_##T) {                                                                 \
        ._parent = (IEnumerable_##T) {                                                                  \
            .GetEnumerator = GetTakeEnumerator_##T                                                      \
        },                                                                                              \
        ._baseEnumerable = source,                                                                      \
        .Count = count                                                                                  \
    };                                                                                                  \
    return (IEnumerable_##T*)result;                                                                    \
}                                                                                                       \
\
IEnumerable_##T* Enumerable_##T##_Skip(const IEnumerable_##T* source, int count)                        \
{                                                                                                       \
    LimitedEnumerable_##T* result = alloc(LimitedEnumerable_##T);                                       \
    *result = (LimitedEnumerable_##T) {                                                                 \
        ._parent = (IEnumerable_##T) {                                                                  \
            .GetEnumerator = GetSkipEnumerator_##T                                                      \
        },                                                                                              \
        ._baseEnumerable = source,                                                                      \
        .Count = count                                                                                  \
    };                                                                                                  \
    return (IEnumerable_##T*)result;                                                                    \
}                                                                                                       \
typedef struct generic_extended_enumerable_##T##_s {                                                    \
    IEnumerable_##T _parent;                                                                            \
    const IEnumerable_##T* _baseEnumerable;                                                             \
    T _added;                                                                                           \
} ExtendEnumerable_##T;                                                                                 \
typedef struct generic_extended_enumerator_##T##_s {                                                    \
    CompoundEnumerator_##T _parent;                                                                     \
    bool _hasEnumeratedExtra;                                                                           \
} ExtendEnumerator_##T;                                                                                 \
static void ExtendReset_##T(IEnumerator_##T *This)                                                      \
{                                                                                                       \
    ((ExtendEnumerator_##T*)This)->_hasEnumeratedExtra = false;                                         \
    CompoundReset_##T(This);                                                                            \
}                                                                                                       \
static bool AppendMoveNext_##T(IEnumerator_##T *This)                                                   \
{                                                                                                       \
    CompoundEnumerator_##T* compound = (CompoundEnumerator_##T*)This;                                   \
    if (compound->_currentEnumerator->MoveNext(compound->_currentEnumerator)) {                         \
        This->Current = compound->_currentEnumerator->Current;                                          \
        return true;                                                                                    \
    } else if (!((ExtendEnumerator_##T*)This)->_hasEnumeratedExtra) {                                   \
        This->Current = ((ExtendEnumerable_##T*)compound->_baseEnumerable)->_added;                     \
        ((ExtendEnumerator_##T*)This)->_hasEnumeratedExtra = true;                                      \
        return true;                                                                                    \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static IEnumerator_##T* GetAppendEnumerator_##T(const IEnumerable_##T *This)                            \
{                                                                                                       \
    const ExtendEnumerable_##T* extend = (const ExtendEnumerable_##T*)This;                             \
    ExtendEnumerator_##T* result = alloc(ExtendEnumerator_##T);                                         \
    *result = (ExtendEnumerator_##T) {                                                                  \
        ._parent = (CompoundEnumerator_##T) {                                                           \
            ._parent = (IEnumerator_##T) {                                                              \
                .MoveNext = AppendMoveNext_##T,                                                         \
                .Reset = ExtendReset_##T,                                                               \
                .Dispose = CompoundDispose_##T                                                          \
            },                                                                                          \
            ._currentEnumerator = extend->_baseEnumerable->                                             \
                GetEnumerator(extend->_baseEnumerable),                                                 \
            ._baseEnumerable = (const IEnumerable_##T*)extend                                           \
        },                                                                                              \
        ._hasEnumeratedExtra = false                                                                    \
    };                                                                                                  \
    return (IEnumerator_##T*)result;                                                                    \
}                                                                                                       \
static bool PrependMoveNext_##T(IEnumerator_##T *This)                                                  \
{                                                                                                       \
    CompoundEnumerator_##T* compound = (CompoundEnumerator_##T*)This;                                   \
    if (!((ExtendEnumerator_##T*)This)->_hasEnumeratedExtra) {                                          \
        This->Current = ((ExtendEnumerable_##T*)compound->_baseEnumerable)->_added;                     \
        ((ExtendEnumerator_##T*)This)->_hasEnumeratedExtra = true;                                      \
        return true;                                                                                    \
    } else if (compound->_currentEnumerator->MoveNext(compound->_currentEnumerator)) {                  \
        This->Current = compound->_currentEnumerator->Current;                                          \
        return true;                                                                                    \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static IEnumerator_##T* GetPrependEnumerator_##T(const IEnumerable_##T *This)                           \
{                                                                                                       \
    const ExtendEnumerable_##T* extend = (const ExtendEnumerable_##T*)This;                             \
    ExtendEnumerator_##T* result = alloc(ExtendEnumerator_##T);                                         \
    *result = (ExtendEnumerator_##T) {                                                                  \
        ._parent = (CompoundEnumerator_##T) {                                                           \
            ._parent = (IEnumerator_##T) {                                                              \
                .MoveNext = PrependMoveNext_##T,                                                        \
                .Reset = ExtendReset_##T,                                                               \
                .Dispose = CompoundDispose_##T                                                          \
            },                                                                                          \
            ._currentEnumerator = extend->_baseEnumerable->                                             \
                GetEnumerator(extend->_baseEnumerable),                                                 \
            ._baseEnumerable = (const IEnumerable_##T*)extend                                           \
        },                                                                                              \
        ._hasEnumeratedExtra = false                                                                    \
    };                                                                                                  \
    return (IEnumerator_##T*)result;                                                                    \
}                                                                                                       \
\
IEnumerable_##T* Enumerable_##T##_Append(const IEnumerable_##T* source, T item)                         \
{                                                                                                       \
    ExtendEnumerable_##T* result = alloc(ExtendEnumerable_##T);                                         \
    *result = (ExtendEnumerable_##T) {                                                                  \
        ._parent = (IEnumerable_##T) {                                                                  \
            .GetEnumerator = GetAppendEnumerator_##T                                                    \
        },                                                                                              \
        ._baseEnumerable = source,                                                                      \
        ._added = item                                                                                  \
    };                                                                                                  \
    return (IEnumerable_##T*)result;                                                                    \
}                                                                                                       \
\
IEnumerable_##T* Enumerable_##T##_Prepend(const IEnumerable_##T* source, T item)                        \
{                                                                                                       \
    ExtendEnumerable_##T* result = alloc(ExtendEnumerable_##T);                                         \
    *result = (ExtendEnumerable_##T) {                                                                  \
        ._parent = (IEnumerable_##T) {                                                                  \
            .GetEnumerator = GetPrependEnumerator_##T                                                   \
        },                                                                                              \
        ._baseEnumerable = source,                                                                      \
        ._added = item                                                                                  \
    };                                                                                                  \
    return (IEnumerable_##T*)result;                                                                    \
}                                                                                                       \
\
typedef struct generic_concat_enumerable_##T##_s {                                                      \
    IEnumerable_##T _parent;                                                                            \
    const IEnumerable_##T* _firstEnumerable;                                                            \
    const IEnumerable_##T* _secondEnumerable;                                                           \
} ConcatEnumerable_##T;                                                                                 \
typedef struct generic_concat_enumerator_##T##_s {                                                      \
    CompoundEnumerator_##T _parent;                                                                     \
    bool _startedSecondEnumeration;                                                                     \
} ConcatEnumerator_##T;                                                                                 \
static bool ConcatMoveNext_##T(IEnumerator_##T *This)                                                   \
{                                                                                                       \
    CompoundEnumerator_##T* compound = (CompoundEnumerator_##T*)This;                                   \
    if (compound->_currentEnumerator->MoveNext(compound->_currentEnumerator)) {                         \
        This->Current = compound->_currentEnumerator->Current;                                          \
        return true;                                                                                    \
    } else if (!((ConcatEnumerator_##T*)This)->_startedSecondEnumeration) {                             \
        const IEnumerable_##T* secondEnum =                                                             \
            ((ConcatEnumerable_##T*)compound->_baseEnumerable)->_secondEnumerable;                      \
        compound->_currentEnumerator = secondEnum->GetEnumerator(secondEnum);                           \
        ((ConcatEnumerator_##T*)This)->_startedSecondEnumeration = true;                                \
        return ConcatMoveNext_##T(This);                                                                \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static void ConcatReset_##T(IEnumerator_##T *This)                                                      \
{                                                                                                       \
    ((ConcatEnumerator_##T*)This)->_startedSecondEnumeration = false;                                   \
    CompoundReset_##T(This);                                                                            \
}                                                                                                       \
static IEnumerator_##T* GetConcatEnumerator_##T(const IEnumerable_##T *This)                            \
{                                                                                                       \
    const ConcatEnumerable_##T* concat = (const ConcatEnumerable_##T*)This;                             \
    ConcatEnumerator_##T* result = alloc(ConcatEnumerator_##T);                                         \
    *result = (ConcatEnumerator_##T) {                                                                  \
        ._parent = (CompoundEnumerator_##T) {                                                           \
            ._parent = (IEnumerator_##T) {                                                              \
                .MoveNext = ConcatMoveNext_##T,                                                         \
                .Reset = ConcatReset_##T,                                                               \
                .Dispose = CompoundDispose_##T                                                          \
            },                                                                                          \
            ._currentEnumerator = concat->_firstEnumerable->                                            \
                GetEnumerator(concat->_firstEnumerable),                                                \
            ._baseEnumerable = (const IEnumerable_##T*)concat                                           \
        },                                                                                              \
        ._startedSecondEnumeration = false                                                              \
    };                                                                                                  \
    return (IEnumerator_##T*)result;                                                                    \
}                                                                                                       \
\
IEnumerable_##T* Enumerable_##T##_Concat(const IEnumerable_##T* first, const IEnumerable_##T* second)   \
{                                                                                                       \
    ConcatEnumerable_##T* result = alloc(ConcatEnumerable_##T);                                         \
    *result = (ConcatEnumerable_##T) {                                                                  \
        ._parent = (IEnumerable_##T) {                                                                  \
            .GetEnumerator = GetConcatEnumerator_##T                                                    \
        },                                                                                              \
        ._firstEnumerable = first,                                                                      \
        ._secondEnumerable = second                                                                     \
    };                                                                                                  \
    return (IEnumerable_##T*)result;                                                                    \
}                                                                                                       \
\
T Enumerable_##T##_ElementAt(const IEnumerable_##T* source, int index)                                  \
{                                                                                                       \
    IEnumerator_##T* e = source->GetEnumerator(source);                                                 \
    while(e->MoveNext(e) && index > 0) { index -= 1; }                                                  \
    T item = e->Current;                                                                                \
    e->Dispose(e);                                                                                      \
    return item;                                                                                        \
}                                                                                                       \
\
bool Enumerable_##T##_Any(const IEnumerable_##T* source, bool (*predicate)(T))                          \
{                                                                                                       \
    IEnumerator_##T* e = source->GetEnumerator(source);                                                 \
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
bool Enumerable_##T##_All(const IEnumerable_##T* source, bool (*predicate)(T))                          \
{                                                                                                       \
    IEnumerator_##T* e = source->GetEnumerator(source);                                                 \
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
int Enumerable_##T##_IndexOf(const IEnumerable_##T* source, T item)                                     \
{                                                                                                       \
    IEnumerator_##T* e = source->GetEnumerator(source);                                                 \
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
T Enumerable_##T##_FirstOrDefault(const IEnumerable_##T* source, bool (*predicate)(T))                  \
{                                                                                                       \
    IEnumerator_##T* e = source->GetEnumerator(source);                                                 \
    while (e->MoveNext(e)) {                                                                            \
        if (predicate(e->Current)) {                                                                    \
            T item = e->Current;                                                                        \
            e->Dispose(e);                                                                              \
            return item;                                                                                \
        }                                                                                               \
    }                                                                                                   \
    e->Dispose(e);                                                                                      \
    return (T){0};                                                                                      \
}                                                                                                       \
\
bool Enumerable_##T##_Contains(const IEnumerable_##T* source, T item)                                   \
{                                                                                                       \
    IEnumerator_##T* e = source->GetEnumerator(source);                                                 \
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
int Enumerable_##T##_Count(const IEnumerable_##T* source)                                               \
{                                                                                                       \
    IEnumerator_##T* e = source->GetEnumerator(source);                                                 \
    int i = 0;                                                                                          \
    while (e->MoveNext(e)) {                                                                            \
        i += 1;                                                                                         \
    }                                                                                                   \
    e->Dispose(e);                                                                                      \
    return i;                                                                                           \
}                                                                                                       \
\
bool Enumerable_##T##_SequenceEqual(const IEnumerable_##T* first, const IEnumerable_##T* second)        \
{                                                                                                       \
    IEnumerator_##T* e1 = first->GetEnumerator(first);                                                  \
    IEnumerator_##T* e2 = second->GetEnumerator(second);                                                \
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
typedef struct generic_select_enumerable_##TSource##_to_##TResult##_s {                                 \
    IEnumerable_##TResult _parent;                                                                      \
    const IEnumerable_##TSource* _baseEnumerable;                                                       \
    TResult (*_selector)(TSource);                                                                      \
} SelectEnumerable_##TSource##_##TResult;                                                               \
typedef struct generic_select_enumerator_##TSource##_to_##TResult##_s {                                 \
    IEnumerator_##TResult _parent;                                                                      \
    IEnumerator_##TSource* _currentEnumerator;                                                          \
    const SelectEnumerable_##TSource##_##TResult* _baseEnumerable;                                      \
} SelectEnumerator_##TSource##_##TResult;                                                               \
static bool SelectMoveNext_##TSource##_##TResult(IEnumerator_##TResult *This)                           \
{                                                                                                       \
    SelectEnumerator_##TSource##_##TResult* select = (SelectEnumerator_##TSource##_##TResult*)This;     \
    if (select->_currentEnumerator->MoveNext(select->_currentEnumerator)) {                             \
        This->Current = ((const SelectEnumerable_##TSource##_##TResult*)select->_baseEnumerable)->      \
            _selector(select->_currentEnumerator->Current);                                             \
        return true;                                                                                    \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static void SelectReset_##TSource##_##TResult(IEnumerator_##TResult *This)                              \
{                                                                                                       \
    This->Current = (TResult)0;                                                                         \
    SelectEnumerator_##TSource##_##TResult* e = (SelectEnumerator_##TSource##_##TResult*)This;          \
    e->_currentEnumerator->Reset(e->_currentEnumerator);                                                \
}                                                                                                       \
static void SelectDispose_##TSource##_##TResult(IEnumerator_##TResult *This)                            \
{                                                                                                       \
    SelectEnumerator_##TSource##_##TResult* e = (SelectEnumerator_##TSource##_##TResult*)This;          \
    e->_currentEnumerator->Dispose(e->_currentEnumerator);                                              \
    free(This);                                                                                         \
}                                                                                                       \
static IEnumerator_##TResult* GetSelectEnumerator_##TSource##_##TResult(const IEnumerable_##TResult *This)  \
{                                                                                                       \
    const SelectEnumerable_##TSource##_##TResult* select =                                              \
        (const SelectEnumerable_##TSource##_##TResult*)This;                                            \
    SelectEnumerator_##TSource##_##TResult* result = alloc(SelectEnumerator_##TSource##_##TResult);     \
    *result = (SelectEnumerator_##TSource##_##TResult) {                                                \
        ._parent = (IEnumerator_##TResult) {                                                            \
            .MoveNext = SelectMoveNext_##TSource##_##TResult,                                           \
            .Reset = SelectReset_##TSource##_##TResult,                                                 \
            .Dispose = SelectDispose_##TSource##_##TResult                                              \
        },                                                                                              \
        ._currentEnumerator = select->_baseEnumerable->GetEnumerator(select->_baseEnumerable),          \
        ._baseEnumerable = select                                                                       \
    };                                                                                                  \
    return (IEnumerator_##TResult*)result;                                                              \
}                                                                                                       \
\
IEnumerable_##TResult* Enumerable_##TSource##_Select_##TResult(const IEnumerable_##TSource* source, TResult (*selector)(TSource))   \
{                                                                                                       \
    SelectEnumerable_##TSource##_##TResult* result = alloc(SelectEnumerable_##TSource##_##TResult);     \
    *result = (SelectEnumerable_##TSource##_##TResult) {                                                \
        ._parent = (IEnumerable_##TResult) {                                                            \
            .GetEnumerator = GetSelectEnumerator_##TSource##_##TResult                                  \
        },                                                                                              \
        ._selector = selector,                                                                          \
        ._baseEnumerable = source                                                                       \
    };                                                                                                  \
    return (IEnumerable_##TResult*)result;                                                                        \
}

#define ENUMERABLE_IMPLEMENT_SELECTMANY(TSource, TResult)                                               \
typedef struct generic_select_many_enumerable_##TSource##_to_##TResult##_s {                            \
    IEnumerable_##TResult _parent;                                                                      \
    const IEnumerable_##TSource* _baseEnumerable;                                                       \
    IEnumerable_##TResult* (*_selector)(TSource);                                                       \
} SelectManyEnumerable_##TSource##_##TResult;                                                           \
typedef struct generic_select_many_enumerator_##TSource##_to_##TResult##_s {                            \
    IEnumerator_##TResult _parent;                                                                      \
    IEnumerator_##TSource* _innerEnumerator;                                                            \
    /* CALCOPOD. WE. ARE SO. FUCKED. */    \
    IEnumerator_##TResult* _outerEnumerator;                                                            \
    const SelectManyEnumerable_##TSource##_##TResult* _baseEnumerable;                                  \
} SelectManyEnumerator_##TSource##_##TResult;                                                           \
static bool SelectManyMoveNext_##TSource##_##TResult(IEnumerator_##TResult *This)                       \
{                                                                                                       \
    SelectManyEnumerator_##TSource##_##TResult* selectMany =                                            \
        (SelectManyEnumerator_##TSource##_##TResult*)This;                                              \
    if (selectMany->_innerEnumerator != NULL &&                                                         \
        selectMany->_innerEnumerator->MoveNext(selectMany->_innerEnumerator)) {                         \
        This->Current = selectMany->_innerEnumerator->Current;                                          \
        return true;                                                                                    \
    } else if (selectMany->_outerEnumerator->MoveNext(selectMany->_outerEnumerator)) {                  \
        if (selectMany->_innerEnumerator != NULL) {                                                     \
            selectMany->_innerEnumerator->Dispose(selectMany->_innerEnumerator);                        \
        }                                                                                               \
        IEnumerable_##TResult* result = ((const SelectManyEnumerable_##TSource##_##TResult*)selectMany->\
            _baseEnumerable)->_selector(selectMany->_outerEnumerator->Current);                         \
        selectMany->_innerEnumerator = result->GetEnumerator(result);                                   \
        return SelectManyMoveNext_##TSource##_##TResult(This);                                          \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static void SelectManyReset_##TSource##_##TResult(IEnumerator_##TResult *This)                          \
{                                                                                                       \
    SelectManyEnumerator_##TSource##_##TResult* selectMany =                                            \
        (SelectManyEnumerator_##TSource##_##TResult*)This;                                              \
    if (selectMany->_innerEnumerator != NULL)                                                           \
        selectMany->_innerEnumerator->Dispose(selectMany->_innerEnumerator);                            \
    selectMany->_innerEnumerator = NULL;                                                                \
    selectMany->_outerEnumerator->Reset(selectMany->_outerEnumerator);                                  \
}                                                                                                       \
static void SelectManyDispose_##TSource##_##TResult(IEnumerator_##TResult *This)                        \
{                                                                                                       \
    SelectManyEnumerator_##TSource##_##TResult* selectMany =                                            \
        (SelectManyEnumerator_##TSource##_##TResult*)This;                                              \
    if (selectMany->_innerEnumerator != NULL)                                                           \
        selectMany->_innerEnumerator->Dispose(selectMany->_innerEnumerator);                            \
    selectMany->_outerEnumerator->Dispose(selectMany->_outerEnumerator);                                \
    free(This);                                                                                         \
}                                                                                                       \
static IEnumerator_##TResult* GetSelectManyEnumerator_##TSource##_##TResult(const IEnumerable_##TResult* This)  \
{                                                                                                       \
    const SelectManyEnumerable_##TSource##_##TResult* selectMany =                                      \
        (const SelectManyEnumerable_##TSource##_##TResult*)This;                                        \
    SelectManyEnumerator_##TSource##_##TResult* result =                                                \
        alloc(SelectManyEnumerator_##TSource##_##TResult);                                              \
    *result = (SelectManyEnumerator_##TSource##_##TResult) {                                            \
        ._parent = (IEnumerator_##TResult) {                                                            \
            .MoveNext = SelectManyMoveNext_##TSource##_##TResult,                                       \
            .Reset = SelectManyReset_##TSource##_##TResult,                                             \
            .Dispose = SelectManyDispose_##TSource##_##TResult                                          \
        },                                                                                              \
        ._outerEnumerator = selectMany->_baseEnumerable->GetEnumerator(selectMany->_baseEnumerable),    \
        ._baseEnumerable = selectMany                                                                   \
    };                                                                                                  \
    return (IEnumerator_##TResult*)result;                                                              \
}                                                                                                       \
\
IEnumerable_##TResult* Enumerable_##TSource##_SelectMany_##TResult(const IEnumerable_##TSource* source, IEnumerable_##TResult* (*selector)(TSource))    \
{                                                                                                       \
    SelectManyEnumerable_##TSource##_##TResult* result =                                                \
        alloc(SelectManyEnumerable_##TSource##_##TResult);                                              \
    *result = (SelectManyEnumerable_##TSource##_##TResult) {                                            \
        ._parent = (IEnumerable_##TResult) {                                                            \
            .GetEnumerator = GetSelectManyEnumerator_##TSource##_##TResult,                             \
        },                                                                                              \
        ._selector = selector,                                                                          \
        ._baseEnumerable = source                                                                       \
    };                                                                                                  \
    return (IEnumerable_##TResult*)result;                                                              \
}

#define ENUMERABLE_IMPLEMENT_AGGREGATE(TSource, TAggregate)         \
TAggregate Enumerable_##TSource##_Aggregate_##TAggregate(const IEnumerable_##TSource* source, TAggregate seed, TAggregate (*aggregate)(TAggregate, TSource))    \
{                                                                   \
    IEnumerator_##TSource* e = source->GetEnumerator(source);       \
    TAggregate current = seed;                                      \
    while(e->MoveNext(e)) {                                         \
        current = aggregate(current, e->Current);                   \
    }                                                               \
    e->Dispose(e);                                                  \
    return current;                                                 \
}

#pragma endregion
#pragma endregion

#endif