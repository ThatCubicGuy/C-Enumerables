#ifndef DEFINES
#define DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

// Calls the primary constructor of TYPE.
#define new(TYPE) TYPE##__ctor
// Allocates a single instance of TYPE and returns its memory location.
#define alloc(TYPE) ((TYPE*)malloc(sizeof(TYPE)))
// Allocates an array of CAPACITY elements of type ARRAY_TYPE and returns its location.
#define alloc_array(ARRAY_TYPE, CAPACITY) ((ARRAY_TYPE*)calloc(sizeof(ARRAY_TYPE), CAPACITY))
// Initializes a potentially readonly struct pointer with a given initializer.
#define init(STRUCT_TYPE, var) *(struct STRUCT_TYPE*)var = (struct STRUCT_TYPE)
// Initializes a readonly location with the given reference value.
#define init_ro(REF_TYPE, var) *(REF_TYPE*)&var = (REF_TYPE)
// Allocates memory for a struct of type BASE_TYPE and initializes it with a given initializer.
#define allocinit(STUCT_TYPE, var) var = alloc(struct STUCT_TYPE); init(STUCT_TYPE, var)
// Returns the type that this type directly inherits from.
#define base(REF_TYPE) (&(REF_TYPE)->_parent)
// Returns the default (zero initialized) value for the given type.
#define default(TYPE) ((TYPE){0})
// Defines the empty constructor for a value type (that is, initializing all of its values with zero.)
#define DEFINE_EMPTY_VALUE_CTOR(VALUE_TYPE) VALUE_TYPE VALUE_TYPE##__ctor(void) { return default(VALUE_TYPE); }

typedef const void* object;
typedef const char* string;
typedef unsigned char byte;

typedef void Action(object);
typedef bool Equator(object, object);
typedef int Comparer(object, object);

bool ValueEquator(size_t itemSize, object item1ref, object item2ref);

bool ReferenceEquator(object item1, object item2);

int ReferenceComparer(object left, object right);

void MemCopy(void* dest, const void* source, size_t size_memb);

void MemCopyToNull(void* dest, const void* source);

#endif