#ifndef COLLECTIONS_GENERIC_DICTIONARY
#define COLLECTIONS_GENERIC_DICTIONARY
#include "Keywords.h"
#define Dictionary(TKey, TValue) CAT(Dictionary_,TKey,_,TValue)

#pragma region Define

#define MAX_DICTIONARY_ARRAY_LENGTH 64
#define DICTIONARY_DEFINE(TKey, TValue)                                 \
typedef TAG(DictEntry_##TKey##_##TValue) *DictEntry_##TKey##_##TValue;  \
typedef TAG(Dictionary(TKey,TValue)) {      \
    IEqualityComparer(TKey) Comparer;       \
    int Count;                              \
    DictEntry_##TKey##_##TValue *_items;    \
} *Dictionary(TKey,TValue);                 \
Dictionary(TKey,TValue) new(Dictionary(TKey,TValue))(IEqualityComparer(TKey) comparer);                         \
void Dictionary_##TKey##_##TValue##_Add(Dictionary(TKey,TValue) source, TKey key, TValue value);                \
TValue Dictionary_##TKey##_##TValue##_Get(Dictionary(TKey,TValue) source, TKey key);                            \
void Dictionary_##TKey##_##TValue##_Clear(Dictionary(TKey,TValue) source);                                      \
bool Dictionary_##TKey##_##TValue##_Remove(Dictionary(TKey,TValue) source, TKey key);                           \
bool Dictionary_##TKey##_##TValue##_ContainsKey(Dictionary(TKey,TValue) source, TKey key);                      \
bool Dictionary_##TKey##_##TValue##_ContainsValue(Dictionary(TKey,TValue) source, TValue value);                \
bool Dictionary_##TKey##_##TValue##_TryGetValue(Dictionary(TKey,TValue) source, TKey key, TValue* out);         \
IEnumerable(TValue) Dictionary_##TKey##_##TValue##_GetValues(Dictionary(TKey,TValue) source);

#pragma endregion

#endif