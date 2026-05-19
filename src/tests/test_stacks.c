#include "Tests.h"
#include "Keywords.h"
#include "Collections/Generic/StackT.h"
#include "Collections/Generic/StackImplement.h"
#include "Collections/Generic/EnumerableT.h"
#include <stdio.h>

ENUMERABLE_DEFINE(int)
STACK_DEFINE(int)

#define TRYPEEK(STACK, OUT) do { if (Stack_int_TryPeek(STACK, &OUT)) { fprintf(output, "Peek "#STACK": %d\n", OUT); } else fprintf(output, "Peek failed! Count: %d\n", STACK->Count); } while (0)
#define TRYPOP(STACK, OUT) do { if (Stack_int_TryPop(STACK, &OUT)) { fprintf(output, "Pop "#STACK": %d\n", OUT); } else fprintf(output, "Pop failed! Count: %d\n", STACK->Count); } while (0)

void test_stacks(FILE* output)
{
    int result;
    Stack(int) stack = new(Stack(int))(2);
    Stack_int_Push(stack, 4);
    Stack_int_Push(stack, 5);
    Stack_int_Push(stack, 1);
    Stack_int_Push(stack, 2);
    Stack_int_Push(stack, 6);
    Stack_int_Push(stack, 7);
    foreach (int item in stack) {
        fprintf(output, "Item: %d\n", item);
    }
    TRYPOP(stack, result);
    foreach (int item in stack) {
        fprintf(output, "Item: %d\n", item);
    }
    TRYPEEK(stack, result);
    foreach (int item in stack) {
        fprintf(output, "Item: %d\n", item);
    }
    fprintf(output, "Clear stack...\n");
    Stack_int_Clear(stack);
    foreach (int item in stack) {
        fprintf(output, "Item: %d\n", item);
    }
    TRYPOP(stack, result);
    TRYPEEK(stack, result);
    fprintf(output, "Add 57...\n");
    Stack_int_Push(stack, 57);
    TRYPEEK(stack, result);
    TRYPOP(stack, result);
}

STACK_IMPLEMENT(int)