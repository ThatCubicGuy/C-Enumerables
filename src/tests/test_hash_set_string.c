#include "Collections/Generic/EnumerableT.h"
#include "Collections/Generic/HashSetT.h"
#include "Tests.h"
#include "Keywords.h"
#include "String.h"

EQUALITY_COMPARER_DEFINE(string)
HASH_SET_DEFINE(string)

bool StartsWithHopper(string item)
{
    if (!item) return false;
    const string smth = "Hopper";
    for (int i = 0; smth[i]; ++i) {
        if (smth[i] != item[i]) return false;
    }
    return true;
}

void test_hash_set_string(FILE* output)
{
    HashSet(string) set = new(HashSet(string))(StringComparer.Ordinal.EqualityComparer);
    HashSet_string_Add(set, new(string)("DustinHenderson"));
    HashSet_string_Add(set, new(string)("MikeWheeler"));
    HashSet_string_Add(set, new(string)("MaxMayfield"));
    fprintf(output, "Count should be 3: %d\n", set->Count);
    HashSet_string_Add(set, new(string)("ElevenHopper"));
    HashSet_string_Add(set, new(string)("HopperChief"));
    HashSet_string_Add(set, new(string)("SteveTruth"));
    fprintf(output, "Count should be 6: %d\n", set->Count);
    fprintf(output, "Enumerate...\n");
    foreach (string item in set) {
        fprintf(output, "Item: \"%s\"\n", item);
    }
    HashSet_string_Remove(set, "IDK!");
    HashSet_string_Remove(set, "SteveTruth");
    fprintf(output, "Remove some items...\n");
    foreach (string item in set) {
        fprintf(output, "Item: \"%s\"\n", item);
    }
    fprintf(output, "Does the set contain \"%s\"? %s\n", "DustinHenderson", HashSet_string_Contains(set, "DustinHenderson") ? "Yes!" : "NO");
    HashSet_string_RemoveWhere(set, StartsWithHopper);
    fprintf(output, "Remove everything that starts with \"Hopper\"...\n");
    foreach (string item in set) {
        fprintf(output, "Item: \"%s\"\n", item);
    }
    fprintf(output, "Count btw: %d\n", set->Count);
}
