#include "Collections/Generic/EnumerableT.h"
#include "Collections/Generic/HashSetT.h"
#include "Tests.h"
#include "Defines.h"
#include "Text/String.h"
#include <stdio.h>

ENUMERABLE_DEFINE(string)
EQUALITY_COMPARER_DEFINE(string)
HASH_SET_DEFINE(string)

bool StringEquals(string left, string right)
{
    return StringComparer.Ordinal(left, right) == 0;
}

struct tag_IEqualityComparer_string StringEquator[1] = {(struct tag_IEqualityComparer_string) {
    .Equals = StringEquals,
    .GetHashCode = string_HashCode
}};

bool StartsWithSomething(string item)
{
    if (!item) return false;
    const string smth = "Something ";
    for (int i = 0; i < 10; ++i) {
        if (smth[i] != item[i]) return false;
    }
    return true;
}

void test_hash_set(void)
{
    HashSet(string) set = new(HashSet(string))(StringEquator);
    HashSet_string_Add(set, new(string)("Something like that"));
    HashSet_string_Add(set, new(string)("Something like this"));
    HashSet_string_Add(set, new(string)("Hey that's interesting!"));
    printf("Count should be 3: %d\n", set->Count);
    HashSet_string_Add(set, new(string)("Something like that"));
    HashSet_string_Add(set, new(string)("Something like that"));
    HashSet_string_Add(set, new(string)("Woah..."));
    HashSet_string_Add(set, new(string)("Something like that"));
    HashSet_string_Add(set, new(string)("Something like everything!"));
    printf("Count should be 4: %d\n", set->Count);
    printf("Enumerate...\n");
    foreach(string, item, set, {
        printf("Item: \"%s\"\n", item);
    });
    HashSet_string_Remove(set, "IDK!");
    HashSet_string_Remove(set, "Something like that");
    printf("Remove some items...\n");
    foreach(string, item, set, {
        printf("Item: \"%s\"\n", item);
    });
    printf("Does the set contain \"%s\"? %d\n", "Woah...", HashSet_string_Contains(set, "Woah..."));
    HashSet_string_RemoveWhere(set, StartsWithSomething);
    printf("Remove everything that starts with \"Something\"...\n");
    foreach(string, item, set, {
        printf("Item: \"%s\"\n", item);
    });
    printf("Count btw: %d\n", set->Count);
}

EQUALITY_COMPARER_DEFINE(object)
struct tag_IEqualityComparer_object ObjectEquator[1] = {(struct tag_IEqualityComparer_object) {
    .Equals = ReferenceEquator,
    .GetHashCode = ReferenceHashCode
}};
#include "Collections/Generic/HashSetImplement.h"
HASH_SET_IMPLEMENT(string)
