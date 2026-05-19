#ifndef DEFINE_TUPLE
#define DEFINE_TUPLE

// Tuples are supposed to be light data structures created on a whim.
// Because of this, I've decided to keep only the DEFINE macro,
// making functions static so as to not run into linking issues.

#include "Keywords.h"

// Declares a tuple of the given types.
#define t(...) JOIN(_, __VA_ARGS__)

#define TUPLE_TO_STRUCT_MEMBER(IDX, TYPE) TYPE CAT(Item,IDX);
#define TUPLE_TO_CTOR_PARAM(IDX, TYPE) , TYPE CAT(item,IDX)
#define TUPLE_TO_INIT_MEMBER(IDX, TYPE) CAT(item,IDX),
#define TUPLE_TO_OUT_PARAM(IDX, TYPE) , TYPE* CAT(out,IDX)
#define TUPLE_TO_OUTPUT(IDX, TYPE) if (CAT(out,IDX)) *CAT(out,IDX) = CAT(source.Item,IDX); 
#define TUPLE_DEFINE(TYPES...) typedef struct JOIN(_,tag,ValueTuple,TYPES) { \
    FOREACH_INDEX(1,TUPLE_TO_STRUCT_MEMBER,TYPES)   \
} t(TYPES);     \
ATTR_UNUSED static inline t(TYPES) new(t(TYPES))(TRIM(FOREACH_INDEX(1,TUPLE_TO_CTOR_PARAM,TYPES))) { return (t(TYPES)) {FOREACH_INDEX(1,TUPLE_TO_INIT_MEMBER,TYPES)}; } \
ATTR_UNUSED static inline void JOIN(_,Tuple,TYPES,Deconstruct)(t(TYPES) source FOREACH_INDEX(1,TUPLE_TO_OUT_PARAM,TYPES)) \
{                                   \
    FOREACH_INDEX(1,TUPLE_TO_OUTPUT,TYPES) \
}

#endif