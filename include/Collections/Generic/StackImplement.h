#include "Defines.h"
#include "StackT.h"

#define STACK_IMPLEMENT(T)                                                          \
struct StackCell_##T##_s {                                                          \
    StackCell_##T Next;                                                             \
    T Value;                                                                        \
};                                                                                  \
typedef struct StackEnumerator_##T##_s {                                            \
    struct IEnumerator_##T##_s _parent;                                             \
    Stack_##T _source;                                                              \
    StackCell_##T _currentNode;                                                     \
} *StackEnumerator_##T;                                                             \
static bool StackMoveNext_##T(IEnumerator_##T This)                                 \
{                                                                                   \
    StackEnumerator_##T e = (StackEnumerator_##T)This;                              \
    if (e->_currentNode == NULL) {                                                  \
        if (e->_source->Start == NULL) return false;                                \
        e->_currentNode = e->_source->Start;                                        \
        This->Current = e->_currentNode->Value;                                     \
        return true;                                                                \
    }                                                                               \
    if ((e->_currentNode)->Next == NULL) return false;                              \
    e->_currentNode = e->_currentNode->Next;                                        \
    This->Current = e->_currentNode->Value;                                         \
    return true;                                                                    \
}                                                                                   \
static void StackReset_##T(IEnumerator_##T This)                                    \
{                                                                                   \
    ((StackEnumerator_##T)This)->_currentNode = NULL;                               \
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
        ._currentNode = NULL,                                                       \
        ._source = (Stack_##T)This                                                  \
    };                                                                              \
    return base(result);                                                            \
}                                                                                   \
Stack_##T Stack_##T##__ctor()                           \
{                                                       \
    Stack_##T allocinit(Stack_##T, result) {            \
        ._parent = (struct IEnumerable_##T##_s) {       \
            .GetEnumerator = StackGetEnumerator_##T     \
        },                                              \
        .Count = 0,                                     \
        .Start = NULL                                   \
    };                                                  \
    return result;                                      \
}                                                       \
static void RemoveCell_##T(StackCell_##T cell)          \
{                                                       \
    if (cell->Next) RemoveCell_##T(cell->Next);         \
    free(cell);                                         \
}                                                       \
void Stack_##T##_Clear(Stack_##T source)                \
{                                                       \
    RemoveCell_##T(source->Start);                      \
    source->Start = NULL;                               \
    source->Count = 0;                                  \
}                                                       \
void Stack_##T##_Push(Stack_##T source, T item)         \
{                                                       \
    StackCell_##T allocinit(StackCell_##T, result) {    \
        .Value = item,                                  \
        .Next = source->Start                           \
    };                                                  \
    source->Start = result;                             \
    source->Count += 1;                                 \
}                                                       \
T Stack_##T##_Peek(Stack_##T source)                    \
{                                                       \
    return source->Start->Value;                        \
}                                                       \
T Stack_##T##_Pop(Stack_##T source)                     \
{                                                       \
    T result = source->Start->Value;                    \
    StackCell_##T next = source->Start->Next;           \
    free(source->Start);                                \
    source->Start = next;                               \
    return result;                                      \
}                                                       \
bool Stack_##T##_TryPeek(Stack_##T source, T* out)      \
{                                                       \
    if (source->Start) {                                \
        *out = Stack_##T##_Peek(source);                \
        return true;                                    \
    } else return false;                                \
}                                                       \
bool Stack_##T##_TryPop(Stack_##T source, T* out)       \
{                                                       \
    if (source->Start) {                                \
        *out = Stack_##T##_Pop(source);                 \
        return true;                                    \
    } else return false;                                \
}