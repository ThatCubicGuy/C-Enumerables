#ifndef COLLECTIONS_GENERIC_HASH_SET_IMPLEMENTATIONS
#define COLLECTIONS_GENERIC_HASH_SET_IMPLEMENTATIONS
#include "HashSetT.h"
#include "Keywords.h"

#define HASH_SET_IMPLEMENT(T)                                                                   \
TAG(HashSetEntry_##T) {                                                                         \
    HashSetEntry_##T Next;                                                                      \
    T Value;                                                                                    \
    size_t Hash;                                                                                \
};                                                                                              \
typedef TAG(HashSetEnumerator_##T) {                                                            \
    IMPL(IEnumerator(T));                                                                       \
    HashSetEntry_##T _currentNode;                                                              \
    int _currentIndex;                                                                          \
    HashSet(T) _set;                                                                            \
} *HashSetEnumerator_##T;                                                                       \
static bool HashSetMoveNext_##T(IEnumerator(T) This)                                            \
{                                                                                               \
    HashSetEnumerator_##T e = (HashSetEnumerator_##T)This;                                      \
    while (!e->_currentNode && e->_currentIndex < MAX_HASH_SET_ARRAY_LENGTH) {                  \
        e->_currentNode = e->_set->_items[e->_currentIndex];                                    \
        e->_currentIndex += 1;                                                                  \
    }                                                                                           \
    if (e->_currentIndex == MAX_HASH_SET_ARRAY_LENGTH) return false;                            \
    This->Current = e->_currentNode->Value;                                                     \
    e->_currentNode = e->_currentNode->Next;                                                    \
    return true;                                                                                \
}                                                                                               \
static void HashSetReset_##T(IEnumerator(T) This)                                               \
{                                                                                               \
    HashSetEnumerator_##T e = (HashSetEnumerator_##T)This;                                      \
    e->_currentIndex = 0;                                                                       \
    e->_currentNode = NULL;                                                                     \
    This->Current = default(T);                                                                 \
}                                                                                               \
static void HashSetDispose_##T(IEnumerator(T) This)                                             \
{                                                                                               \
    memfree(This);                                                                              \
}                                                                                               \
static IEnumerator(T) HashSetGetEnumerator_##T(IEnumerable(T) This)                             \
{                                                                                               \
    HashSetEnumerator_##T result = meminit(HashSetEnumerator_##T) {                             \
        .MoveNext = HashSetMoveNext_##T,                                                        \
        .Reset = HashSetReset_##T,                                                              \
        .Dispose = HashSetDispose_##T,                                                          \
        ._currentIndex = 0,                                                                     \
        ._currentNode = NULL,                                                                   \
        ._set = (HashSet(T))This                                                                \
    };                                                                                          \
    return (IEnumerator(T))result;                                                              \
}                                                                                               \
void HashSet_##T##_Add(HashSet(T) source, T item)                                               \
{                                                                                               \
    ThrowIfNull(source);                                                                        \
    HashSetEntry_##T node = meminit(HashSetEntry_##T) {                                         \
        .Hash = source->Comparer->GetHashCode(item),                                            \
        .Value = item,                                                                          \
        .Next = NULL                                                                            \
    };                                                                                          \
    size_t index = node->Hash % MAX_HASH_SET_ARRAY_LENGTH;                                      \
    HashSetEntry_##T *bucket = &source->_items[index];                                          \
    while ((*bucket) && !(source->Comparer->Equals((*bucket)->Value, item))) {                  \
        bucket = &(*bucket)->Next;                                                              \
    }                                                                                           \
    if (*bucket) {                                                                              \
        node->Next = (*bucket)->Next;                                                           \
        memfree(*bucket);                                                                       \
    } else source->Count += 1;                                                                  \
    *bucket = node;                                                                             \
}                                                                                               \
bool HashSet_##T##_Remove(HashSet(T) source, T item)                                            \
{                                                                                               \
    ThrowIfNull(source);                                                                        \
    size_t hash = source->Comparer->GetHashCode(item);                                          \
    size_t index = hash % MAX_HASH_SET_ARRAY_LENGTH;                                            \
    HashSetEntry_##T bucket = source->_items[index], old = bucket;                              \
    if (!bucket) return false;                                                                  \
    if (source->Comparer->Equals(bucket->Value, item)) {                                        \
        source->Count -= 1;                                                                     \
        source->_items[index] = bucket->Next;                                                   \
        memfree(bucket);                                                                        \
        return true;                                                                            \
    } else for (bucket = bucket->Next; bucket; old = bucket, bucket = bucket->Next) {           \
        if (source->Comparer->Equals(bucket->Value, item)) {                                    \
            source->Count -= 1;                                                                 \
            old->Next = bucket->Next;                                                           \
            memfree(bucket);                                                                    \
            return true;                                                                        \
        }                                                                                       \
    }                                                                                           \
    return false;                                                                               \
}                                                                                               \
void HashSet_##T##_RemoveWhere(HashSet(T) source, bool (*predicate)(T))                         \
{                                                                                               \
    ThrowIfNull(source, predicate);                                                             \
    for (size_t i = 0; i < MAX_HASH_SET_ARRAY_LENGTH; ++i) {                                    \
        HashSetEntry_##T bucket = source->_items[i], old = bucket;                              \
        while (bucket && predicate(bucket->Value)) {                                            \
            source->Count -= 1;                                                                 \
            source->_items[i] = bucket->Next;                                                   \
            memfree(bucket);                                                                    \
            old = bucket = source->_items[i];                                                   \
        }                                                                                       \
        if (!bucket) continue;                                                                  \
        for (bucket = bucket->Next; bucket; old = bucket, bucket = bucket->Next) {              \
            while (bucket && predicate(bucket->Value)) {                                        \
                source->Count -= 1;                                                             \
                old->Next = bucket->Next;                                                       \
                memfree(bucket);                                                                \
                bucket = old->Next;                                                             \
            }                                                                                   \
        }                                                                                       \
    }                                                                                           \
}                                                                                               \
static void RemoveNodes_##T(HashSetEntry_##T start)                                             \
{                                                                                               \
    if (start) {                                                                                \
        RemoveNodes_##T(start->Next);                                                           \
        memfree(start);                                                                         \
    }                                                                                           \
}                                                                                               \
void HashSet_##T##_Clear(HashSet(T) source)                                                     \
{                                                                                               \
    ThrowIfNull(source);                                                                        \
    for (int i = 0; i < MAX_HASH_SET_ARRAY_LENGTH; ++i) {                                       \
        RemoveNodes_##T(source->_items[i]);                                                     \
        source->_items[i] = NULL;                                                               \
    }                                                                                           \
    source->Count = 0;                                                                          \
}                                                                                               \
bool HashSet_##T##_Contains(HashSet(T) source, T item)                                          \
{                                                                                               \
    ThrowIfNull(source);                                                                        \
    size_t hash = source->Comparer->GetHashCode(item);                                          \
    size_t index = hash % MAX_HASH_SET_ARRAY_LENGTH;                                            \
    HashSetEntry_##T bucket = source->_items[index];                                            \
    while (bucket) {                                                                            \
        if (source->Comparer->Equals(bucket->Value, item)) return true;                         \
        bucket = bucket->Next;                                                                  \
    }                                                                                           \
    return false;                                                                               \
}                                                                                               \
HashSet(T) new(HashSet(T))(IEqualityComparer(T) comparer)                                       \
{                                                                                               \
    ThrowIfNull(comparer);                                                                      \
    HashSet(T) result = meminit(HashSet(T)) {                                                   \
        .GetEnumerator = HashSetGetEnumerator_##T,                                              \
        .Comparer = comparer,                                                                   \
        .Count = 0,                                                                             \
    };                                                                                          \
    return result;                                                                              \
}                                                                                               \
void HashSet_##T##_Destroy(HashSet(T)* set)                                                     \
{                                                                                               \
    ThrowIfNull(set, *set);                                                                     \
    HashSet_##T##_Clear(*set);                                                                  \
    memfree(*set);                                                                              \
    *set = NULL;                                                                                \
}                                                                                               \
HashSet(T) Enumerable_##T##_ToHashSet(IEnumerable(T) source, IEqualityComparer(T) comparer)     \
{                                                                                               \
    ThrowIfNull(source, comparer);                                                              \
    HashSet(T) result = new(HashSet(T))(comparer);                                              \
    foreach (T item in source) {                                                                \
        HashSet_##T##_Add(result, item);                                                        \
    }                                                                                           \
    return result;                                                                              \
}

#endif