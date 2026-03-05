#ifndef DEFINES
#define DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define new(TYPE) (TYPE*)TYPE##__ctor
#define bare(TYPE) ((TYPE*)malloc(sizeof(TYPE)))
#define new_array(ARRAY_TYPE, CAPACITY) ((ARRAY_TYPE*)malloc(sizeof(ARRAY_TYPE) * CAPACITY))

typedef const void* object;

typedef object ctor(void);

typedef void Action(object);
typedef int Comparer(object, object);

#endif