#include "Keywords.h"
#include "System.h"

#define EXIT_UNHANDLED_EXCEPTION 134

#pragma region Stack Trace

#define STACKTRACE_SIZE 256

struct StackTrace_s {
    Exception LastThrownException;
    int UpThrow:1, FinallyReturnsToCode:1, Tail:30;
    struct __jmp_buf_tag Stack[STACKTRACE_SIZE];
} _StackTrace = {
    .LastThrownException = NULL,
    .Stack = {{{0},0,{{0}}}},
    .Tail = 0
};

#include <stdio.h>

struct __jmp_buf_tag* StackTrace_Push(void)
{
    DEBUG_WRITELINE("Called StackTrace_Push. Count before: %d", _StackTrace.Tail);
    _StackTrace.Tail += 1;
    return _StackTrace.Stack + _StackTrace.Tail - 1;
}

struct __jmp_buf_tag* StackTrace_Peek(void)
{
    DEBUG_WRITELINE("Called StackTrace_Peek. Count: %d", _StackTrace.Tail);
    return _StackTrace.Stack + _StackTrace.Tail - 1;
}

struct __jmp_buf_tag* StackTrace_Pop(Exception ex)
{
    DEBUG_WRITELINE("Called StackTrace_Pop. Count before: %d", _StackTrace.Tail);
    if (_StackTrace.Tail == 0) {
        fprintf(stderr, "Unhandled exception: \"%s\"\n", ex ? ex->Message : "(nil)");
        exit(EXIT_UNHANDLED_EXCEPTION);
    }
    _StackTrace.LastThrownException = ex;
    _StackTrace.Tail -= 1;
    return _StackTrace.Stack + _StackTrace.Tail;
}

Exception StackTrace_ThrownException(void)
{
    DEBUG_WRITELINE("Called StackTrace_ThrownException.");
    return _StackTrace.LastThrownException;
}

void StackTrace_Rethrow(void)
{
    DEBUG_WRITELINE("Called StackTrace_Rethrow.");
    throw(_StackTrace.LastThrownException);
}

int StackTrace_UpThrow(enum StackTraceOperation op)
{
    DEBUG_WRITELINE("Called StackTrace_DownThrow with op %d, Value: %d", op, _StackTrace.UpThrow);
    if (op != STACKTRACE_GET) _StackTrace.UpThrow = op; 
    return _StackTrace.UpThrow;
}

int StackTrace_Finally(enum StackTraceOperation op)
{
    DEBUG_WRITELINE("Called StackTrace_Finally with op %d. Value: %d", op, _StackTrace.FinallyReturnsToCode);
    if (op != STACKTRACE_GET) _StackTrace.FinallyReturnsToCode = op; 
    return _StackTrace.FinallyReturnsToCode;
}

#pragma endregion
