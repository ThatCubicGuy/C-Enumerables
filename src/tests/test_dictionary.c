#include "Collections/Generic/DictionaryT.h"
#include "Collections/Generic/EnumerableT.h"
#include "Tests.h"
#include "String.h"
#include "Keywords.h"
#include <stdio.h>

typedef struct car {
    string brand;
    int year, price;
} Car;

ENUMERABLE_DEFINE(Car)
DICTIONARY_DEFINE(string, Car)

void Get(Dictionary(string, Car) source, string key)
{
    Car result;
    if (Dictionary_string_Car_TryGetValue(source, key, &result)) {
        printf("Key \"%s\" retrieved: %s (%d); $%d\n", key, result.brand, result.year, result.price);
    } else {
        printf("Key %s does not exist!\n", key);
    }
}

void print(Dictionary(string, Car) source)
{
    for (int i = 0, j; i < MAX_DICTIONARY_ARRAY_LENGTH; ++i) {
        j = 0;
        for (typeof(void*) old = (void*)1, p = source->_items[i]; old; (void)((old = p) && (p = *(void**)p)), ++j) {
            printf("Item \033[3%dm#%d.%d\033[0m: %p\n", j + 1, i, j, p);
        }
    }
}

void test_dictionary(void)
{
    auto dict = new(Dictionary(string, Car))(StringComparer.Ordinal.EqualityComparer);
    printf("Dictionary count before: %d\n", dict->Count);
    Dictionary_string_Car_Add(dict, "Placeholder", (Car){"Toyota Corolla",2023,1500});
    Dictionary_string_Car_Add(dict, "My Fav", (Car){"Toyota AE86",1986,99});
    Dictionary_string_Car_Add(dict, "My Other Fav", (Car){"Nissan GT-R",2017,7500});
    Dictionary_string_Car_Add(dict, "My Third Fav", (Car){"Mitsubishi Evo X",2007,3500});
    printf("Dictionary count after: %d\n", dict->Count);
    Get(dict, "Placeholder");
    Get(dict, "My Other Fav");
    Get(dict, "My Fav");
    Get(dict, "My Third Fav");
    Get(dict, "idk");
    printf("Dictionary count before: %d\n", dict->Count);
    Dictionary_string_Car_Add(dict, "My Third Fav", (Car){"Nissan Silvia",2002,3000});
    printf("Dictionary count after: %d\n", dict->Count);
    foreach (var item in Dictionary_string_Car_GetValues(dict)) {
        printf("Item: %s (%d), $%d\n", item.brand, item.year, item.price);
    }
    print(dict);
}

#include "Collections/Generic/DictionaryImplement.h"
DICTIONARY_IMPLEMENT(string, Car)
