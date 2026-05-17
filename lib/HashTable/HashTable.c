#include "HashTable.h"
#include "String.h"

typedef TAG(HashNode) *HashNode;

KeyNotFoundException KeyNotFoundException__ctor(HashTable table, string key)
{
    var result = memalloc(KeyNotFoundException);
    *result = init(KeyNotFoundException) {
        .Message = new(string)("Key not found"),
        .Table = table,
        .Key = key
    };
    return result;
}

TAG(HashNode) {
    int Hash;
    string Key;
    object Value;
    HashNode Next;
};

TAG(HashTable) {
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
        || StringComparer.Ordinal.Compare((*bucket)->Key, key)))
    {
        bucket = &(*bucket)->Next;
    }
    *bucket = node;
}

object HashTable_Get(HashTable source, string key)
{
    var hash = source->HashCode(key);
    var bucket = source->Items[hash];
    while (bucket && StringComparer.Ordinal.Compare(bucket->Key, key)) bucket = bucket->Next;
    if (bucket == NULL) throw(new(KeyNotFoundException)(source, key));
    return bucket->Value;
}
