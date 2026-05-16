#ifndef COLLECTIONS_MULTI_WAY_TREE
#define COLLECTIONS_MULTI_WAY_TREE

#include "Keywords.h"

#define MULTI_WAY_TREE_DEFINE(T)
typedef TAG(MultiWayNode)* _MultiWayNode;
typedef TAG(MultiWayTree) {
    int Count, Capacity;
    _MultiWayNode* Values;
} *MultiWayTree;
[[nodiscard("Constructor")]]
MultiWayTree new(MultiWayTree)(int capacity);
void MultiWayTree_AddNode(MultiWayTree source, void* item);
void MultiWayTree_DelNode(MultiWayTree source, void* item);

#endif