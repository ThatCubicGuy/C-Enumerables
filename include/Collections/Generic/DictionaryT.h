#ifndef COLLECTIONS_GENERIC_DICTIONARY
#define COLLECTIONS_GENERIC_DICTIONARY

#define Dictionary(TKey, TValue) _Dictionary_##TKey##_##TValue
#include "Delegate.h"
#define DICTIONARY_DEFINE(TKey, TValue)                                         \
typedef struct struct_DictEntry_##TKey##_##TValue *DictEntry_##TKey##_##TValue; \
typedef struct dictionary_##TKey##_##TValue##_s {       \
    Func(int, TKey, TKey) Compare;                      \
    Func(unsigned long, TKey) HashCode;                 \
    int Count, Capacity;                                \
    DictEntry_##TKey##_##TValue *_items;                \
} *Dictionary(TKey,TValue);                             \
Dictionary(TKey,TValue) new(Dictionary(TKey,TValue))(int capacity, Func(unsigned long, TKey) hashCode, Func(int, TKey,TKey) comparer);\
void Dictionary_##TKey##_##TValue##_Add(Dictionary(TKey,TValue) source, TKey key, TValue value);                \
TValue Dictionary_##TKey##_##TValue##_Get(Dictionary(TKey,TValue) source, TKey key);                            \
void Dictionary_##TKey##_##TValue##_Clear(Dictionary(TKey,TValue) source);                                      \
bool Dictionary_##TKey##_##TValue##_Remove(Dictionary(TKey,TValue) source, TKey key);                           \
bool Dictionary_##TKey##_##TValue##_ContainsKey(Dictionary(TKey,TValue) source, TKey key);                      \
bool Dictionary_##TKey##_##TValue##_ContainsValue(Dictionary(TKey,TValue) source, TValue value);                \
bool Dictionary_##TKey##_##TValue##_TryGetValue(Dictionary(TKey,TValue) source, TKey key, TValue* out);         \
IEnumerable(TValue) Dictionary_##TKey##_##TValue##_GetValues(Dictionary(TKey,TValue) source);

#endif