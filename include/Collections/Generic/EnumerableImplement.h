#ifndef COLLECTIONS_GENERIC_ENUMERABLE_IMPLEMENTATIONS
#define COLLECTIONS_GENERIC_ENUMERABLE_IMPLEMENTATIONS
#include "EnumerableT.h"
#define ENUMERABLE_IMPLEMENT(T)                                                                         \
static bool EmptyMoveNext_##T([[maybe_unused]] IEnumerator(T) This) { return false; }                   \
static void EmptyReset_##T([[maybe_unused]] IEnumerator(T) This) {}                                     \
static void EmptyDispose_##T(IEnumerator(T) This) { memfree(This); }                                    \
static IEnumerator(T) EmptyEnumerator_##T([[maybe_unused]] IEnumerable(T) This)                         \
{                                                                                                       \
    IEnumerator(T) result = meminit(IEnumerator(T)) {                                                   \
        .MoveNext = EmptyMoveNext_##T,                                                                  \
        .Reset = EmptyReset_##T,                                                                        \
        .Dispose = EmptyDispose_##T,                                                                    \
        .Current = default(T)                                                                           \
    };                                                                                                  \
    return result;                                                                                      \
}                                                                                                       \
TAG(IEnumerable(T)) Enumerable_##T##_Empty[1] = {(TAG(IEnumerable(T))) {                                \
    .GetEnumerator = EmptyEnumerator_##T                                                                \
}};                                                                                                     \
typedef TAG(CompoundEnumerator_##T) {                                                                   \
    IMPL(IEnumerator(T));                                                                               \
    IEnumerator(T) _currentEnumerator;                                                                  \
    IEnumerable(T) _baseEnumerable;                                                                     \
} *CompoundEnumerator_##T;                                                                              \
static void CompoundReset_##T(IEnumerator(T) This)                                                      \
{                                                                                                       \
    This->Current = default(T);                                                                         \
    CompoundEnumerator_##T e = (CompoundEnumerator_##T)This;                                            \
    e->_currentEnumerator->Reset(e->_currentEnumerator);                                                \
}                                                                                                       \
static void CompoundDispose_##T(IEnumerator(T) This)                                                    \
{                                                                                                       \
    CompoundEnumerator_##T e = (CompoundEnumerator_##T)This;                                            \
    e->_currentEnumerator->Dispose(e->_currentEnumerator);                                              \
    memfree(This);                                                                                      \
}                                                                                                       \
typedef TAG(WhereEnumerable_##T) {                                                                \
    IMPL(IEnumerable(T));                                                                               \
    IEnumerable(T) _baseEnumerable;                                                                     \
    bool (*_filter)(T);                                                                                 \
} *WhereEnumerable_##T;                                                                                 \
static bool WhereMoveNext_##T(IEnumerator(T) This)                                                      \
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
static IEnumerator(T) GetWhereEnumerator_##T(IEnumerable(T) This)                                       \
{                                                                                                       \
    WhereEnumerable_##T where = (WhereEnumerable_##T)This;                                              \
    CompoundEnumerator_##T result = meminit(CompoundEnumerator_##T) {                                   \
        .MoveNext = WhereMoveNext_##T,                                                                  \
        .Reset = CompoundReset_##T,                                                                     \
        .Dispose = CompoundDispose_##T,                                                                 \
        ._currentEnumerator = where->_baseEnumerable->GetEnumerator(where->_baseEnumerable),            \
        ._baseEnumerable = (IEnumerable(T))where                                                        \
    };                                                                                                  \
    return (IEnumerator(T))result;                                                                      \
}                                                                                                       \
IEnumerable(T) Enumerable_##T##_Where(IEnumerable(T) source, bool (*filter)(T))                         \
{                                                                                                       \
    WhereEnumerable_##T result = meminit(WhereEnumerable_##T) {                                         \
        .GetEnumerator = GetWhereEnumerator_##T,                                                        \
        ._baseEnumerable = source,                                                                      \
        ._filter = filter                                                                               \
    };                                                                                                  \
    return (IEnumerable(T))result;                                                                      \
}                                                                                                       \
typedef TAG(LimitedEnumerable_##T) {                                                                    \
    IMPL(IEnumerable(T));                                                                               \
    IEnumerable(T) _baseEnumerable;                                                                     \
    int Count;                                                                                          \
} *LimitedEnumerable_##T;                                                                               \
typedef TAG(LimitedEnumerator_##T) {                                                                    \
    IMPL(CompoundEnumerator_##T);                                                                       \
    int Count;                                                                                          \
} *LimitedEnumerator_##T;                                                                               \
static void LimitedReset_##T(IEnumerator(T) This)                                                       \
{                                                                                                       \
    ((LimitedEnumerator_##T)This)->Count =                                                              \
        ((LimitedEnumerable_##T)((CompoundEnumerator_##T)This)->_baseEnumerable)->Count;                \
    CompoundReset_##T(This);                                                                            \
}                                                                                                       \
static bool TakeMoveNext_##T(IEnumerator(T) This)                                                       \
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
static IEnumerator(T) GetTakeEnumerator_##T(IEnumerable(T) This)                                        \
{                                                                                                       \
    LimitedEnumerable_##T limited = (LimitedEnumerable_##T)This;                                        \
    LimitedEnumerator_##T result = meminit(LimitedEnumerator_##T) {                                     \
        .MoveNext = TakeMoveNext_##T,                                                                   \
        .Reset = LimitedReset_##T,                                                                      \
        .Dispose = CompoundDispose_##T,                                                                 \
        ._currentEnumerator = limited->_baseEnumerable->                                                \
            GetEnumerator(limited->_baseEnumerable),                                                    \
        ._baseEnumerable = (IEnumerable(T))limited,                                                     \
        .Count = limited->Count                                                                         \
    };                                                                                                  \
    return (IEnumerator(T))result;                                                                      \
}                                                                                                       \
static bool SkipMoveNext_##T(IEnumerator(T) This)                                                       \
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
static IEnumerator(T) GetSkipEnumerator_##T(IEnumerable(T) This)                                        \
{                                                                                                       \
    LimitedEnumerable_##T limited = (LimitedEnumerable_##T)This;                                        \
    LimitedEnumerator_##T result = meminit(LimitedEnumerator_##T) {                                     \
        .MoveNext = SkipMoveNext_##T,                                                                   \
        .Reset = LimitedReset_##T,                                                                      \
        .Dispose = CompoundDispose_##T,                                                                 \
        ._currentEnumerator = limited->_baseEnumerable->                                                \
            GetEnumerator(limited->_baseEnumerable),                                                    \
        ._baseEnumerable = (IEnumerable(T))limited,                                                     \
        .Count = limited->Count                                                                         \
    };                                                                                                  \
    return (IEnumerator(T))result;                                                                      \
}                                                                                                       \
IEnumerable(T) Enumerable_##T##_Take(IEnumerable(T) source, int count)                                  \
{                                                                                                       \
    LimitedEnumerable_##T result = meminit(LimitedEnumerable_##T) {                                     \
        .GetEnumerator = GetTakeEnumerator_##T,                                                         \
        ._baseEnumerable = source,                                                                      \
        .Count = count                                                                                  \
    };                                                                                                  \
    return (IEnumerable(T))result;                                                                      \
}                                                                                                       \
IEnumerable(T) Enumerable_##T##_Skip(IEnumerable(T) source, int count)                                  \
{                                                                                                       \
    LimitedEnumerable_##T result = meminit(LimitedEnumerable_##T) {                                     \
        .GetEnumerator = GetSkipEnumerator_##T,                                                         \
        ._baseEnumerable = source,                                                                      \
        .Count = count                                                                                  \
    };                                                                                                  \
    return (IEnumerable(T))result;                                                                      \
}                                                                                                       \
typedef TAG(ExtendEnumerable_##T) {                                                                     \
    IMPL(IEnumerable(T));                                                                               \
    IEnumerable(T) _baseEnumerable;                                                                     \
    T _added;                                                                                           \
} *ExtendEnumerable_##T;                                                                                \
typedef TAG(ExtendEnumerator_##T) {                                                                     \
    IMPL(CompoundEnumerator_##T);                                                                       \
    bool _hasEnumeratedExtra;                                                                           \
} *ExtendEnumerator_##T;                                                                                \
static void ExtendReset_##T(IEnumerator(T) This)                                                        \
{                                                                                                       \
    ((ExtendEnumerator_##T)This)->_hasEnumeratedExtra = false;                                          \
    CompoundReset_##T(This);                                                                            \
}                                                                                                       \
static bool AppendMoveNext_##T(IEnumerator(T) This)                                                     \
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
static IEnumerator(T) GetAppendEnumerator_##T(IEnumerable(T) This)                                      \
{                                                                                                       \
    ExtendEnumerable_##T extend = (ExtendEnumerable_##T)This;                                           \
    ExtendEnumerator_##T result = meminit(ExtendEnumerator_##T) {                                       \
        .MoveNext = AppendMoveNext_##T,                                                                 \
        .Reset = ExtendReset_##T,                                                                       \
        .Dispose = CompoundDispose_##T,                                                                 \
        ._currentEnumerator = extend->_baseEnumerable->                                                 \
            GetEnumerator(extend->_baseEnumerable),                                                     \
        ._baseEnumerable = (IEnumerable(T))extend,                                                      \
        ._hasEnumeratedExtra = false                                                                    \
    };                                                                                                  \
    return (IEnumerator(T))result;                                                                      \
}                                                                                                       \
static bool PrependMoveNext_##T(IEnumerator(T) This)                                                    \
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
static IEnumerator(T) GetPrependEnumerator_##T(IEnumerable(T) This)                                     \
{                                                                                                       \
    ExtendEnumerable_##T extend = (ExtendEnumerable_##T)This;                                           \
    ExtendEnumerator_##T result = meminit(ExtendEnumerator_##T) {                                       \
        .MoveNext = PrependMoveNext_##T,                                                                \
        .Reset = ExtendReset_##T,                                                                       \
        .Dispose = CompoundDispose_##T,                                                                 \
        ._currentEnumerator = extend->_baseEnumerable->                                                 \
            GetEnumerator(extend->_baseEnumerable),                                                     \
        ._baseEnumerable = (IEnumerable(T))extend,                                                      \
        ._hasEnumeratedExtra = false                                                                    \
    };                                                                                                  \
    return (IEnumerator(T))result;                                                                      \
}                                                                                                       \
IEnumerable(T) Enumerable_##T##_Append(IEnumerable(T) source, T item)                                   \
{                                                                                                       \
    ExtendEnumerable_##T result = meminit(ExtendEnumerable_##T) {                                       \
        .GetEnumerator = GetAppendEnumerator_##T,                                                       \
        ._baseEnumerable = source,                                                                      \
        ._added = item                                                                                  \
    };                                                                                                  \
    return (IEnumerable(T))result;                                                                      \
}                                                                                                       \
IEnumerable(T) Enumerable_##T##_Prepend(IEnumerable(T) source, T item)                                  \
{                                                                                                       \
    ExtendEnumerable_##T result = meminit(ExtendEnumerable_##T) {                                       \
        .GetEnumerator = GetPrependEnumerator_##T,                                                      \
        ._baseEnumerable = source,                                                                      \
        ._added = item                                                                                  \
    };                                                                                                  \
    return (IEnumerable(T))result;                                                                      \
}                                                                                                       \
typedef TAG(ConcatEnumerable_##T) {                                                                     \
    IMPL(IEnumerable(T));                                                                               \
    IEnumerable(T) _firstEnumerable;                                                                    \
    IEnumerable(T) _secondEnumerable;                                                                   \
} *ConcatEnumerable_##T;                                                                                \
typedef TAG(ConcatEnumerator_##T) {                                                                     \
    IMPL(CompoundEnumerator_##T);                                                                       \
    bool _startedSecondEnumeration;                                                                     \
} *ConcatEnumerator_##T;                                                                                \
static bool ConcatMoveNext_##T(IEnumerator(T) This)                                                     \
{                                                                                                       \
    CompoundEnumerator_##T compound = (CompoundEnumerator_##T)This;                                     \
    if (compound->_currentEnumerator->MoveNext(compound->_currentEnumerator)) {                         \
        This->Current = compound->_currentEnumerator->Current;                                          \
        return true;                                                                                    \
    } else if (!((ConcatEnumerator_##T)This)->_startedSecondEnumeration) {                              \
        IEnumerable(T) secondEnum =                                                                     \
            ((ConcatEnumerable_##T)compound->_baseEnumerable)->_secondEnumerable;                       \
        compound->_currentEnumerator = secondEnum->GetEnumerator(secondEnum);                           \
        ((ConcatEnumerator_##T)This)->_startedSecondEnumeration = true;                                 \
        return ConcatMoveNext_##T(This);                                                                \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static void ConcatReset_##T(IEnumerator(T) This)                                                        \
{                                                                                                       \
    ((ConcatEnumerator_##T)This)->_startedSecondEnumeration = false;                                    \
    CompoundReset_##T(This);                                                                            \
}                                                                                                       \
static IEnumerator(T) GetConcatEnumerator_##T(IEnumerable(T) This)                                      \
{                                                                                                       \
    ConcatEnumerable_##T concat = (ConcatEnumerable_##T)This;                                           \
    ConcatEnumerator_##T result = meminit(ConcatEnumerator_##T) {                                       \
        .MoveNext = ConcatMoveNext_##T,                                                                 \
        .Reset = ConcatReset_##T,                                                                       \
        .Dispose = CompoundDispose_##T,                                                                 \
        ._currentEnumerator = concat->_firstEnumerable->                                                \
            GetEnumerator(concat->_firstEnumerable),                                                    \
        ._baseEnumerable = (IEnumerable(T))concat,                                                      \
        ._startedSecondEnumeration = false                                                              \
    };                                                                                                  \
    return (IEnumerator(T))result;                                                                      \
}                                                                                                       \
IEnumerable(T) Enumerable_##T##_Concat(IEnumerable(T) first, IEnumerable(T) second)                     \
{                                                                                                       \
    ConcatEnumerable_##T result = meminit(ConcatEnumerable_##T) {                                       \
        .GetEnumerator = GetConcatEnumerator_##T,                                                       \
        ._firstEnumerable = first,                                                                      \
        ._secondEnumerable = second                                                                     \
    };                                                                                                  \
    return (IEnumerable(T))result;                                                                      \
}                                                                                                       \
T Enumerable_##T##_ElementAt(IEnumerable(T) source, int index)                                          \
{                                                                                                       \
    if (index < 0) return default(T);                                                                   \
    IEnumerator(T) e = source->GetEnumerator(source);                                                   \
    while(e->MoveNext(e) && index > 0) { index -= 1; }                                                  \
    T item = index ? default(T) : e->Current;                                                           \
    e->Dispose(e);                                                                                      \
    return item;                                                                                        \
}                                                                                                       \
bool Enumerable_##T##_Any(IEnumerable(T) source, bool (*predicate)(T))                                  \
{                                                                                                       \
    IEnumerator(T) e = source->GetEnumerator(source);                                                   \
    while(e->MoveNext(e)) {                                                                             \
        if (predicate(e->Current)) {                                                                    \
            e->Dispose(e);                                                                              \
            return true;                                                                                \
        }                                                                                               \
    }                                                                                                   \
    e->Dispose(e);                                                                                      \
    return false;                                                                                       \
}                                                                                                       \
bool Enumerable_##T##_All(IEnumerable(T) source, bool (*predicate)(T))                                  \
{                                                                                                       \
    IEnumerator(T) e = source->GetEnumerator(source);                                                   \
    while(e->MoveNext(e)) {                                                                             \
        if (!predicate(e->Current)) {                                                                   \
            e->Dispose(e);                                                                              \
            return false;                                                                               \
        }                                                                                               \
    }                                                                                                   \
    e->Dispose(e);                                                                                      \
    return true;                                                                                        \
}                                                                                                       \
int Enumerable_##T##_IndexOf(IEnumerable(T) source, T item)                                             \
{                                                                                                       \
    IEnumerator(T) e = source->GetEnumerator(source);                                                   \
    for (int i = 0; e->MoveNext(e); ++i) {                                                              \
        if (equals(e->Current, item)) {                                                                 \
            e->Dispose(e);                                                                              \
            return i;                                                                                   \
        }                                                                                               \
    }                                                                                                   \
    e->Dispose(e);                                                                                      \
    return -1;                                                                                          \
}                                                                                                       \
T Enumerable_##T##_FirstOrDefault(IEnumerable(T) source, bool (*predicate)(T))                          \
{                                                                                                       \
    IEnumerator(T) e = source->GetEnumerator(source);                                                   \
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
bool Enumerable_##T##_Contains(IEnumerable(T) source, T item)                                           \
{                                                                                                       \
    IEnumerator(T) e = source->GetEnumerator(source);                                                   \
    while (e->MoveNext(e)) {                                                                            \
        if (equals(e->Current, item)) {                                                                 \
            e->Dispose(e);                                                                              \
            return true;                                                                                \
        }                                                                                               \
    }                                                                                                   \
    e->Dispose(e);                                                                                      \
    return false;                                                                                       \
}                                                                                                       \
int Enumerable_##T##_Count(IEnumerable(T) source)                                                       \
{                                                                                                       \
    IEnumerator(T) e = source->GetEnumerator(source);                                                   \
    int i = 0;                                                                                          \
    while (e->MoveNext(e)) {                                                                            \
        i += 1;                                                                                         \
    }                                                                                                   \
    e->Dispose(e);                                                                                      \
    return i;                                                                                           \
}                                                                                                       \
bool Enumerable_##T##_SequenceEqual(IEnumerable(T) first, IEnumerable(T) second)                        \
{                                                                                                       \
    IEnumerator(T) e1 = first->GetEnumerator(first);                                                    \
    IEnumerator(T) e2 = second->GetEnumerator(second);                                                  \
    while (e1->MoveNext(e1) & e2->MoveNext(e2)) {                                                       \
        if (!equals(e1->Current, e2->Current)) {                                                        \
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
}

#pragma region Converters

#define ENUMERABLE_IMPLEMENT_INDEX(T)                                                                   \
typedef TAG(IndexEnumerable_##T) {                                                                      \
    IMPL(IEnumerable(t(int,T)));                                                                        \
    IEnumerable(T) _baseEnumerable;                                                                     \
} *IndexEnumerable_##T;                                                                                 \
typedef TAG(IndexEnumerator_##T) {                                                                      \
    IMPL(IEnumerator(t(int,T)));                                                                        \
    IEnumerator(T) _currentEnumerator;                                                                  \
    int _currentIndex;                                                                                  \
} *IndexEnumerator_##T;                                                                                 \
static bool IndexMoveNext_##T(IEnumerator(t(int,T)) This)                                               \
{                                                                                                       \
    IndexEnumerator_##T index = (IndexEnumerator_##T)This;                                              \
    if (!index->_currentEnumerator->MoveNext(index->_currentEnumerator)) return false;                  \
    index->_currentIndex += 1;                                                                          \
    This->Current = new(int_##T)(index->_currentIndex, index->_currentEnumerator->Current);             \
    return true;                                                                                        \
}                                                                                                       \
static void IndexReset_##T(IEnumerator(t(int,T)) This)                                                  \
{                                                                                                       \
    ((IndexEnumerator_##T)This)->_currentIndex = -1;                                                    \
    CompoundReset_int_##T(This);                                                                        \
}                                                                                                       \
static IEnumerator(t(int,T)) GetIndexEnumerator_##T(IEnumerable(t(int,T)) This)                         \
{                                                                                                       \
    IndexEnumerable_##T index = (IndexEnumerable_##T)This;                                              \
    IndexEnumerator_##T result = meminit(IndexEnumerator_##T) {                                         \
        .MoveNext = IndexMoveNext_##T,                                                                  \
        .Reset = IndexReset_##T,                                                                        \
        .Dispose = CompoundDispose_int_##T,                                                             \
        ._currentEnumerator = index->_baseEnumerable->                                                  \
            GetEnumerator(index->_baseEnumerable),                                                      \
        ._currentIndex = -1                                                                             \
    };                                                                                                  \
    return (IEnumerator(t(int,T)))result;                                                               \
}                                                                                                       \
IEnumerable(t(int,T)) Enumerable_##T##_Index(IEnumerable(T) source)                                     \
{                                                                                                       \
    IndexEnumerable_##T result = meminit(IndexEnumerable_##T) {                                         \
        .GetEnumerator = GetIndexEnumerator_##T,                                                        \
        ._baseEnumerable = source                                                                       \
    };                                                                                                  \
    return (IEnumerable(t(int,T)))result;                                                               \
}

#define ENUMERABLE_IMPLEMENT_SELECT(TSource, TResult)                                                   \
typedef TAG(SelectEnumerable_##TSource##_##TResult) {                                                   \
    IMPL(IEnumerable(TResult));                                                                         \
    IEnumerable(TSource) _baseEnumerable;                                                               \
    TResult (*_selector)(TSource);                                                                      \
} *SelectEnumerable_##TSource##_##TResult;                                                              \
typedef TAG(SelectEnumerator_##TSource##_##TResult) {                                                   \
    IMPL(IEnumerator(TResult));                                                                         \
    IEnumerator(TSource) _currentEnumerator;                                                            \
    SelectEnumerable_##TSource##_##TResult _baseEnumerable;                                             \
} *SelectEnumerator_##TSource##_##TResult;                                                              \
static bool SelectMoveNext_##TSource##_##TResult(IEnumerator(TResult) This)                             \
{                                                                                                       \
    SelectEnumerator_##TSource##_##TResult select = (SelectEnumerator_##TSource##_##TResult)This;       \
    if (select->_currentEnumerator->MoveNext(select->_currentEnumerator)) {                             \
        This->Current = ((SelectEnumerable_##TSource##_##TResult)select->_baseEnumerable)->             \
            _selector(select->_currentEnumerator->Current);                                             \
        return true;                                                                                    \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static void SelectReset_##TSource##_##TResult(IEnumerator(TResult) This)                                \
{                                                                                                       \
    This->Current = (TResult)0;                                                                         \
    SelectEnumerator_##TSource##_##TResult e = (SelectEnumerator_##TSource##_##TResult)This;            \
    e->_currentEnumerator->Reset(e->_currentEnumerator);                                                \
}                                                                                                       \
static void SelectDispose_##TSource##_##TResult(IEnumerator(TResult) This)                              \
{                                                                                                       \
    SelectEnumerator_##TSource##_##TResult e = (SelectEnumerator_##TSource##_##TResult)This;            \
    e->_currentEnumerator->Dispose(e->_currentEnumerator);                                              \
    memfree(This);                                                                                      \
}                                                                                                       \
static IEnumerator(TResult) GetSelectEnumerator_##TSource##_##TResult(IEnumerable(TResult) This)        \
{                                                                                                       \
    SelectEnumerable_##TSource##_##TResult select = (SelectEnumerable_##TSource##_##TResult)This;       \
    SelectEnumerator_##TSource##_##TResult                                                              \
        result = meminit(SelectEnumerator_##TSource##_##TResult) {                                      \
        .MoveNext = SelectMoveNext_##TSource##_##TResult,                                               \
        .Reset = SelectReset_##TSource##_##TResult,                                                     \
        .Dispose = SelectDispose_##TSource##_##TResult,                                                 \
        ._currentEnumerator = select->_baseEnumerable->GetEnumerator(select->_baseEnumerable),          \
        ._baseEnumerable = select                                                                       \
    };                                                                                                  \
    return (IEnumerator(TResult))(result);                                                              \
}                                                                                                       \
IEnumerable(TResult) Enumerable_##TSource##_Select_##TResult(IEnumerable(TSource) source, TResult (*selector)(TSource)) \
{                                                                                                       \
    SelectEnumerable_##TSource##_##TResult                                                              \
        result = meminit(SelectEnumerable_##TSource##_##TResult) {                                      \
        .GetEnumerator = GetSelectEnumerator_##TSource##_##TResult,                                     \
        ._selector = selector,                                                                          \
        ._baseEnumerable = source                                                                       \
    };                                                                                                  \
    return (IEnumerable(TResult))(result);                                                              \
}                                                                                                       \
typedef TAG(SelectIndexEnumerable_##TSource##_##TResult) {                                              \
    IMPL(IEnumerable(TResult));                                                                         \
    IEnumerable(TSource) _baseEnumerable;                                                               \
    TResult (*_selector)(TSource, int);                                                                 \
} *SelectIndexEnumerable_##TSource##_##TResult;                                                         \
typedef TAG(SelectIndexEnumerator_##TSource##_##TResult) {                                              \
    IMPL(IEnumerator(TResult));                                                                         \
    IEnumerator(TSource) _currentEnumerator;                                                            \
    SelectIndexEnumerable_##TSource##_##TResult _baseEnumerable;                                        \
    int _currentIndex;                                                                                  \
} *SelectIndexEnumerator_##TSource##_##TResult;                                                         \
static bool SelectIndexMoveNext_##TSource##_##TResult(IEnumerator(TResult) This)                        \
{                                                                                                       \
    SelectIndexEnumerator_##TSource##_##TResult select =                                                \
        (SelectIndexEnumerator_##TSource##_##TResult)This;                                              \
    if (select->_currentEnumerator->MoveNext(select->_currentEnumerator)) {                             \
        select->_currentIndex += 1;                                                                     \
        This->Current = ((SelectIndexEnumerable_##TSource##_##TResult)select->_baseEnumerable)->        \
            _selector(select->_currentEnumerator->Current, select->_currentIndex);                      \
        return true;                                                                                    \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static void SelectIndexReset_##TSource##_##TResult(IEnumerator(TResult) This)                           \
{                                                                                                       \
    This->Current = default(TResult);                                                                   \
    SelectIndexEnumerator_##TSource##_##TResult e = (SelectIndexEnumerator_##TSource##_##TResult)This;  \
    e->_currentEnumerator->Reset(e->_currentEnumerator);                                                \
    e->_currentIndex = -1;                                                                              \
}                                                                                                       \
static void SelectIndexDispose_##TSource##_##TResult(IEnumerator(TResult) This)                         \
{                                                                                                       \
    SelectIndexEnumerator_##TSource##_##TResult e = (SelectIndexEnumerator_##TSource##_##TResult)This;  \
    e->_currentEnumerator->Dispose(e->_currentEnumerator);                                              \
    memfree(This);                                                                                      \
}                                                                                                       \
static IEnumerator(TResult) GetSelectIndexEnumerator_##TSource##_##TResult(IEnumerable(TResult) This)   \
{                                                                                                       \
    SelectIndexEnumerable_##TSource##_##TResult select =                                                \
        (SelectIndexEnumerable_##TSource##_##TResult)This;                                              \
    SelectIndexEnumerator_##TSource##_##TResult                                                         \
        result = meminit(SelectIndexEnumerator_##TSource##_##TResult) {                                 \
        .MoveNext = SelectIndexMoveNext_##TSource##_##TResult,                                          \
        .Reset = SelectIndexReset_##TSource##_##TResult,                                                \
        .Dispose = SelectIndexDispose_##TSource##_##TResult,                                            \
        ._currentEnumerator = select->_baseEnumerable->GetEnumerator(select->_baseEnumerable),          \
        ._baseEnumerable = select,                                                                      \
        ._currentIndex = -1                                                                             \
    };                                                                                                  \
    return (IEnumerator(TResult))(result);                                                              \
}                                                                                                       \
IEnumerable(TResult) Enumerable_##TSource##_SelectIndex_##TResult(IEnumerable(TSource) source, TResult (*selector)(TSource, int)) \
{                                                                                                       \
    SelectIndexEnumerable_##TSource##_##TResult                                                         \
        result = meminit(SelectIndexEnumerable_##TSource##_##TResult) {                                 \
        .GetEnumerator = GetSelectIndexEnumerator_##TSource##_##TResult,                                \
        ._selector = selector,                                                                          \
        ._baseEnumerable = source                                                                       \
    };                                                                                                  \
    return (IEnumerable(TResult))(result);                                                              \
}                                                                                                       \
typedef TAG(SelectManyEnumerable_##TSource##_##TResult) {                                               \
    IMPL(IEnumerable(TResult));                                                                         \
    IEnumerable(TSource) _baseEnumerable;                                                               \
    IEnumerable(TResult) (*_selector)(TSource);                                                         \
} *SelectManyEnumerable_##TSource##_##TResult;                                                          \
typedef TAG(SelectManyEnumerator_##TSource##_##TResult) {                                               \
    IMPL(IEnumerator(TResult));                                                                         \
    IEnumerator(TSource) _innerEnumerator;                                                              \
    IEnumerator(TResult) _outerEnumerator;                                                              \
    SelectManyEnumerable_##TSource##_##TResult _baseEnumerable;                                         \
} *SelectManyEnumerator_##TSource##_##TResult;                                                          \
static bool SelectManyMoveNext_##TSource##_##TResult(IEnumerator(TResult) This)                         \
{                                                                                                       \
    SelectManyEnumerator_##TSource##_##TResult selectMany =                                             \
        (SelectManyEnumerator_##TSource##_##TResult)This;                                               \
    if (selectMany->_outerEnumerator != NULL &&                                                         \
        selectMany->_outerEnumerator->MoveNext(selectMany->_outerEnumerator)) {                         \
        This->Current = selectMany->_outerEnumerator->Current;                                          \
        return true;                                                                                    \
    } else if (selectMany->_innerEnumerator->MoveNext(selectMany->_innerEnumerator)) {                  \
        if (selectMany->_outerEnumerator != NULL) {                                                     \
            selectMany->_outerEnumerator->Dispose(selectMany->_outerEnumerator);                        \
        }                                                                                               \
        IEnumerable(TResult) result = ((SelectManyEnumerable_##TSource##_##TResult)selectMany->         \
            _baseEnumerable)->_selector(selectMany->_innerEnumerator->Current);                         \
        selectMany->_outerEnumerator = result->GetEnumerator(result);                                   \
        return SelectManyMoveNext_##TSource##_##TResult(This);                                          \
    }                                                                                                   \
    return false;                                                                                       \
}                                                                                                       \
static void SelectManyReset_##TSource##_##TResult(IEnumerator(TResult) This)                            \
{                                                                                                       \
    SelectManyEnumerator_##TSource##_##TResult selectMany =                                             \
        (SelectManyEnumerator_##TSource##_##TResult)This;                                               \
    if (selectMany->_outerEnumerator != NULL)                                                           \
        selectMany->_outerEnumerator->Dispose(selectMany->_outerEnumerator);                            \
    selectMany->_outerEnumerator = NULL;                                                                \
    selectMany->_innerEnumerator->Reset(selectMany->_innerEnumerator);                                  \
}                                                                                                       \
static void SelectManyDispose_##TSource##_##TResult(IEnumerator(TResult) This)                          \
{                                                                                                       \
    SelectManyEnumerator_##TSource##_##TResult selectMany =                                             \
        (SelectManyEnumerator_##TSource##_##TResult)This;                                               \
    if (selectMany->_outerEnumerator != NULL)                                                           \
        selectMany->_outerEnumerator->Dispose(selectMany->_outerEnumerator);                            \
    selectMany->_innerEnumerator->Dispose(selectMany->_innerEnumerator);                                \
    memfree(This);                                                                                      \
}                                                                                                       \
static IEnumerator(TResult) GetSelectManyEnumerator_##TSource##_##TResult(IEnumerable(TResult) This)    \
{                                                                                                       \
    SelectManyEnumerable_##TSource##_##TResult selectMany =                                             \
        (SelectManyEnumerable_##TSource##_##TResult)This;                                               \
    SelectManyEnumerator_##TSource##_##TResult result =                                                 \
        meminit(SelectManyEnumerator_##TSource##_##TResult) {                                           \
        .MoveNext = SelectManyMoveNext_##TSource##_##TResult,                                           \
        .Reset = SelectManyReset_##TSource##_##TResult,                                                 \
        .Dispose = SelectManyDispose_##TSource##_##TResult,                                             \
        ._innerEnumerator = selectMany->_baseEnumerable->GetEnumerator(selectMany->_baseEnumerable),    \
        ._baseEnumerable = selectMany                                                                   \
    };                                                                                                  \
    return (IEnumerator(TResult))(result);                                                              \
}                                                                                                       \
IEnumerable(TResult) Enumerable_##TSource##_SelectMany_##TResult(IEnumerable(TSource) source, IEnumerable(TResult) (*selector)(TSource)) \
{                                                                                                       \
    SelectManyEnumerable_##TSource##_##TResult                                                          \
        result = meminit(SelectManyEnumerable_##TSource##_##TResult) {                                  \
        .GetEnumerator = GetSelectManyEnumerator_##TSource##_##TResult,                                 \
        ._selector = selector,                                                                          \
        ._baseEnumerable = source                                                                       \
    };                                                                                                  \
    return (IEnumerable(TResult))(result);                                                              \
}

#define ENUMERABLE_IMPLEMENT_AGGREGATE(TSource, TAggregate)         \
TAggregate Enumerable_##TSource##_Aggregate_##TAggregate(IEnumerable(TSource) source, TAggregate seed, TAggregate (*aggregate)(TAggregate, TSource)) \
{                                                                   \
    foreach (TSource item in source) {                              \
        seed = aggregate(seed, item);                               \
    }                                                               \
    return seed;                                                    \
}

#pragma endregion

#endif