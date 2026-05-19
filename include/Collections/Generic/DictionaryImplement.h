#ifndef COLLECTIONS_GENERIC_DICTIONARY_IMPLEMENTATIONS
#define COLLECTIONS_GENERIC_DICTIONARY_IMPLEMENTATIONS
#include "DictionaryT.h"
#define DICTIONARY_IMPLEMENT(TKey,TValue)                                                                                   \
TAG(DictEntry_##TKey##_##TValue) {                                                                                          \
    DictEntry_##TKey##_##TValue Next;                                                                                       \
    unsigned long Hash;                                                                                                     \
    TKey Key;                                                                                                               \
    TValue Value;                                                                                                           \
};                                                                                                                          \
Dictionary(TKey,TValue) new(Dictionary(TKey,TValue))(IEqualityComparer(TKey) comparer)                                      \
{                                                                                                                           \
    Dictionary(TKey,TValue) result = meminit(Dictionary(TKey,TValue)) {                                                     \
        .Comparer = comparer,                                                                                               \
        .Count = 0,                                                                                                         \
        ._items = arralloc(DictEntry_##TKey##_##TValue, MAX_DICTIONARY_ARRAY_LENGTH)                                        \
    };                                                                                                                      \
    return result;                                                                                                          \
}                                                                                                                           \
void Dictionary_##TKey##_##TValue##_Add(Dictionary(TKey,TValue) source, TKey key, TValue value)                             \
{                                                                                                                           \
    auto node = meminit(DictEntry_##TKey##_##TValue) {                                                                      \
        .Hash = source->Comparer->GetHashCode(key),                                                                         \
        .Key = key,                                                                                                         \
        .Value = value,                                                                                                     \
        .Next = NULL                                                                                                        \
    };                                                                                                                      \
    int index = node->Hash % MAX_DICTIONARY_ARRAY_LENGTH;                                                                   \
    auto bucket = &source->_items[index];                                                                                   \
    while (*bucket && ((*bucket)->Hash != node->Hash                                                                        \
        || !source->Comparer->Equals((*bucket)->Key, key)))                                                                 \
    {                                                                                                                       \
        bucket = &(*bucket)->Next;                                                                                          \
    }                                                                                                                       \
    if (!(*bucket)) source->Count += 1;                                                                                     \
    else if ((*bucket)->Next) node->Next = (*bucket)->Next;                                                                 \
    *bucket = node;                                                                                                         \
}                                                                                                                           \
TValue Dictionary_##TKey##_##TValue##_Get(Dictionary(TKey,TValue) source, TKey key)                                         \
{                                                                                                                           \
    unsigned long index = source->Comparer->GetHashCode(key) % MAX_DICTIONARY_ARRAY_LENGTH;                                 \
    auto bucket = source->_items[index];                                                                                    \
    while (bucket && !source->Comparer->Equals(bucket->Key, key)) {                                                         \
        bucket = bucket->Next;                                                                                              \
    }                                                                                                                       \
    if (!bucket) return default(TValue);                                                                                    \
    return bucket->Value;                                                                                                   \
}                                                                                                                           \
bool Dictionary_##TKey##_##TValue##_Remove(Dictionary(TKey,TValue) source, TKey key)                                        \
{                                                                                                                           \
    unsigned long index = source->Comparer->GetHashCode(key) % MAX_DICTIONARY_ARRAY_LENGTH;                                 \
    auto bucket = source->_items[index];                                                                                    \
    if (source->Comparer->Equals(bucket->Key, key)) {                                                                       \
        source->_items[index] = bucket->Next;                                                                               \
        memfree(bucket);                                                                                                    \
        return true;                                                                                                        \
    } else {                                                                                                                \
        auto old = bucket;                                                                                                  \
        for (bucket = bucket->Next; bucket; old = bucket, bucket = bucket->Next) {                                          \
            if (source->Comparer->Equals(bucket->Key, key)) {                                                               \
                old->Next = bucket->Next;                                                                                   \
                memfree(bucket);                                                                                            \
                return true;                                                                                                \
            }                                                                                                               \
        }                                                                                                                   \
    }                                                                                                                       \
    return false;                                                                                                           \
}                                                                                                                           \
static void RemoveNodes(DictEntry_##TKey##_##TValue start)                                                                  \
{                                                                                                                           \
    if (!start) return;                                                                                                     \
    RemoveNodes(start->Next);                                                                                               \
    memfree(start);                                                                                                         \
}                                                                                                                           \
void Dictionary_##TKey##_##TValue##_Clear(Dictionary(TKey,TValue) source)                                                   \
{                                                                                                                           \
    for (int i = 0; i < source->Count; ++i) {                                                                               \
        RemoveNodes(source->_items[i]);                                                                                     \
    }                                                                                                                       \
}                                                                                                                           \
bool Dictionary_##TKey##_##TValue##_ContainsKey(Dictionary(TKey,TValue) source, TKey key)                                   \
{                                                                                                                           \
    unsigned long index = source->Comparer->GetHashCode(key) % MAX_DICTIONARY_ARRAY_LENGTH;                                 \
    auto bucket = source->_items[index];                                                                                    \
    while (bucket) {                                                                                                        \
        if (source->Comparer->Equals(bucket->Key, key)) return true;                                                        \
        bucket = bucket->Next;                                                                                              \
    }                                                                                                                       \
    return false;                                                                                                           \
}                                                                                                                           \
bool Dictionary_##TKey##_##TValue##_ContainsValue(Dictionary(TKey,TValue) source, TValue value)                             \
{                                                                                                                           \
    for (int i = 0; i < MAX_DICTIONARY_ARRAY_LENGTH; ++i) {                                                                 \
        for (auto bucket = source->_items[i]; bucket; bucket = bucket->Next) {                                              \
            if (equals(bucket->Value, value)) {                                                                             \
                return true;                                                                                                \
            }                                                                                                               \
        }                                                                                                                   \
    }                                                                                                                       \
    return false;                                                                                                           \
}                                                                                                                           \
bool Dictionary_##TKey##_##TValue##_TryGetValue(Dictionary(TKey,TValue) source, TKey key, TValue* out)                      \
{                                                                                                                           \
    unsigned long index = source->Comparer->GetHashCode(key) % MAX_DICTIONARY_ARRAY_LENGTH;                                 \
    auto bucket = source->_items[index];                                                                                    \
    while (bucket && !source->Comparer->Equals(bucket->Key, key)) {                                                         \
        bucket = bucket->Next;                                                                                              \
    }                                                                                                                       \
    if (!bucket) {                                                                                                          \
        *out = default(TValue);                                                                                             \
        return false;                                                                                                       \
    } else {                                                                                                                \
        *out = bucket->Value;                                                                                               \
        return true;                                                                                                        \
    }                                                                                                                       \
}                                                                                                                           \
typedef TAG(DictionaryValuesEnumerable_##TKey##_##TValue) {                                                                 \
    IMPL(IEnumerable(TValue));                                                                                              \
    Dictionary(TKey,TValue) dict;                                                                                           \
} *DictionaryValuesEnumerable_##TKey##_##TValue;                                                                            \
typedef TAG(DictionaryValuesEnumerator_##TKey##_##TValue) {                                                                 \
    IMPL(IEnumerator(TValue));                                                                                              \
    int _currentIndex;                                                                                                      \
    DictEntry_##TKey##_##TValue _currentNode;                                                                               \
    DictionaryValuesEnumerable_##TKey##_##TValue source;                                                                    \
} *DictionaryValuesEnumerator_##TKey##_##TValue;                                                                            \
static bool DictionaryValuesMoveNext_##TKey##_##TValue(IEnumerator(TValue) This)                                            \
{                                                                                                                           \
    auto e = (DictionaryValuesEnumerator_##TKey##_##TValue)This;                                                            \
    while (!e->_currentNode && e->_currentIndex < MAX_DICTIONARY_ARRAY_LENGTH) {                                            \
        e->_currentIndex += 1;                                                                                              \
        e->_currentNode = e->source->dict->_items[e->_currentIndex];                                                        \
    }                                                                                                                       \
    if (e->_currentIndex == MAX_DICTIONARY_ARRAY_LENGTH) return false;                                                      \
    This->Current = e->_currentNode->Value;                                                                                 \
    e->_currentNode = e->_currentNode->Next;                                                                                \
    return true;                                                                                                            \
}                                                                                                                           \
static void DictionaryValuesReset_##TKey##_##TValue(IEnumerator(TValue) This)                                               \
{                                                                                                                           \
    auto e = (DictionaryValuesEnumerator_##TKey##_##TValue)This;                                                            \
    e->_currentIndex = -1;                                                                                                  \
    e->_currentNode = NULL;                                                                                                 \
    This->Current = default(TValue);                                                                                        \
}                                                                                                                           \
static void DictionaryValuesDispose_##TKey##_##TValue(IEnumerator(TValue) This)                                             \
{                                                                                                                           \
    memfree(This);                                                                                                          \
}                                                                                                                           \
static IEnumerator(TValue) DictionaryValuesGetEnumerator_##TKey##_##TValue(IEnumerable(TValue) This)                        \
{                                                                                                                           \
    auto e = (DictionaryValuesEnumerable_##TKey##_##TValue)This;                                                            \
    auto result = meminit(DictionaryValuesEnumerator_##TKey##_##TValue) {                                                   \
        .MoveNext = DictionaryValuesMoveNext_##TKey##_##TValue,                                                             \
        .Reset = DictionaryValuesReset_##TKey##_##TValue,                                                                   \
        .Dispose = DictionaryValuesDispose_##TKey##_##TValue,                                                               \
        .Current = default(TValue),                                                                                         \
        ._currentIndex = -1,                                                                                                \
        ._currentNode = NULL,                                                                                               \
        .source = e                                                                                                         \
    };                                                                                                                      \
    return (IEnumerator(TValue))result;                                                                                     \
}                                                                                                                           \
IEnumerable(TValue) Dictionary_##TKey##_##TValue##_GetValues(Dictionary(TKey,TValue) source)                                \
{                                                                                                                           \
    auto result = meminit(DictionaryValuesEnumerable_##TKey##_##TValue) {                                                   \
        .GetEnumerator = DictionaryValuesGetEnumerator_##TKey##_##TValue,                                                   \
        .dict = source                                                                                                      \
    };                                                                                                                      \
    return (IEnumerable(TValue))result;                                                                                     \
}

#endif