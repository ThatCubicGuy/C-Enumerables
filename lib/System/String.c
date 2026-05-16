#include "Keywords.h"
#include "String.h"

const string string_Empty = "";

static int StringComparerOrdinalCompare(string left, string right)
{
    int len = string_Length(left);
    for (int i = 0; i <= len; ++i) {
        if (left[i] != right[i]) return left[i] - right[i];
    }
    return 0;
}
static bool StringComparerOrdinalEquals(string left, string right)
{
    return StringComparerOrdinalCompare(left, right) == 0;
}
static size_t StringComparerOrdinalGetHashCode(string obj)
{
    size_t hash = 5381;
    int c;
    while((c = *obj++)) hash = ((hash << 5) + hash) + c;
    return hash;
}
static inline char ToLower(char c)
{
    return ('A' <= c && c <= 'Z') ? c - ('A' - 'a') : c;
}
static int StringComparerOrdinalIgnoreCaseCompare(string left, string right)
{
    int len = string_Length(left);
    for (int i = 0; i <= len; ++i) {
        if (ToLower(left[i]) != ToLower(right[i])) {
            return ToLower(left[i]) - ToLower(right[i]);
        }
    }
    return 0;
}
static bool StringComparerOrdinalIgnoreCaseEquals(string left, string right)
{
    return StringComparerOrdinalIgnoreCaseCompare(left, right) == 0;
}
static size_t StringComparerOrdinalIgnoreCaseGetHashCode(string obj)
{
    size_t hash = 5381;
    int c;
    while((c = ToLower(*obj++))) hash = ((hash << 5) + hash) + c;
    return hash;
}

const TAG(StaticStringComparer) StringComparer = {
    .Ordinal = (TAG(StringComparer)) {
        .Comparer = {{
            .Compare = StringComparerOrdinalCompare,
        }},
        .EqualityComparer = {{
            .Equals = StringComparerOrdinalEquals,
            .GetHashCode = StringComparerOrdinalGetHashCode
        }}
    },
    .OrdinalIgnoreCase = (TAG(StringComparer)) {
        .Comparer = {{
            .Compare = StringComparerOrdinalIgnoreCaseCompare,
        }},
        .EqualityComparer = {{
            .Equals = StringComparerOrdinalIgnoreCaseEquals,
            .GetHashCode = StringComparerOrdinalIgnoreCaseGetHashCode
        }}
    }
};

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
    string result = arralloc(char, length + 1);
    for (int i = 0; i < length; ++i) {
        ((char*)result)[i] = other[i];
    }
    ((char*)result)[length] = 0;
    return result;
}

string string_Concat(string first, string second)
{
    int length = string_Length(first) + string_Length(second);
    string result = arralloc(char, length + 1);
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

#include <stdarg.h>
#include <stdio.h>
string string_Format(string format, ...)
{
    char buf[8192];
    va_list argv;
    va_start(argv, format);
    vsprintf(buf, format, argv);
    va_end(argv);
    return new(string)(buf);
}

#include "CubeEnumerablesGenericImplementations.h"
ENUMERABLE_DEFINE(string)
ENUMERABLE_DEFINE_AGGREGATE(string, int)
LIST_DEFINE(string)

static int sumLengths(int current, string item)
{
    return current + string_Length(item);
}

string string_Join(string separator, IEnumerable(string) values)
{
    if (values == NULL) return NULL;
    List(string) list = Enumerable_string_ToList(values);
    if (list->Count == 0) {
        List_string_Destroy(&list);
        return string_Empty;
    }
    int totalLength = Enumerable_string_Aggregate_int(
        (IEnumerable(string))(list),
        string_Length(separator) * (list->Count - 1),
        sumLengths);
    string first = list->Values[0];
    string result = arralloc(char, totalLength + 1);
    memcopy((char*)(result + string_Length(result)), first, string_Length(first));
    List_string_Remove(list, first);
    foreach (string str in list) {
        memcopy((char*)(result + string_Length(result)), separator, string_Length(separator));
        memcopy((char*)(result + string_Length(result)), str, string_Length(str));
    }
    List_string_Destroy(&list);
    ((char*)result)[totalLength] = 0;
    return result;
}

ENUMERABLE_IMPLEMENT(string)
ENUMERABLE_IMPLEMENT_AGGREGATE(string, int)
LIST_IMPLEMENT(string)
