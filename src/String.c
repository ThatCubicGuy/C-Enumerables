#include "Collections/Generic/EnumerableT.h"
#include "Collections/Generic/ListT.h"
#include "Text/String.h"

#ifndef STRING_LIST_DEFINED
#define STRING_LIST_DEFINED
LIST_DEFINE(string)
#endif
#ifndef STRING_INT_AGGREGATE_DEFINED
#define STRING_INT_AGGREGATE_DEFINED
ENUMERABLE_DEFINE_AGGREGATE(string, int)
#endif

const string string_Empty = "";

int string_Length(string source)
{
    if (source == NULL) return -1;
    int i = 0;
    while (source[i]) {
        i += 1;
    }
    return i;
}

string string__ctor(string other)
{
    int length = string_Length(other);
    string result = alloc_array(char, length + 1);
    for (int i = 0; i < length; ++i) {
        ((char*)result)[i] = other[i];
    }
    ((char*)result)[length] = 0;
    return result;
}

string string_Concat(string first, string second)
{
    int length = string_Length(first) + string_Length(second);
    string result = alloc_array(char, length + 1);
    int i = 0;
    while (first[i]) {
        ((char*)result)[i] = first[i];
        i += 1;
    }
    for (int j = 0; second[i]; ++j) {
        ((char*)result)[i] = second[j];
    }
    ((char*)result)[length] = 0;
    return result;
}

#ifdef STRING_ENUMERABLE_DEFINED
#ifdef STRING_LIST_DEFINED
#ifdef STRING_INT_AGGREGATE_DEFINED
static int sumLengths(int current, string item)
{
    return current + string_Length(item);
}
string string_Join(string separator, IEnumerable_string values)
{
    if (values == NULL) return NULL;
    List_string list = Enumerable_string_ToList(values);
    if (list->Count == 0) {
        DestroyList_string(&list);
        return string_Empty;
    }
    int totalLength = Enumerable_string_Aggregate_int(
        base(list),
        string_Length(separator) * (list->Count - 1),
        sumLengths);
    string first = Enumerable_string_ElementAt(base(list), 0);
    string result = alloc_array(char, totalLength - 1);
    MemCopy((char*)(result + string_Length(result)), first, string_Length(first));
    foreach(string, str, Enumerable_string_Skip(base(list), 1), {
        MemCopy((char*)(result + string_Length(result)), separator, string_Length(separator));
        MemCopy((char*)(result + string_Length(result)), str, string_Length(str));
    });
    DestroyList_string(&list);
    ((char*)result)[totalLength] = 0;
    return result;
}
#endif
#endif
#endif

#ifdef STRING_ENUMERABLE_DEFINED
#ifndef STRING_ENUMERABLE_IMPLEMENTED
#define STRING_ENUMERABLE_IMPLEMENTED
#include "Collections/Generic/EnumerableImplement.h"
ENUMERABLE_IMPLEMENT(string)
ENUMERABLE_IMPLEMENT_SELECT(string, string)
ENUMERABLE_IMPLEMENT_SELECTMANY(string, string)
ENUMERABLE_IMPLEMENT_AGGREGATE(string, string)
#endif
#endif
#ifdef STRING_LIST_DEFINED
#ifndef STRING_LIST_IMPLEMENTED
#define STRING_LIST_IMPLEMENTED
#include "Collections/Generic/ListImplement.h"
LIST_IMPLEMENT(string)
#endif
#endif
#ifdef STRING_INT_AGGREGATE_DEFINED
#ifndef STRING_INT_AGGREGATE_IMPLEMENTED
#define STRING_INT_AGGREGATE_IMPLEMENTED
#include "Collections/Generic/EnumerableImplement.h"
ENUMERABLE_IMPLEMENT_AGGREGATE(string, int)
#endif
#endif