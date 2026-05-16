#include <stdlib.h>
#include <stdbool.h>

typedef struct tag_bnode {
    struct tag_bnode* Left;
    struct tag_bnode* Right;
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
    return indexof_aux(source->Left, value, 2 * currentIndex + 1) || indexof_aux(source->Right, value, 2 * currentIndex + 2);
}

int IndexOf(BinaryTree source, int value)
{
    return indexof_aux(source, value, 0);
}
