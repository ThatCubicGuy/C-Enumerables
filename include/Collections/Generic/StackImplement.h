#ifndef COLLECTIONS_GENERIC_STACK_IMPLEMENTATIONS
#define COLLECTIONS_GENERIC_STACK_IMPLEMENTATIONS

#include "Collections/Generic/StackT.h"

#define STACK_IMPLEMENT(T)                                                          \
typedef struct StackEnumerator_##T##_s {                                            \
    struct IEnumerator_##T##_s _parent;                                             \
    Stack_##T _source;                                                              \
    int _currentIndex;                                                              \
} *StackEnumerator_##T;                                                             \
static bool StackMoveNext_##T(IEnumerator_##T This)                                 \
{                                                                                   \
    StackEnumerator_##T e = (StackEnumerator_##T)This;                              \
    if (e->_currentIndex < 0) return false;                                         \
    if (e->_source->Count == 0) return false;                                       \
    This->Current = e->_source->_values[e->_currentIndex];                          \
    e->_currentIndex -= 1;                                                          \
    return true;                                                                    \
}                                                                                   \
static void StackReset_##T(IEnumerator_##T This)                                    \
{                                                                                   \
    StackEnumerator_##T e = ((StackEnumerator_##T)This);                            \
    e->_currentIndex = e->_source->_start;                                          \
    This->Current = default(T);                                                     \
}                                                                                   \
static void StackDispose_##T(IEnumerator_##T This)                                  \
{                                                                                   \
    free(This);                                                                     \
}                                                                                   \
static IEnumerator_##T StackGetEnumerator_##T(const IEnumerable_##T This)           \
{                                                                                   \
    StackEnumerator_##T allocinit(StackEnumerator_##T, result) {                    \
        ._parent = (struct IEnumerator_##T##_s) {                                   \
            .MoveNext = StackMoveNext_##T,                                          \
            .Reset = StackReset_##T,                                                \
            .Dispose = StackDispose_##T                                             \
        },                                                                          \
        ._currentIndex = ((Stack_##T)This)->_start,                                 \
        ._source = (Stack_##T)This                                                  \
    };                                                                              \
    return base(result);                                                            \
}                                                                                   \
Stack_##T Stack_##T##__ctor(int capacity)                           \
{                                                                   \
    Stack_##T allocinit(Stack_##T, result) {                        \
        ._parent = (struct IEnumerable_##T##_s) {                   \
            .GetEnumerator = StackGetEnumerator_##T                 \
        },                                                          \
        .Count = 0,                                                 \
        .Capacity = capacity,                                       \
        ._start = -1,                                               \
        ._values = alloc_array(T, capacity)                         \
    };                                                              \
    return result;                                                  \
}                                                                   \
void Stack_##T##_Destroy(Stack_##T* source)                         \
{                                                                   \
    free((*source)->_values);                                       \
    free(*source);                                                  \
    *source = NULL;                                                 \
}                                                                   \
void Stack_##T##_EnsureCapacity(Stack_##T source, int capacity)     \
{                                                                   \
    if (source->Capacity < capacity) {                              \
        source->_values = realloc(source->_values,                  \
            sizeof(T) * capacity);                                  \
        source->Capacity = capacity;                                \
    }                                                               \
}                                                                   \
void Stack_##T##_Clear(Stack_##T source)                            \
{                                                                   \
    source->_start = -1;                                            \
    source->Count = 0;                                              \
}                                                                   \
void Stack_##T##_Push(Stack_##T source, T item)                     \
{                                                                   \
    if (source->Count == source->Capacity) {                        \
        Stack_##T##_EnsureCapacity(source, source->Capacity * 2);   \
    }                                                               \
    source->Count += 1;                                             \
    source->_start += 1;                                            \
    source->_values[source->_start] = item;                         \
}                                                                   \
T Stack_##T##_Peek(Stack_##T source)                                \
{                                                                   \
    return source->_values[source->_start];                         \
}                                                                   \
T Stack_##T##_Pop(Stack_##T source)                                 \
{                                                                   \
    T result = source->_values[source->_start];                     \
    source->_start -= 1;                                            \
    source->Count -= 1;                                             \
    return result;                                                  \
}                                                                   \
bool Stack_##T##_TryPeek(Stack_##T source, T* out)                  \
{                                                                   \
    if (source->Count) {                                            \
        *out = Stack_##T##_Peek(source);                            \
        return true;                                                \
    } else return false;                                            \
}                                                                   \
bool Stack_##T##_TryPop(Stack_##T source, T* out)                   \
{                                                                   \
    if (source->Count) {                                            \
        *out = Stack_##T##_Pop(source);                             \
        return true;                                                \
    } else return false;                                            \
}

#endif