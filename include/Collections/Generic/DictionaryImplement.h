#ifndef COLLECTIONS_GENERIC_DICTIONARY_IMPLEMENTATIONS
#define COLLECTIONS_GENERIC_DICTIONARY_IMPLEMENTATIONS

#include "Collections/Generic/DictionaryT.h"
#include <stdlib.h>

#define DICTIONARY_IMPLEMENT(TKey,TValue)                                                                                   \
struct struct_DictEntry_##TKey##_##TValue {                                                                                 \
    DictEntry_##TKey##_##TValue Next;                                                                                       \
    unsigned long Hash;                                                                                                     \
    TKey Key;                                                                                                               \
    TValue Value;                                                                                                           \
};                                                                                                                          \
Dictionary(TKey,TValue) new(Dictionary(TKey,TValue))(int capacity, Func(unsigned long, TKey) hashCode, Func(int, TKey,TKey) comparer) \
{                                                                                                                           \
    auto allocinit(Dictionary(TKey,TValue), result) {                                                                       \
        .Compare = comparer,                                                                                                \
        .HashCode = hashCode,                                                                                               \
        .Capacity = capacity,                                                                                               \
        .Count = 0,                                                                                                         \
        ._items = alloc_array(DictEntry_##TKey##_##TValue, capacity)                                                        \
    };                                                                                                                      \
    return result;                                                                                                          \
}                                                                                                                           \
void Dictionary_##TKey##_##TValue##_Add(Dictionary(TKey,TValue) source, TKey key, TValue value)                             \
{                                                                                                                           \
    auto allocinit(DictEntry_##TKey##_##TValue, node) {                                                                     \
        .Hash = source->HashCode(key),                                                                                      \
        .Key = key,                                                                                                         \
        .Value = value,                                                                                                     \
        .Next = NULL                                                                                                        \
    };                                                                                                                      \
    int index = node->Hash % source->Capacity;                                                                              \
    auto bucket = &source->_items[index];                                                                                   \
    while (*bucket && ((*bucket)->Hash != node->Hash                                                                        \
        || source->Compare((*bucket)->Key, key)))                                                                           \
    {                                                                                                                       \
        bucket = &(*bucket)->Next;                                                                                          \
    }                                                                                                                       \
    if (!(*bucket)) source->Count += 1;                                                                                     \
    else if ((*bucket)->Next) node->Next = (*bucket)->Next;                                                                 \
    *bucket = node;                                                                                                         \
}                                                                                                                           \
TValue Dictionary_##TKey##_##TValue##_Get(Dictionary(TKey,TValue) source, TKey key)                                         \
{                                                                                                                           \
    unsigned long index = source->HashCode(key) % source->Capacity;                                                         \
    auto bucket = source->_items[index];                                                                                    \
    while (bucket && source->Compare(bucket->Key, key)) {                                                                   \
        bucket = bucket->Next;                                                                                              \
    }                                                                                                                       \
    if (!bucket) return default(TValue);                                                                                    \
    return bucket->Value;                                                                                                   \
}                                                                                                                           \
bool Dictionary_##TKey##_##TValue##_Remove(Dictionary(TKey,TValue) source, TKey key)                                        \
{                                                                                                                           \
    unsigned long index = source->HashCode(key) % source->Capacity;                                                         \
    auto bucket = source->_items[index];                                                                                    \
    if (!source->Compare(bucket->Key, key)) {                                                                               \
        source->_items[index] = bucket->Next;                                                                               \
        free(bucket);                                                                                                       \
        return true;                                                                                                        \
    } else {                                                                                                                \
        auto old = bucket;                                                                                                  \
        for (bucket = bucket->Next; bucket; old = bucket, bucket = bucket->Next) {                                          \
            if (!source->Compare(bucket->Key, key)) {                                                                       \
                old->Next = bucket->Next;                                                                                   \
                free(bucket);                                                                                               \
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
    free(start);                                                                                                            \
}                                                                                                                           \
void Dictionary_##TKey##_##TValue##_Clear(Dictionary(TKey,TValue) source)                                                   \
{                                                                                                                           \
    for (int i = 0; i < source->Count; ++i) {                                                                               \
        RemoveNodes(source->_items[i]);                                                                                     \
    }                                                                                                                       \
}                                                                                                                           \
bool Dictionary_##TKey##_##TValue##_ContainsKey(Dictionary(TKey,TValue) source, TKey key)                                   \
{                                                                                                                           \
    unsigned long index = source->HashCode(key) % source->Capacity;                                                         \
    auto bucket = source->_items[index];                                                                                    \
    while (bucket) {                                                                                                        \
        if (!source->Compare(bucket->Key, key)) return true;                                                                \
        bucket = bucket->Next;                                                                                              \
    }                                                                                                                       \
    return false;                                                                                                           \
}                                                                                                                           \
bool Dictionary_##TKey##_##TValue##_ContainsValue(Dictionary(TKey,TValue) source, TValue value)                             \
{                                                                                                                           \
    for (int i = 0; i < source->Capacity; ++i) {                                                                            \
        for (auto bucket = source->_items[i]; bucket; bucket = bucket->Next) {                                              \
            if (ValueEquator(sizeof(TValue), &bucket->Value, &value)) {                                                     \
                return true;                                                                                                \
            }                                                                                                               \
        }                                                                                                                   \
    }                                                                                                                       \
    return false;                                                                                                           \
}                                                                                                                           \
bool Dictionary_##TKey##_##TValue##_TryGetValue(Dictionary(TKey,TValue) source, TKey key, TValue* out)                      \
{                                                                                                                           \
    unsigned long index = source->HashCode(key) % source->Capacity;                                                         \
    auto bucket = source->_items[index];                                                                                    \
    while (bucket && source->Compare(bucket->Key, key)) {                                                                   \
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
typedef struct {                                                                                                            \
    refbase(IEnumerable(TValue)) _parent;                                                                                   \
    Dictionary(TKey,TValue) dict;                                                                                           \
} *DictionaryValuesEnumerable_##TKey##_##TValue;                                                                            \
typedef struct {                                                                                                            \
    refbase(IEnumerator(TValue)) _parent;                                                                                   \
    int _currentIndex;                                                                                                      \
    DictEntry_##TKey##_##TValue _currentNode;                                                                               \
    DictionaryValuesEnumerable_##TKey##_##TValue source;                                                                    \
} *DictionaryValuesEnumerator_##TKey##_##TValue;                                                                            \
static bool DictionaryValuesMoveNext_##TKey##_##TValue(IEnumerator(TValue) This)                                            \
{                                                                                                                           \
    auto e = (DictionaryValuesEnumerator_##TKey##_##TValue)This;                                                            \
    while (!e->_currentNode && e->_currentIndex < e->source->dict->Capacity) {                                              \
        e->_currentIndex += 1;                                                                                              \
        e->_currentNode = e->source->dict->_items[e->_currentIndex];                                                        \
    }                                                                                                                       \
    if (e->_currentIndex == e->source->dict->Capacity) return false;                                                        \
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
    free(This);                                                                                                             \
}                                                                                                                           \
static IEnumerator(TValue) DictionaryValuesGetEnumerator_##TKey##_##TValue(IEnumerable(TValue) This)                        \
{                                                                                                                           \
    auto e = (DictionaryValuesEnumerable_##TKey##_##TValue)This;                                                            \
    auto allocinit(DictionaryValuesEnumerator_##TKey##_##TValue, result) {                                                  \
        ._parent = {                                                                                                        \
            .MoveNext = DictionaryValuesMoveNext_##TKey##_##TValue,                                                         \
            .Reset = DictionaryValuesReset_##TKey##_##TValue,                                                               \
            .Dispose = DictionaryValuesDispose_##TKey##_##TValue,                                                           \
            .Current = default(TValue)                                                                                      \
        },                                                                                                                  \
        ._currentIndex = -1,                                                                                                \
        ._currentNode = NULL,                                                                                               \
        .source = e                                                                                                         \
    };                                                                                                                      \
    return (IEnumerator(TValue))result;                                                                                     \
}                                                                                                                           \
IEnumerable(TValue) Dictionary_##TKey##_##TValue##_GetValues(Dictionary(TKey,TValue) source)                                \
{                                                                                                                           \
    auto allocinit(DictionaryValuesEnumerable_##TKey##_##TValue, result) {                                                  \
        ._parent = {                                                                                                        \
            .GetEnumerator = DictionaryValuesGetEnumerator_##TKey##_##TValue                                                \
        },                                                                                                                  \
        .dict = source                                                                                                      \
    };                                                                                                                      \
    return (IEnumerable(TValue))result;                                                                                     \
}

#endif