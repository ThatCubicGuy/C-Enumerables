#ifndef COLLECTIONS_GENERIC_STACK
#define COLLECTIONS_GENERIC_STACK

#define Stack(T) _Stack_##T

#define STACK_DEFINE(T)                             \
typedef struct _Stack_##T##_s {                     \
    struct _IEnumerable_##T##_s _parent;            \
    int _start;                                     \
    T* _values;                                     \
    int Capacity, Count;                            \
} *Stack(T);                                        \
Stack(T) new(Stack(T))(int capacity);               \
void Stack_##T##_Destroy(Stack(T)* source);         \
void Stack_##T##_Clear(Stack(T) source);            \
void Stack_##T##_Push(Stack(T) source, T item);     \
T Stack_##T##_Peek(Stack(T) source);                \
T Stack_##T##_Pop(Stack(T) source);                 \
bool Stack_##T##_TryPeek(Stack(T) source, T* out);  \
bool Stack_##T##_TryPop(Stack(T) source, T* out);   \
void Stack_##T##_EnsureCapacity(Stack(T) source, int capacity);

#endif