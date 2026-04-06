#include <stdlib.h>
#include <stdbool.h>

typedef struct bnode_s {
    struct bnode_s* Left;
    struct bnode_s* Right;
    int Value;
} *BinaryTree;

bool Contains(BinaryTree source, int value)
{
    if (source == NULL) return false;
    if (source->Value == value) return true;
    return Contains(source->Left, value) || Contains(source->Right, value);
}

static int indexof_aux(BinaryTree source, int value, int currentIndex)
{
    if (source == NULL) return 0;
    if (source->Value == value) return currentIndex;
    return indexof_aux(source->Left, value, 2 * currentIndex) || indexof_aux(source->Right, value, 2 * currentIndex + 1);
}

// Zero based, which I hate for binary trees.
int IndexOf(BinaryTree source, int value)
{
    return indexof_aux(source, value, 1) - 1;
}
