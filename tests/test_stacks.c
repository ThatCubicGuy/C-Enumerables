#include "Tests.h"
#include "Defines.h"
#include "Collections/Generic/StackT.h"
#include "Collections/Generic/StackImplement.h"
#include "Collections/Generic/EnumerableT.h"

ENUMERABLE_DEFINE(int)
STACK_DEFINE(int)

#define TRYPEEK(STACK, OUT) do { if (Stack_int_TryPeek(STACK, &OUT)) { printf("Peek "#STACK": %d\n", OUT); } else printf("Peek failed! Count: %d\n", STACK->Count); } while (0)
#define TRYPOP(STACK, OUT) do { if (Stack_int_TryPop(STACK, &OUT)) { printf("Pop "#STACK": %d\n", OUT); } else printf("Pop failed! Count: %d\n", STACK->Count); } while (0)

void test_stacks(void)
{
    int result;
    Stack(int) stack = new(Stack(int))(2);
    Stack_int_Push(stack, 4);
    Stack_int_Push(stack, 5);
    Stack_int_Push(stack, 1);
    Stack_int_Push(stack, 2);
    Stack_int_Push(stack, 6);
    Stack_int_Push(stack, 7);
    foreach (int, item, stack, {
        printf("Item: %d\n", item);
    });
    TRYPOP(stack, result);
    foreach (int, item, stack, {
        printf("Item: %d\n", item);
    });
    TRYPEEK(stack, result);
    foreach (int, item, stack, {
        printf("Item: %d\n", item);
    });
    printf("Clear stack...\n");
    Stack_int_Clear(stack);
    foreach (int, item, stack, {
        printf("Item: %d\n", item);
    });
    TRYPOP(stack, result);
    TRYPEEK(stack, result);
    printf("Add 57...\n");
    Stack_int_Push(stack, 57);
    TRYPEEK(stack, result);
    TRYPOP(stack, result);
}

STACK_IMPLEMENT(int)