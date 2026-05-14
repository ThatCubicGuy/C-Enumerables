#ifndef C_DELEGATES
#define C_DELEGATES
#define delegate typedef
// Defines a function with parameters and potentially void return value.
#define Func(TReturn, ...) typeof(TReturn (*)(__VA_ARGS__))
// Defines a method pertaining to a specified class.
#define Method(TParent, TReturn, ...) typeof(TReturn (*)(This(TParent), __VA_ARGS__))
// Defines a function that decides whether or not two items are equal.
#define Equator(T) Func(bool, T, T)
// Defines a function that compares two items.
#define Comparer(T) Func(int, T, T)
// Defines a function that decides whether an item fulfills a condition.
#define Predicate(T) Func(bool, T)

#endif