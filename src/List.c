#include "Collections/Generic/ListImplement.h"
#include "Collections/Generic/EnumerableImplement.h"

ENUMERABLE_DEFINE(int)
LIST_DEFINE(int)

ENUMERABLE_IMPLEMENT(int)
LIST_IMPLEMENT(int)
ENUMERABLE_IMPLEMENT_SELECT(int, int)
ENUMERABLE_IMPLEMENT_SELECTMANY(int, int)
ENUMERABLE_IMPLEMENT_AGGREGATE(int, int)