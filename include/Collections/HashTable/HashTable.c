#include "HashTable.h"
#include "Keywords.h"
#include "System/String.h"

typedef struct HashNode_s *HashNode;

typedef struct _KeyNotFoundException {
    struct _Exception;
    HashTable Table;
    string Key;
} *KeyNotFoundException;
static KeyNotFoundException KeyNotFoundException__ctor(HashTable table, string key)
{
    var result = memalloc(KeyNotFoundException);
    *result = init(KeyNotFoundException) {
        .Message = new(string)("Key not found"),
        .Table = table,
        .Key = key
    };
    return result;
}

struct HashNode_s {
    int Hash;
    string Key;
    object Value;
    HashNode Next;
};

struct HashTable_s {
    int Capacity;
    HashFunc *HashCode;
    HashNode *Items;
};

HashTable HashTable__ctor(int capacity, HashFunc *hashCode)
{
    var result = memalloc(HashTable);
    result->Capacity = capacity;
    result->HashCode = hashCode;
    result->Items = arralloc(HashNode, capacity);
    return result;
}

void HashTable_Put(HashTable source, string key, object value)
{
    var node = memalloc(HashNode);
    *node = init(HashNode) {
        .Hash = source->HashCode(key),
        .Key = key,
        .Value = value,
        .Next = NULL
    };
    int index = node->Hash % source->Capacity;
    var bucket = &source->Items[index];
    while (*bucket && ((*bucket)->Hash != node->Hash
        || string_Compare((*bucket)->Key, key)))
    {
        bucket = &(*bucket)->Next;
    }
    *bucket = node;
}

object HashTable_Get(HashTable source, string key)
{
    var hash = source->HashCode(key);
    var bucket = source->Items[hash];
    while (bucket && string_Compare(bucket->Key, key)) bucket = bucket->Next;
    if (bucket == NULL) throw(new(KeyNotFoundException)(source, key));
    return bucket->Value;
}
