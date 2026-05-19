#include "Keywords.h"
#include "System.h"

#pragma region Stack Trace

static TAG(StackTraceStatic) {
    Exception LastThrownException;
    int Tail;
    bool UpThrow;
    bool FinallyReturnsToCode;
    jmp_buf Stack[STACKTRACE_SIZE][1];
} StackTrace = {
    .LastThrownException = NULL,
    .Tail = 0,
    .UpThrow = false,
    .FinallyReturnsToCode = false,
    .Stack = {{{{{0},0,{{0}}}}}},
};

jmp_buf* StackTrace_Push(void)
{
    DEBUG_WRITELINE("Called StackTrace_Push. Count before: %d", StackTrace.Tail);
    StackTrace.Tail += 1;
    TRACE_ASSERT(StackTrace.Tail > 0);
    return StackTrace.Stack[StackTrace.Tail - 1];
}

jmp_buf* StackTrace_Peek(void)
{
    DEBUG_WRITELINE("Called StackTrace_Peek. Count: %d", StackTrace.Tail);
    TRACE_ASSERT(StackTrace.Tail > 0);
    return StackTrace.Stack[StackTrace.Tail - 1];
}

jmp_buf* StackTrace_Pop(Exception ex)
{
    DEBUG_WRITELINE("Called StackTrace_Pop. Count before: %d", StackTrace.Tail);
    if (StackTrace.Tail == 0) {
        fprintf(stderr, "Unhandled exception: \"%s\"\n", ex ? ex->Message : "(nil)");
        exit(EXIT_UNHANDLED_EXCEPTION);
    }
    TRACE_ASSERT(StackTrace.Tail > 0 && StackTrace.Tail < STACKTRACE_SIZE, "StackTrace fields:\nLast thrown exception: %p\nTail (Count): %d\nUpThrow set: %d\nFinallyReturnsToCode set: %d\nStack[0]: %p\n", StackTrace.LastThrownException, StackTrace.Tail, StackTrace.UpThrow, StackTrace.FinallyReturnsToCode, StackTrace.Stack[0]);
    StackTrace.LastThrownException = ex;
    StackTrace.Tail -= 1;
    return StackTrace.Stack[StackTrace.Tail];
}

Exception StackTrace_ThrownException(void)
{
    DEBUG_WRITELINE("Called StackTrace_ThrownException.");
    return StackTrace.LastThrownException;
}

void StackTrace_Rethrow(void)
{
    DEBUG_WRITELINE("Called StackTrace_Rethrow.");
    throw StackTrace.LastThrownException;
}

int StackTrace_UpThrow(enum StackTraceOperation op)
{
    DEBUG_WRITELINE("Called StackTrace_UpThrow with op %d, Value: %d", op, StackTrace.UpThrow);
    if (op != STACKTRACE_GET) StackTrace.UpThrow = op; 
    return StackTrace.UpThrow;
}

int StackTrace_Finally(enum StackTraceOperation op)
{
    DEBUG_WRITELINE("Called StackTrace_Finally with op %d. Value: %d", op, StackTrace.FinallyReturnsToCode);
    if (op != STACKTRACE_GET) StackTrace.FinallyReturnsToCode = op; 
    return StackTrace.FinallyReturnsToCode;
}

#pragma endregion
