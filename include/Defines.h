#ifndef DEFINES
#define DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define new(TYPE) ((TYPE*)malloc(sizeof(TYPE)))
#define new_arr(ARRAY_TYPE, CAPACITY) ((ARRAY_TYPE*)malloc(sizeof(ARRAY_TYPE) * CAPACITY))

typedef const void* object;

typedef void Action(object);

#endif