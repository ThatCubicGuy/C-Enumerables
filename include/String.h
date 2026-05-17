#ifndef CUBE_STRING
#define CUBE_STRING
#include "Keywords.h"

/**
 * @brief Represents the empty string.
 * This field is constant.
 */
extern const string string_Empty;

EQUALITY_COMPARER_DEFINE(string)
COMPARER_DEFINE(string)
TAG(StringComparer) {
    union {
        TAG(IComparer(string)) Comparer[1];
        TAG(IComparer(string));
    };
    union {
        TAG(IEqualityComparer(string)) EqualityComparer[1];
        TAG(IEqualityComparer(string));
    };
};
extern const TAG(StringComparerStatic) {
    TAG(StringComparer) Ordinal;
    TAG(StringComparer) OrdinalIgnoreCase;
} StringComparer;

/**
 * @brief Copies a string and returns the result.
 * @return A new string with characters from the other.
 * @pure
 */
string string__ctor(string other);

/**
 * @brief Gets the length of the given string.
 * @returns An integer representing the
 * amount of characters in the string.
 * @pure
 */
int string_Length(string source);

/**
 * @brief Concatenates two strings and returns the result.
 * @return A new string.
 * @pure
 */
string string_Concat(string first, string second);

/**
 * @brief Formats the given parameters into the format string and returns the result.
 * @return A new string.
 * @pure
 */
string string_Format(string format, ...);
#ifdef IEnumerable
ENUMERABLE_DEFINE(string)
/**
 * @brief Joins the values of a string collection into one string,
 * using the specified separator.
 * @pure
 */
string string_Join(string separator, IEnumerable(string) values);
#endif
#endif