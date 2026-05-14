#ifndef COLLECTIONS_GENERIC_HASH_SET_IMPLEMENTATIONS
#define COLLECTIONS_GENERIC_HASH_SET_IMPLEMENTATIONS

#define MAX_HASH_SET_ARRAY_LENGTH 64
#define HASH_SET_IMPLEMENT(T)                                                                   \
struct HashSetEntry_##T##_s {                                                                   \
    HashSetEntry_##T Next;                                                                      \
    T Value;                                                                                    \
    size_t Hash;                                                                                \
};                                                                                              \
typedef struct HashSetEnumerator_##T##_s {                                                      \
    struct _IEnumerator_##T##_s _parent;                                                        \
    HashSetEntry_##T _currentNode;                                                              \
    int _currentIndex;                                                                          \
    HashSet(T) _set;                                                                            \
} *HashSetEnumerator_##T;                                                                       \
static bool HashSetMoveNext_##T(IEnumerator(T) This)                                            \
{                                                                                               \
    HashSetEnumerator_##T e = (HashSetEnumerator_##T)This;                                      \
    while (!e->_currentNode && e->_currentIndex < MAX_HASH_SET_ARRAY_LENGTH) {                  \
        e->_currentIndex += 1;                                                                  \
        e->_currentNode = e->_set->Values[e->_currentIndex];                                    \
    }                                                                                           \
    if (e->_currentIndex == MAX_HASH_SET_ARRAY_LENGTH) return false;                            \
    This->Current = e->_currentNode->Value;                                                     \
    e->_currentNode = e->_currentNode->Next;                                                    \
    return true;                                                                                \
}                                                                                               \
static void HashSetReset_##T(IEnumerator(T) This)                                               \
{                                                                                               \
    HashSetEnumerator_##T e = (HashSetEnumerator_##T)This;                                      \
    e->_currentIndex = -1;                                                                      \
    e->_currentNode = NULL;                                                                     \
    This->Current = default(T);                                                                 \
}                                                                                               \
static void HashSetDispose_##T(IEnumerator(T) This)                                             \
{                                                                                               \
    free(This);                                                                                 \
}                                                                                               \
static IEnumerator(T) HashSetGetEnumerator_##T(IEnumerable(T) This)                             \
{                                                                                               \
    HashSetEnumerator_##T result = malloc(sizeof(struct HashSetEnumerator_##T##_s));            \
    *result = (typeof(*(HashSetEnumerator_##T)0)) {                                             \
        ._parent = (typeof(*(IEnumerator(T))0)) {                                               \
            .MoveNext = HashSetMoveNext_##T,                                                    \
            .Reset = HashSetReset_##T,                                                          \
            .Dispose = HashSetDispose_##T,                                                      \
            .Current = default(T)                                                               \
        },                                                                                      \
        ._currentIndex = -1,                                                                    \
        ._currentNode = NULL,                                                                   \
        ._set = (HashSet(T))This                                                                \
    };                                                                                          \
    return (IEnumerator(T))result;                                                              \
}                                                                                               \
void HashSet_##T##_Add(HashSet(T) source, T item)                                               \
{                                                                                               \
    HashSetEntry_##T node = malloc(sizeof(struct HashSetEntry_##T##_s));                        \
    *node = (typeof(*(HashSetEntry_##T)0)) {                                                    \
        .Hash = source->Comparer->GetHashCode(item),                                            \
        .Value = item,                                                                          \
        .Next = NULL                                                                            \
    };                                                                                          \
    size_t index = node->Hash % MAX_HASH_SET_ARRAY_LENGTH;                                      \
    HashSetEntry_##T *bucket = &source->Values[index];                                          \
    while (*bucket && !source->Comparer->Equals((*bucket)->Value, item)) {                      \
        bucket = &(*bucket)->Next;                                                              \
    }                                                                                           \
    if (*bucket) {                                                                              \
        node->Next = (*bucket)->Next;                                                           \
        free(*bucket);                                                                          \
    } else source->Count += 1;                                                                  \
    *bucket = node;                                                                             \
}                                                                                               \
bool HashSet_##T##_Remove(HashSet(T) source, T item)                                            \
{                                                                                               \
    size_t hash = source->Comparer->GetHashCode(item);                                          \
    size_t index = hash % MAX_HASH_SET_ARRAY_LENGTH;                                            \
    HashSetEntry_##T bucket = source->Values[index], old = bucket;                              \
    if (!bucket) return false;                                                                  \
    if (source->Comparer->Equals(bucket->Value, item)) {                                        \
        source->Count -= 1;                                                                     \
        source->Values[index] = bucket->Next;                                                   \
        free(bucket);                                                                           \
        return true;                                                                            \
    } else for (bucket = bucket->Next; bucket; bucket = bucket->Next) {                         \
        if (source->Comparer->Equals(bucket->Value, item)) {                                    \
            source->Count -= 1;                                                                 \
            old->Next = bucket->Next;                                                           \
            free(bucket);                                                                       \
            return true;                                                                        \
        }                                                                                       \
    }                                                                                           \
    return false;                                                                               \
}                                                                                               \
void HashSet_##T##_RemoveWhere(HashSet(T) source, bool (*predicate)(T))                         \
{                                                                                               \
    for (size_t i = 0; i < MAX_HASH_SET_ARRAY_LENGTH; ++i) {                                    \
        HashSetEntry_##T bucket = source->Values[i], old = bucket;                              \
        while (bucket && predicate(bucket->Value)) {                                            \
            source->Count -= 1;                                                                 \
            source->Values[i] = bucket->Next;                                                   \
            free(bucket);                                                                       \
            old = bucket = source->Values[i];                                                   \
        }                                                                                       \
        if (!bucket) continue;                                                                  \
        for (bucket = bucket->Next; bucket; old = bucket, bucket ? bucket = bucket->Next : 0) { \
            while (bucket && predicate(bucket->Value)) {                                        \
                source->Count -= 1;                                                             \
                old->Next = bucket->Next;                                                       \
                free(bucket);                                                                   \
                bucket = old->Next;                                                             \
            }                                                                                   \
        }                                                                                       \
    }                                                                                           \
}                                                                                               \
static void RemoveNodes_##T(HashSetEntry_##T start)                                             \
{                                                                                               \
    if (start) RemoveNodes_##T(start->Next);                                                    \
    free(start);                                                                                \
}                                                                                               \
void HashSet_##T##_Clear(HashSet(T) source)                                                     \
{                                                                                               \
    for (int i = 0; i < MAX_HASH_SET_ARRAY_LENGTH; ++i) {                                       \
        RemoveNodes_##T(source->Values[i]);                                                     \
        source->Values[i] = NULL;                                                               \
    }                                                                                           \
    source->Count = 0;                                                                          \
}                                                                                               \
bool HashSet_##T##_Contains(HashSet(T) source, T item)                                          \
{                                                                                               \
    size_t hash = source->Comparer->GetHashCode(item);                                          \
    size_t index = hash % MAX_HASH_SET_ARRAY_LENGTH;                                            \
    HashSetEntry_##T bucket = source->Values[index];                                            \
    while (bucket) {                                                                            \
        if (source->Comparer->Equals(bucket->Value, item)) return true;                         \
        bucket = bucket->Next;                                                                  \
    }                                                                                           \
    return false;                                                                               \
}                                                                                               \
HashSet(T) new(HashSet(T))(IEqualityComparer(T) comparer)                                       \
{                                                                                               \
    HashSet(T) result = malloc(sizeof(struct HashSet_##T##_s));                                 \
    *result = (typeof(*(HashSet(T))0)) {                                                        \
        ._parent = (typeof(*(IEnumerable(T))0)) {                                               \
            .GetEnumerator = HashSetGetEnumerator_##T                                           \
        },                                                                                      \
        .Comparer = comparer,                                                                   \
        .Count = 0,                                                                             \
        .Values = {0}                                                                           \
    };                                                                                          \
    return result;                                                                              \
}                                                                                               \
void HashSet_##T##_Destroy(HashSet(T)* set)                                                     \
{                                                                                               \
    free(*set);                                                                                 \
    *set = NULL;                                                                                \
}                                                                                               \
HashSet(T) Enumerable_##T##_ToHashSet(IEnumerable(T) source)                                    \
{                                                                                               \
    HashSet(T) result = new(HashSet(T))((IEqualityComparer(T))ObjectEquator);                   \
    for (IEnumerator(T) e = source->GetEnumerator(source); e->MoveNext(e) || (e->Dispose(e), 0);) { \
        HashSet_##T##_Add(result, e->Current);                                                  \
    }                                                                                           \
    return result;                                                                              \
}

#endif