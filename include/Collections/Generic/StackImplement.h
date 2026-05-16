#ifndef COLLECTIONS_GENERIC_STACK_IMPLEMENTATIONS
#define COLLECTIONS_GENERIC_STACK_IMPLEMENTATIONS
#include "StackT.h"
#define STACK_IMPLEMENT(T)                                                          \
typedef TAG(StackEnumerator_##T) {                                                  \
    IMPL(IEnumerator(T));                                                           \
    Stack(T) _source;                                                               \
    int _currentIndex;                                                              \
} *StackEnumerator_##T;                                                             \
static bool StackMoveNext_##T(IEnumerator(T) This)                                  \
{                                                                                   \
    StackEnumerator_##T e = (StackEnumerator_##T)This;                              \
    if (e->_currentIndex < 0) return false;                                         \
    if (e->_source->Count == 0) return false;                                       \
    This->Current = e->_source->_values[e->_currentIndex];                          \
    e->_currentIndex -= 1;                                                          \
    return true;                                                                    \
}                                                                                   \
static void StackReset_##T(IEnumerator(T) This)                                     \
{                                                                                   \
    StackEnumerator_##T e = ((StackEnumerator_##T)This);                            \
    e->_currentIndex = e->_source->_start;                                          \
    This->Current = default(T);                                                     \
}                                                                                   \
static void StackDispose_##T(IEnumerator(T) This)                                   \
{                                                                                   \
    memfree(This);                                                                  \
}                                                                                   \
static IEnumerator(T) StackGetEnumerator_##T(const IEnumerable(T) This)             \
{                                                                                   \
    auto result = meminit(StackEnumerator_##T) {                                    \
        .MoveNext = StackMoveNext_##T,                                              \
        .Reset = StackReset_##T,                                                    \
        .Dispose = StackDispose_##T,                                                \
        ._currentIndex = ((Stack(T))This)->_start,                                  \
        ._source = (Stack(T))This                                                   \
    };                                                                              \
    return (IEnumerator(T))result;                                                  \
}                                                                                   \
Stack(T) new(Stack(T))(int capacity)                                \
{                                                                   \
    auto result = meminit(Stack(T)) {                               \
        .GetEnumerator = StackGetEnumerator_##T,                    \
        .Count = 0,                                                 \
        .Capacity = capacity,                                       \
        ._start = -1,                                               \
        ._values = arralloc(T, capacity)                            \
    };                                                              \
    return result;                                                  \
}                                                                   \
void Stack_##T##_Destroy(Stack(T)* source)                          \
{                                                                   \
    memfree((*source)->_values);                                    \
    memfree(*source);                                               \
    *source = NULL;                                                 \
}                                                                   \
void Stack_##T##_EnsureCapacity(Stack(T) source, int capacity)      \
{                                                                   \
    if (source->Capacity < capacity) {                              \
        source->_values = memresize(source->_values, capacity);     \
        source->Capacity = capacity;                                \
    }                                                               \
}                                                                   \
void Stack_##T##_Clear(Stack(T) source)                             \
{                                                                   \
    source->_start = -1;                                            \
    source->Count = 0;                                              \
}                                                                   \
void Stack_##T##_Push(Stack(T) source, T item)                      \
{                                                                   \
    if (source->Count == source->Capacity) {                        \
        Stack_##T##_EnsureCapacity(source, source->Capacity * 2);   \
    }                                                               \
    source->Count += 1;                                             \
    source->_start += 1;                                            \
    source->_values[source->_start] = item;                         \
}                                                                   \
T Stack_##T##_Peek(Stack(T) source)                                 \
{                                                                   \
    return source->_values[source->_start];                         \
}                                                                   \
T Stack_##T##_Pop(Stack(T) source)                                  \
{                                                                   \
    T result = source->_values[source->_start];                     \
    source->_start -= 1;                                            \
    source->Count -= 1;                                             \
    return result;                                                  \
}                                                                   \
bool Stack_##T##_TryPeek(Stack(T) source, T* out)                   \
{                                                                   \
    if (source->Count) {                                            \
        *out = Stack_##T##_Peek(source);                            \
        return true;                                                \
    } else return false;                                            \
}                                                                   \
bool Stack_##T##_TryPop(Stack(T) source, T* out)                    \
{                                                                   \
    if (source->Count) {                                            \
        *out = Stack_##T##_Pop(source);                             \
        return true;                                                \
    } else return false;                                            \
}

#endif