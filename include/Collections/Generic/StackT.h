#ifndef COLLECTIONS_GENERIC_STACK
#define COLLECTIONS_GENERIC_STACK

#define STACK_DEFINE(T)                             \
typedef struct StackCell_##T##_s *StackCell_##T;    \
typedef struct Stack_##T##_s {                      \
    struct IEnumerable_##T##_s _parent;             \
    StackCell_##T Start;                            \
    int Count;                                      \
} *Stack_##T;                                       \
Stack_##T Stack_##T##__ctor();                      \
void Stack_##T##_Clear(Stack_##T source);           \
void Stack_##T##_Push(Stack_##T source, T item);    \
T Stack_##T##_Peek(Stack_##T source);               \
T Stack_##T##_Pop(Stack_##T source);                \
bool Stack_##T##_TryPeek(Stack_##T source, T* out); \
bool Stack_##T##_TryPop(Stack_##T source, T* out);

#endif