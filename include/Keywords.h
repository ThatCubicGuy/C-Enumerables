#ifndef CDULL_KEYWORDS
#define CDULL_KEYWORDS

#include <stdbool.h>
#include <stddef.h>
#include <setjmp.h>

#include "Macros.h"

#define typeof(x) __typeof__(x)

#pragma region Type Delcarations

typedef const void* object;
typedef const char* string;
typedef unsigned char byte;

// Represents the underlying tag of a typedef variable.
#define TAG(REF_TYPE) struct CAT(tag_,REF_TYPE)
// Implements an interface inside a type.
#define IMPL(REF_TYPE) union { TAG(REF_TYPE) CAT(impl_,REF_TYPE); TAG(REF_TYPE); }

#pragma endregion

#pragma region Exceptions

typedef struct tag_Exception {
    string Message;
} *Exception;

typedef struct tag_OutOfMemoryException {
    struct tag_Exception;
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
extern jmp_buf* StackTrace_Push(void);
extern jmp_buf* StackTrace_Peek(void);
extern jmp_buf* StackTrace_Pop(Exception);
extern Exception StackTrace_ThrownException(void);
extern void StackTrace_Rethrow(void);
extern int StackTrace_UpThrow(enum StackTraceOperation op);
extern int StackTrace_Finally(enum StackTraceOperation op);

#pragma endregion

#pragma region Memory

void* memalloc_(size_t block_size);
void* zeroalloc_(size_t block_size);
void* memresize_(void* old_location, size_t new_size);
void memcopy_(void* dest, const void* source, size_t size);
void memfree_(void* location);

#define memalloc(CLASS) ((CLASS)memalloc_(sizeof(*(CLASS)0)))
#define arralloc(ARR_TYPE, LEN) ((typeof(ARR_TYPE)*)zeroalloc_(sizeof(ARR_TYPE) * LEN))
#define memcopy(DEST, SOURCE, SIZE) memcopy_(DEST, SOURCE, SIZE)
#define memresize(ARR, NEWLEN) ((typeof(ARR))memresize_(ARR, sizeof(*(ARR)) * NEWLEN))
#define boxalloc(STRUCT) ((typeof(STRUCT)*)memalloc_(sizeof(STRUCT)))
#define memfree(PTR) memfree_(PTR)

#define init(CLASS) (typeof(*(typeof(CLASS))0))

#define meminit(CLASS) memalloc(CLASS); *(TAG(CLASS)*)memalloc_(0) = init(CLASS)

#pragma endregion

#pragma region Base equality

#define IEqualityComparer(T) IEqualityComparer_##T
#define EQUALITY_COMPARER_DEFINE(T) \
typedef TAG(IEqualityComparer(T)) { \
    bool (*Equals)(T, T);           \
    size_t (*GetHashCode)(T);       \
} const *IEqualityComparer(T);

#define IComparer(T) IComparer_##T
#define COMPARER_DEFINE(T)  \
typedef TAG(IComparer(T)) { \
    int (*Compare)(T, T);   \
} const *IComparer(T);

bool object_Equals(size_t, object, object);
bool object_ReferenceEquals(object, object);
size_t object_GetHashCode(object obj);

EQUALITY_COMPARER_DEFINE(object);
extern const TAG(IEqualityComparer(object)) ObjectEquator[1];

#define equals(LEFT, RIGHT) (sizeof(typeof(LEFT)) == sizeof(typeof(RIGHT)) && object_Equals(sizeof(typeof(LEFT)), &LEFT, &RIGHT))

#pragma endregion

#define var auto

// Deranged but plausible!
// struct {
//     Exception (*Exception)();
// } Constructors;
// #define new Constructors.

#define new(TYPE) CAT(TYPE,__ctor)

#define default(TYPE) ((TYPE){0})

// Solves double macro argument use, but can no longer be used as an expression
// #define dispose(x) do { var _to_dispose = (x); _to_dispose->Dispose(_to_dispose); } while(0)
#define dispose(x) (x)->Dispose(x)

#define JMP_TO_FIN_THEN if(!setjmp(_finally_return_longjmp_buf)) StackTrace_Finally(STACKTRACE_SET), longjmp(_finally_longjmp_buf, 1); else
#define finally_return(ITEM) do { var _retval = (ITEM); JMP_TO_FIN_THEN return _retval; } while(0)
// Set the UpThrow variable of the stack trace in case no catch block is attached, then set three separate jump
// points ("finally return", `finally` and `catch`), the latter of which is part of the stack trace.
// Finally and finally_return are technically local, so we need to protect them against true long jumps
// by setting them regardless. For makes sure we turn back and pop the stack in the case of
// no errors being thrown. End the block with a `do` to force completion with catch/finally.
#define try if(StackTrace_UpThrow(STACKTRACE_UNSET), StackTrace_Finally(STACKTRACE_UNSET), 1) if(!setjmp(_finally_longjmp_buf)) if(!setjmp(*StackTrace_Push())) \
    for (int UNIQ(_once) = 1; UNIQ(_once); (void)(*StackTrace_Pop(NULL)), UNIQ(_once) = 0) do
// Close the `do-while` and append an else to the if(!setjmp(StackTrace_Push())), since that's where throw
// statements will send control with a nonzero status.
#define catch(TYPE_NAME) while(0); else if(StackTrace_UpThrow(STACKTRACE_UNSET), 1) PREDEFINE(TYPE_NAME = StackTrace_ThrownException())
// Close the `do-while` (if there is no do-while to close, while(0); does nothing) and use a for
// to loop back after finishing the `finally` and decide whether we have to jump back into code.
#define finally while(0); for (int _continue = 1; _continue; \
    (StackTrace_Finally(STACKTRACE_GET) ? longjmp(_finally_return_longjmp_buf, 1) : (_continue = 0)), \
    (StackTrace_UpThrow(STACKTRACE_GET) ? StackTrace_Rethrow() : 0))
// Throw an exception to the nearest catch block.
#define throw for (Exception UNIQ(ex_) = NULL; (StackTrace_UpThrow(STACKTRACE_SET), !UNIQ(ex_)); longjmp(*StackTrace_Pop(UNIQ(ex_)), 1)) UNIQ(ex_) = (Exception)
// ThrowExpression for use in places where statements are not allowed.
#define throwe(EX) ((void)(StackTrace_UpThrow(STACKTRACE_SET), longjmp(*StackTrace_Pop((Exception)(EX)), 1)), NULL)

#define in ,
#define FOREACH_I(TYPE_VAR, IN)                                                 \
    for(int _once = 1; _once; _once = 0)                                        \
    for(typeof(IN) _src = IN; _once; _once = 0)                                 \
    for(typeof(_src->GetEnumerator((void*)_src)) _e = _src->GetEnumerator((void*)_src); _once; _once = 0, _e->Dispose(_e))  \
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
