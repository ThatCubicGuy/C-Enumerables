#include "Collections/Generic/EnumerableT.h"
#include "Collections/Generic/HashSetT.h"
#include "Tests.h"
#include "Keywords.h"
#include "String.h"

EQUALITY_COMPARER_DEFINE(string)
HASH_SET_DEFINE(string)

static bool StartsWithSomething(string item)
{
    if (!item) return false;
    const string smth = "Something ";
    for (int i = 0; i < 10; ++i) {
        if (smth[i] != item[i]) return false;
    }
    return true;
}

void test_hash_set(FILE* output)
{
    HashSet(string) set = new(HashSet(string))(StringComparer.Ordinal.EqualityComparer);
    HashSet_string_Add(set, new(string)("Something like that"));
    HashSet_string_Add(set, new(string)("Something like this"));
    HashSet_string_Add(set, new(string)("Hey that's interesting!"));
    fprintf(output, "Count should be 3: %d\n", set->Count);
    HashSet_string_Add(set, new(string)("Something like that"));
    HashSet_string_Add(set, new(string)("Something like that"));
    HashSet_string_Add(set, new(string)("Something like that"));
    HashSet_string_Add(set, new(string)("Something like everything!"));
    fprintf(output, "Count should be 4: %d\n", set->Count);
    fprintf(output, "Enumerate...\n");
    foreach (string item in set) {
        fprintf(output, "Item: \"%s\"\n", item);
    }
    HashSet_string_Remove(set, "IDK!");
    HashSet_string_Remove(set, "Something like that");
    fprintf(output, "Remove some items...\n");
    foreach (string item in set) {
        fprintf(output, "Item: \"%s\"\n", item);
    }
    fprintf(output, "Does the set contain \"%s\"? %s\n", "Woah...", HashSet_string_Contains(set, "Woah...") ? "Yes!" : "NO");
    HashSet_string_RemoveWhere(set, StartsWithSomething);
    fprintf(output, "Remove everything that starts with \"Something\"...\n");
    foreach (string item in set) {
        fprintf(output, "Item: \"%s\"\n", item);
    }
    fprintf(output, "Count btw: %d\n", set->Count);
}

#include "Collections/Generic/HashSetImplement.h"
EQUALITY_COMPARER_DEFINE(object)
HASH_SET_IMPLEMENT(string)
