#ifndef COLLECTIONS_GENERIC_STACK
#define COLLECTIONS_GENERIC_STACK

#define STACK_DEFINE(T)                             \
typedef struct Stack_##T##_s {                      \
    struct IEnumerable_##T##_s _parent;             \
    int _start;                                     \
    T* _values;                                     \
    int Capacity, Count;                            \
} *Stack_##T;                                       \
Stack_##T Stack_##T##__ctor(int capacity);          \
void Stack_##T##_Destroy(Stack_##T* source);        \
void Stack_##T##_Clear(Stack_##T source);           \
void Stack_##T##_Push(Stack_##T source, T item);    \
T Stack_##T##_Peek(Stack_##T source);               \
T Stack_##T##_Pop(Stack_##T source);                \
bool Stack_##T##_TryPeek(Stack_##T source, T* out); \
bool Stack_##T##_TryPop(Stack_##T source, T* out);  \
void Stack_##T##_EnsureCapacity(Stack_##T source, int capacity);

#endif