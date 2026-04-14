#ifndef DEFINES
#define DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Calls the primary constructor of TYPE.
#define NEW_I(TYPE) TYPE##__ctor
#define new(TYPE) NEW_I(TYPE)
// Returns the default (zero initialized) value for the given type.
#define default(TYPE) ((TYPE){0})
// Allocates a single instance of REF_TYPE and returns its memory location.
#define alloc(REF_TYPE) ((REF_TYPE)malloc(sizeof(*(REF_TYPE){0})))
// Allocates an instance of VALUE_TYPE on the heap and returns a pointer to its location.
#define box(VALUE_TYPE) ((VALUE_TYPE*)malloc(sizeof(VALUE_TYPE)))
// Allocates an array of CAPACITY elements of type ARRAY_TYPE and returns its location.
#define alloc_array(ARRAY_TYPE, CAPACITY) ((ARRAY_TYPE*)calloc(CAPACITY, sizeof(ARRAY_TYPE)))
// Initializes a potentially readonly reference type with a given initializer.
#define init(REF_TYPE, var) *(struct REF_TYPE##_s*)var = (struct REF_TYPE##_s)
// Initializes a readonly location with the given reference value.
#define init_ro(REF_TYPE, var) *(REF_TYPE*)&var = (REF_TYPE)
// Allocates memory for a reference type and initializes its value with a given initializer.
#define allocinit(REF_TYPE, var) var = alloc(REF_TYPE); init(REF_TYPE, var)
// Returns the type that this type directly inherits from.
#define base(REF_TYPE) (&(REF_TYPE)->_parent)
// Casts var to a void pointer for easy assignment to any reference type.
#define any(var) ((void*)var)
// Default implementation of object equality.
#define equals(LEFT, RIGHT) ValueEquator(sizeof(typeof(LEFT)), &(LEFT), &(RIGHT))
// Defines the empty constructor for a value type (that is, initializing all of its values with zero.)
#define IMPLEMENT_EMPTY_VALUE_CTOR(VALUE_TYPE) VALUE_TYPE VALUE_TYPE##__ctor(void) { return default(VALUE_TYPE); }

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