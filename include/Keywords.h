#ifndef CDULL_KEYWORDS
#define CDULL_KEYWORDS

// Enable debug info
#if 0
// #define DEBUG
#endif

#define STD_C94 1994##09L
#define STD_C99 1999##01L
#define STD_C11 2011##12L
#define STD_C17 2017##10L
#define STD_C23 2023##11L

#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)
#define ADDED_UNUSED 0##00##00

#if GCC_VERSION > ADDED_UNUSED
#define ATTR_UNUSED [[maybe_unused]]
#else
#define ATTR_UNUSED __attribute__((unused))
#endif

#include <stdbool.h>
#include <stddef.h>
#include <setjmp.h>

#include "Macros.h"

#if __STDC_VERSION__ < STD_C23
#define typeof(x) __typeof__(x)
#endif
// Use (void)sizeof(x) to
//  1. Force the parameter to exist, and
//  2. Be a compile time constant through sizeof
#define nameof(x) ((void)sizeof(typeof(x)), #x)

#pragma region Type Delcarations

typedef const void* object;
typedef const char* string;
typedef unsigned char byte;

#pragma endregion

// Represents the underlying tag of a typedef variable.
#define TAG(REF_TYPE) struct CAT(tag_,REF_TYPE)
// Implements an interface inside a type.
#define IMPL(REF_TYPE) union { TAG(REF_TYPE) CAT(impl_,REF_TYPE); TAG(REF_TYPE); }

#define var auto

// Deranged but plausible!
// struct {
//     Exception (*Exception)(string);
//     ...
// } Constructors;
// #define new Constructors.

#define new(TYPE) CAT(TYPE,__ctor)

#define default(TYPE) ((TYPE){0})

#pragma region Debug

#define EXIT_UNHANDLED_EXCEPTION 134
#define EXIT_ASSERT_FAILED 34
#define EXIT_OUT_OF_MEMORY 137
#define EXIT_FAILFAST 1

#ifdef DEBUG
#define DEBUG_WRITELINE(FORMAT, ...) fprintf(stderr, FORMAT"\n",##__VA_ARGS__)
#else
// Anything that may be used as an expression must be defined as something
#define DEBUG_WRITELINE(FORMAT, ...) ((void)0)
#endif

#pragma endregion

#pragma region More important Macros
#define TRACE_ASSERT(CONDITION, ...) if(!(CONDITION)) throw new(Exception)("Assert failed: ("#CONDITION") is false" __VA_OPT__("\n"__VA_ARGS__))
#define TRACE_ASSERT_NOT(CONDITION, ...) if(CONDITION) throw new(Exception)("Assert failed: ("#CONDITION") is true" __VA_OPT__("\n"__VA_ARGS__))
#define TRACE_MEMORYASSERT(PTR) if(!(PTR)) throw new(OutOfMemoryException)(sizeof(*(PTR)))
#define ENVIRONMENT_FAILFAST(MESSAGE...) do { fprintf(stderr, MESSAGE); exit(EXIT_FAILFAST); } while (0)
#define ENVIRONMENT_FAILFAST_IF(CONDITION, MESSAGE...) if (CONDITION) ENVIRONMENT_FAILFAST(MESSAGE)
#define ENVIRONMENT_FAILFAST_IF_NOT(CONDITION, MESSAGE...) if (!(CONDITION)) ENVIRONMENT_FAILFAST(MESSAGE)
#pragma endregion

#pragma region Exceptions

typedef struct tag_Exception {
    string Message;
    string File;
    int Line;
} *Exception;
Exception Exception__ctor(string message, ...);

typedef struct tag_OutOfMemoryException {
    struct tag_Exception;
    size_t BlockSize;
} *OutOfMemoryException;
OutOfMemoryException OutOfMemoryException__ctor(size_t block_size);

typedef struct tag_ArgumentException {
    struct tag_Exception;
    string ArgumentName;
} *ArgumentException;
ArgumentException ArgumentException__ctor(string message, string param_name);

typedef struct tag_ArgumentNullException {
    struct tag_ArgumentException;
} *ArgumentNullException;
ArgumentNullException ArgumentNullException__ctor(string param_name);
#define TO_NULL_CHECK(PARAM) if (!(PARAM)) throw new(ArgumentNullException)(nameof(PARAM));
// Automatically throw an exception if any of the given parameter is null.
#define ThrowIfNull(PARAMS...) do { FOREACH(TO_NULL_CHECK, PARAMS) } while (0)

typedef struct tag_ArgumentOutOfRangeException {
    struct tag_ArgumentException;
    int LowerBound, UpperBound;
} *ArgumentOutOfRangeException;
ArgumentOutOfRangeException ArgumentOutOfRangeException__ctor(string param_name, int lower_bound, int upper_bound);

typedef struct tag_IndexOutOfRangeException {
    struct tag_Exception;
    int Index, ArrayLength;
} *IndexOutOfRangeException;
IndexOutOfRangeException IndexOutOfRangeException__ctor(int index, int max_count);

enum StackTraceOperation {
    STACKTRACE_GET = -1,
    STACKTRACE_UNSET,
    STACKTRACE_SET
};
// Sets information about where the exception was thrown from.
extern void Exception_SetFrame(string filename, size_t line);

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

#define LIBC_MALLOC 0
#define HEAP_ALLOC 1
#define ARENA_ALLOC 2

#define ALLOC_TYPE HEAP_ALLOC

void* memalloc_(size_t block_size);
void* zeroalloc_(size_t block_size);
void* memresize_(void* old_location, size_t new_size);
void memcopy_(void* dest, const void* source, size_t size);
void memfree_(void* address);

#define memalloc(CLASS) (DEBUG_WRITELINE("\033[35mAllocating\033[0m for %s (%s:%d)\n", #CLASS, __FILE__, __LINE__), (typeof(CLASS))memalloc_(sizeof(*(typeof(CLASS))0)))
#define arralloc(ARR_TYPE, LEN) (DEBUG_WRITELINE("\033[35mAllocating array\033[0m of type %s with length %d (%s:%d)\n", #ARR_TYPE, (int)(LEN), __FILE__, __LINE__), (typeof(ARR_TYPE)*)zeroalloc_(sizeof(ARR_TYPE) * LEN))
#define memcopy(DEST, SOURCE, SIZE) (DEBUG_WRITELINE("\033[35mCopying\033[0m from %s to %s (%s:%d)\n", #SOURCE, #DEST, __FILE__, __LINE__), memcopy_(DEST, SOURCE, SIZE))
#define memresize(ARR, NEWLEN) (DEBUG_WRITELINE("\033[35mResizing\033[0m %s to %d size (%s:%d)\n", #ARR, NEWLEN, __FILE__, __LINE__), (typeof(ARR))memresize_(ARR, sizeof(*(ARR)) * NEWLEN))
#define boxalloc(STRUCT) ((typeof(STRUCT)*)memalloc_(sizeof(STRUCT)))
#define memfree(PTR) (DEBUG_WRITELINE("\033[35mFreeing\033[0m %s (%s:%d)\n", #PTR, __FILE__, __LINE__), memfree_(PTR))
#define meminit(CLASS) memalloc(CLASS); *(DEBUG_WRITELINE("\033[35mInitializing\033[0m variable of type %s (%s:%d)", #CLASS, __FILE__, __LINE__), (TAG(CLASS)*)memalloc_(0)) = init(CLASS)

#define init(CLASS) (typeof(*(typeof(CLASS))0))

#pragma endregion

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
#define throw for (Exception UNIQ(ex_) = NULL; (Exception_SetFrame(__FILE__,__LINE__), StackTrace_UpThrow(STACKTRACE_SET), !UNIQ(ex_)); DEBUG_WRITELINE("Exception thrown from (%s:%d): \"%s\"", __FILE__,__LINE__, UNIQ(ex_)->Message), longjmp(*StackTrace_Pop(UNIQ(ex_)), 1)) UNIQ(ex_) = (Exception)
// ThrowExpression for use in places where statements are not allowed.
#define throwe(EX) ((void)(DEBUG_WRITELINE("Exception thrown from (%s:%d): "#EX, __FILE__,__LINE__), Exception_SetFrame(__FILE__,__LINE__), StackTrace_UpThrow(STACKTRACE_SET), longjmp(*StackTrace_Pop((Exception)(EX)), 1)), NULL)

#define in ,
#define FOREACH_I(TYPE_VAR, IN)                                                 \
    for(int once_ = 1; once_; once_ = 0)                                        \
    for(typeof(IN) src_ = IN; once_; once_ = 0)                                 \
    for(typeof(src_->GetEnumerator(NULL)) e_ = src_->GetEnumerator((void*)src_); once_; once_ = 0, e_->Dispose(e_)) \
        for(int loop_ = 1; loop_ < 2 && e_->MoveNext(e_); ++loop_)              \
            IF(IS_INFERRED(TYPE_VAR),                                           \
            for(TYPE_VAR = e_->Current; loop_; loop_ = 0),                      \
            for(ATTR_UNUSED TYPE_VAR, type_; loop_ == 1; loop_ *= 2)            \
                for(TYPE_VAR = (typeof(type_))e_->Current; loop_; loop_ = 0))
/**
 * @brief Enumerates an IEnumerable<T> and sets VAR to
 * the current enumeration value on each iteration.
 */
#define foreach(TYPE_VAR_IN_LIST) FOREACH_I(TYPE_VAR_IN_LIST)
#define foreach_return if(1) { e_->Dispose(e_); goto UNIQ(RET); } else UNIQ(RET): return

#pragma region Base equality

/**
 * @brief Defines a method that may be used to determine
 * whether two objects are equal.
 */
#define IEqualityComparer(T) IEqualityComparer_##T
#define EQUALITY_COMPARER_DEFINE(T) \
typedef TAG(IEqualityComparer(T)) { \
    bool (*Equals)(T, T);           \
    size_t (*GetHashCode)(T);       \
} const *IEqualityComparer(T);

/**
 * @brief Defines a method that may be used to compare two objects.
 * @remarks The comparer should return:
 *  Negative if left precedes right or if left is NULL and right is not NULL.
 *  Zero if left is equal to right or if left and right are both NULL.
 *  Positive if right precedes left or if right is NULL and left is not NULL.
 */
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

#endif
