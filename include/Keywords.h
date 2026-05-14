#ifndef CDULL_KEYWORDS
#define CDULL_KEYWORDS

#include <stdbool.h>
#include <stddef.h>
#include <setjmp.h>

#define public extern
#define private static

typedef const void* object;
typedef const char* string;
typedef unsigned char byte;

#pragma region Exceptions

typedef struct _Exception {
    string Message;
} *Exception;

typedef struct _OutOfMemoryException {
    struct _Exception _parent;
    size_t BlockSize;
} *OutOfMemoryException;

enum StackTraceOperation {
    STACKTRACE_GET = -1,
    STACKTRACE_UNSET,
    STACKTRACE_SET
};

/**
 * @brief Initializes a new Exception with the given message.
 */
extern Exception Exception__ctor(string message);
extern OutOfMemoryException OutOfMemoryException__ctor(string message, size_t block_size);

extern jmp_buf _finally_longjmp_buf;
extern jmp_buf _finally_return_longjmp_buf;
extern struct __jmp_buf_tag* StackTrace_Push(void);
extern struct __jmp_buf_tag* StackTrace_Peek(void);
extern struct __jmp_buf_tag* StackTrace_Pop(Exception);
extern Exception StackTrace_ThrownException(void);
extern void StackTrace_Rethrow(void);
extern int StackTrace_UpThrow(enum StackTraceOperation op);
extern int StackTrace_Finally(enum StackTraceOperation op);

#pragma endregion

#pragma region Memory

void* _memalloc(size_t block_size);
void* _zeroalloc(size_t block_size);
void* _memresize(void* old_location, size_t new_size);
void _memfree(void* location);

#define memalloc(CLASS) ((CLASS)_memalloc(sizeof(*(CLASS){0})))
#define arralloc(ARR_TYPE, CAP) ((ARR_TYPE*)_zeroalloc(sizeof(ARR_TYPE) * CAP))
#define memresize(ARR, NEWLEN) ((typeof(ARR))_memresize(ARR, sizeof(*ARR) * NEWLEN))
#define box(STRUCT) ((STRUCT*)_memalloc(sizeof(STRUCT)))
#define memfree(PTR) _memfree(PTR);

#pragma endregion

bool object_Equals(size_t, void*, void*);

#define equals(LEFT, RIGHT) (sizeof(LEFT) == sizeof(RIGHT) && object_Equals(sizeof(LEFT), &LEFT, &RIGHT))

// extern void* _null_coal_helper;
// #define coal(PTR, ELSE) (_null_coal_helper = PTR, _null_coal_helper ? (typeof(PTR))_null_coal_helper : (ELSE))

#define var auto

// Deranged but plausible!
// struct {
//     Exception (*Exception)();
// } Constructors;
// #define new Constructors.

#define NEW_I(TYPE) TYPE##__ctor
#define new(TYPE) NEW_I(TYPE)

#define default(TYPE) ((TYPE){0})

// Solves double macro argument use, but can no longer be used as an expression
// #define dispose(x) do { var _to_dispose = (x); _to_dispose->Dispose(_to_dispose); } while(0)
#define dispose(x) (x)->Dispose(x)

#include "Macros.h"

#define JMP_TO_FIN_THEN if(!setjmp(_finally_return_longjmp_buf)) StackTrace_Finally(STACKTRACE_SET), longjmp(_finally_longjmp_buf, 1); else
#define finally_return(ITEM) do { var _retval = (ITEM); JMP_TO_FIN_THEN return _retval; } while(0)
// Set the UpThrow variable of the stack trace in case no catch block is attached, then set three separate jump
// points ("finally return", `finally` and `catch`), the latter of which is part of the stack trace.
// Finally and finally_return are technically local, so we need to protect them against true long jumps
// by setting them regardless. For makes sure we turn back and pop the stack in the case of
// no errors being thrown. End the block with a `do` to force completion with catch/finally.
#define try if(StackTrace_UpThrow(STACKTRACE_UNSET), StackTrace_Finally(STACKTRACE_UNSET), 1) if(!setjmp(_finally_longjmp_buf)) if(!setjmp(StackTrace_Push())) \
    for (int UNIQ(_once) = 1; UNIQ(_once); (void)(StackTrace_Pop(NULL)), UNIQ(_once) = 0) do
// Close the `do-while` and append an else to the if(!setjmp(StackTrace_Push())), since that's where throw
// statements will send control with a nonzero status.
#define catch(TYPE_NAME) while(0); else if(StackTrace_UpThrow(STACKTRACE_UNSET), 1) PREDEFINE(TYPE_NAME = StackTrace_ThrownException())
// Close the `do-while` (if there is no do-while to close, while(0); does nothing) and use a for
// to loop back after finishing the `finally` and decide whether we have to jump back into code.
#define finally while(0); for (int _continue = 1; _continue; \
    (StackTrace_Finally(STACKTRACE_GET) ? longjmp(_finally_return_longjmp_buf, 1) : (_continue = 0)), \
    (StackTrace_UpThrow(STACKTRACE_GET) ? StackTrace_Rethrow() : 0))
#define throw(EX) ((void)(StackTrace_UpThrow(STACKTRACE_SET), longjmp(StackTrace_Pop((Exception)(EX)), 1)))

#define in ,
#define FOREACH_I(TYPE_VAR, IN)                                                 \
    for(int _once = 1; _once; _once = 0)                                        \
    for(var _src = IN; _once; _once = 0)                                        \
    for(var _e = _src->GetEnumerator(_src); _once; _once = 0, _e->Dispose(_e))  \
        for(int _loop = 1; _loop < 2 && _e->MoveNext(_e); ++_loop)              \
            IF_INFERRED(TYPE_VAR,                                               \
            for(TYPE_VAR = _e->Current; _loop; _loop = 0),                      \
            for([[maybe_unused]] TYPE_VAR, _type; _loop == 1; _loop *= 2)       \
                for(TYPE_VAR = (typeof(_type))_e->Current; _loop; _loop = 0))
/**
 * @brief Enumerates an IEnumerable<T> and sets VAR to
 * the current enumeration value on each iteration.
 */
#define foreach(TYPE_VAR_IN_LIST) FOREACH_I(TYPE_VAR_IN_LIST)
#define foreach_return if(1) { dispose(_e); goto UNIQ(RET); } else UNIQ(RET): return

#endif
