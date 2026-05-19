#include "Keywords.h"
#include "System.h"
#include <stdio.h>
#include <stdarg.h>

jmp_buf _finally_longjmp_buf = default(jmp_buf);
jmp_buf _finally_return_longjmp_buf = default(jmp_buf);

TAG(exception_source) {
    string filename[STACKTRACE_SIZE];
    size_t line[STACKTRACE_SIZE];
    size_t current_index;
} exception_source = {
    .filename = {0},
    .line = {0},
    .current_index = 0,
};

void Exception_SetFrame(string filename, size_t line)
{
    ENVIRONMENT_FAILFAST_IF(exception_source.current_index >= STACKTRACE_SIZE, "CATASTROPHIC FAILURE: StackTrace overflow\nOverflow values: (%s:%zu)\n", filename, line);
    exception_source.filename[exception_source.current_index] = filename;
    exception_source.line[exception_source.current_index] = line;
    exception_source.current_index += 1;
}

#define init_Exception(TYPE, VAR, MESSAGE...) TYPE VAR = memalloc(TYPE); *(Exception)VAR = Exception__init(MESSAGE)
static inline struct tag_Exception VException__init(string message, va_list args)
{
    char buf[256];
    vsprintf(buf, message, args);
    exception_source.current_index -= 1;
    return (struct tag_Exception) {
        .Message = new(string)(buf),
        .File = new(string)(exception_source.filename[exception_source.current_index]),
        .Line = exception_source.line[exception_source.current_index],
    };
}
static inline struct tag_Exception Exception__init(string message, ...)
{
    va_list args;
    va_start(args, message);
    struct tag_Exception result = VException__init(message, args);
    va_end(args);
    return result;
}

Exception Exception__ctor(string msg, ...)
{
    Exception result = memalloc(Exception);
    if (!result) throw new(OutOfMemoryException)(sizeof(*result));
    va_list args;
    va_start(args, msg);
    *result = VException__init(msg, args);
    va_end(args);
    return result;
}

// Needed because when you run out of memory it's not really a good idea to malloc lmao
struct tag_OutOfMemoryException last_out_of_memory_exception[1] = {0};
OutOfMemoryException OutOfMemoryException__ctor(size_t size)
{
    last_out_of_memory_exception[0] = (struct tag_OutOfMemoryException) {
        Exception__init("Insufficient memory to continue the execution of the program."),
        .BlockSize = size,
    };
    return last_out_of_memory_exception;
}

ArgumentException ArgumentException__ctor(string message, string param_name)
{
    ArgumentException result = meminit(ArgumentException) {
        Exception__init("%s\nParameter name: %s", message, param_name),
        .ArgumentName = new(string)(param_name),
    };
    return result;
}

ArgumentNullException ArgumentNullException__ctor(string param_name)
{
    ArgumentNullException result = (ArgumentNullException)new(ArgumentException)("Value cannot be null.", param_name);
    result = memresize(result, 1);
    result->ArgumentName = new(string)(param_name);
    return result;
}

ArgumentOutOfRangeException ArgumentOutOfRangeException__ctor(string param_name, int lower_bound, int upper_bound)
{
    ArgumentOutOfRangeException result = (ArgumentOutOfRangeException)new(ArgumentException)("Value cannot be null.", param_name);
    result->LowerBound = lower_bound;
    result->UpperBound = upper_bound;
    return result;
}

IndexOutOfRangeException IndexOutOfRangeException__ctor(int index, int max_count)
{
    init_Exception(IndexOutOfRangeException, result, "Index was outside the bounds of the array.\nIndex: %d\nArray length: %d", index, max_count);
    result->Index = index;
    result->ArrayLength = max_count;
    return result;
}
