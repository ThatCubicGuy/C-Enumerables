#ifndef DEFINES
#define DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#define new(TYPE) TYPE##__ctor
#define alloc(TYPE) ((TYPE*)malloc(sizeof(TYPE)))
#define alloc_array(ARRAY_TYPE, CAPACITY) ((ARRAY_TYPE*)calloc(sizeof(ARRAY_TYPE), CAPACITY))

typedef const void* object;
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