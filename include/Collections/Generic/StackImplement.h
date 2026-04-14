#ifndef COLLECTIONS_GENERIC_STACK_IMPLEMENTATIONS
#define COLLECTIONS_GENERIC_STACK_IMPLEMENTATIONS

#include "Collections/Generic/StackT.h"

#define STACK_IMPLEMENT(T)                                                          \
typedef struct StackEnumerator_##T##_s {                                            \
    struct _IEnumerator_##T##_s _parent;                                            \
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
    free(This);                                                                     \
}                                                                                   \
static IEnumerator(T) StackGetEnumerator_##T(const IEnumerable(T) This)             \
{                                                                                   \
    StackEnumerator_##T allocinit(StackEnumerator_##T, result) {                    \
        ._parent = (struct _IEnumerator_##T##_s) {                                  \
            .MoveNext = StackMoveNext_##T,                                          \
            .Reset = StackReset_##T,                                                \
            .Dispose = StackDispose_##T                                             \
        },                                                                          \
        ._currentIndex = ((Stack(T))This)->_start,                                  \
        ._source = (Stack(T))This                                                   \
    };                                                                              \
    return base(result);                                                            \
}                                                                                   \
Stack(T) new(Stack(T))(int capacity)                                \
{                                                                   \
    Stack(T) allocinit(Stack(T), result) {                          \
        ._parent = (struct _IEnumerable_##T##_s) {                  \
            .GetEnumerator = StackGetEnumerator_##T                 \
        },                                                          \
        .Count = 0,                                                 \
        .Capacity = capacity,                                       \
        ._start = -1,                                               \
        ._values = alloc_array(T, capacity)                         \
    };                                                              \
    return result;                                                  \
}                                                                   \
void Stack_##T##_Destroy(Stack(T)* source)                          \
{                                                                   \
    free((*source)->_values);                                       \
    free(*source);                                                  \
    *source = NULL;                                                 \
}                                                                   \
void Stack_##T##_EnsureCapacity(Stack(T) source, int capacity)      \
{                                                                   \
    if (source->Capacity < capacity) {                              \
        source->_values = realloc(source->_values,                  \
            sizeof(T) * capacity);                                  \
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