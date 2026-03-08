#ifndef DEFINE_TUPLE
#define DEFINE_TUPLE

#define TUPLE_2_DECLARE(T1, T2) typedef struct T1##_##T2##_tuple {  \
    T1 Item1;           \
    T2 Item2;           \
} T1##_##T2;          \
T1##_##T2 T1##_##T2##__ctor(T1 item1, T2 item2) { return (T1##_##T2) {item1, item2}; }

#define TUPLE_3_DECLARE(T1, T2, T3) typedef struct T1##_##T2##_##T3##_tuple {   \
    T1 Item1;           \
    T2 Item2;           \
} T1##_##T2##_##T3;     \
T1##_##T2##_##T3 T1##_##T2##_##T3##__ctor(T1 item1, T2 item2, T3 item3) { return (T1##_##T2##_##T3) {item1, item2, item3}; }

#endif