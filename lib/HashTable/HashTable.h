#ifndef HASHTABLE
#define HASHTABLE
#include "Keywords.h"

typedef TAG(HashTable) *HashTable;
typedef int HashFunc(object);

typedef TAG(KeyNotFoundException) {
    TAG(Exception);
    HashTable Table;
    string Key;
} *KeyNotFoundException;
KeyNotFoundException KeyNotFoundException__ctor(HashTable table, string key);

HashTable HashTable__ctor(int capacity, HashFunc *hashCode);

void HashTable_Put(HashTable source, string key, object value);

object HashTable_Get(HashTable source, string key);

#endif