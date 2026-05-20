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

static void print(FILE* output, HashSet(string) source)
{
    for (int i = 0, j; i < MAX_HASH_SET_ARRAY_LENGTH; ++i) {
        j = 0;
        for (typeof(void*) old = (void*)1, p = source->_items[i]; old; (void)((old = p) && (p = *(void**)p)), ++j) {
            fprintf(output, "Item \033[3%dm#%d.%d\033[0m: %p\n", j + 1, i, j, p);
        }
    }
}

static void shuffle(char* str)
{
    static union { struct {char a, b, c, d;}; char v[4];} s = {{'!', '\"', '#', '$'}};
    static size_t i = 0;
    s.v[object_GetHashCode((void*)(i++)) % 4];
    for (int i = 0; i < 4; ++i) {
        str[i] = (str[i] + s.v[i]) / 2 - (str[i] - s.v[i]) * 2;
    }
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
    print(output, set);
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
    fprintf(output, "Stress test time!\n");
    HashSet_string_Clear(set);
    fprintf(output, "Count after clear: %d\n", set->Count);
    for (char str[5] = "smhg", i = 0; i < 127; shuffle(str), ++i) {
        fprintf(output, "Adding: %s\n", str);
        HashSet_string_Add(set, new(string)(str));
    }
    fprintf(output, "Count after adds: %d\n", set->Count);
    foreach (string item in set) {
        fprintf(output, "Item: {%s}\n", item);
    }
    print(output, set);
}

#include "Collections/Generic/HashSetImplement.h"
EQUALITY_COMPARER_DEFINE(object)
HASH_SET_IMPLEMENT(string)
