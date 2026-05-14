#ifndef COLLECTIONS_MULTI_WAY_TREE
#define COLLECTIONS_MULTI_WAY_TREE

#include "Defines.h"

#define MULTI_WAY_TREE_DEFINE(T)
typedef struct _MultiWayNode_s* _MultiWayNode;
typedef struct _MultiWayTree_s {
    int Count, Capacity;
    _MultiWayNode* Values;
} *MultiWayTree;
[[nodiscard("Constructor")]]
MultiWayTree new(MultiWayTree)(int capacity);
void MultiWayTree_AddNode(MultiWayTree source, void* item);
void MultiWayTree_DelNode(MultiWayTree source, void* item);

#endif