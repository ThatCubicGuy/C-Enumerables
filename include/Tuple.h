#ifndef DEFINE_TUPLE
#define DEFINE_TUPLE

#define TUPLE_2_DEFINE(T1, T2) typedef struct T1##_##T2##_tuple {  \
    T1 Item1;           \
    T2 Item2;           \
} T1##_##T2;            \
T1##_##T2 T1##_##T2##__ctor(T1 item1, T2 item2);    \
void T1##_##T2##__dtor(T1##_##T2 source, T1* out1, T2* out2);

#define TUPLE_3_DEFINE(T1, T2, T3) typedef struct T1##_##T2##_##T3##_tuple {   \
    T1 Item1;           \
    T2 Item2;           \
} T1##_##T2##_##T3;     \
T1##_##T2##_##T3 T1##_##T2##_##T3##__ctor(T1 item1, T2 item2, T3 item3);    \
void T1##_##T2##_##T3##__dtor(T1##_##T2##_##T3 source, T1* out1, T2* out2, T3* out3);

#define TUPLE_2_IMPLEMENT(T1, T2)   \
T1##_##T2 T1##_##T2##__ctor(T1 item1, T2 item2) { return (T1##_##T2) {item1, item2}; }  \
void T1##_##T2##__dtor(T1##_##T2 source, T1* out1, T2* out2)    \
{                                   \
    if (out1) *out1 = source.Item1; \
    if (out2) *out2 = source.Item2; \
}

#define TUPLE_3_IMPLEMENT(T1, T2)   \
T1##_##T2##_##T3 T1##_##T2##_##T3##__ctor(T1 item1, T2 item2, T3 item3) { return (T1##_##T2##_##T3) {item1, item2, item3}; }    \
void T1##_##T2##_##T3##__dtor(T1##_##T2##_##T3 source, T1* out1, T2* out2, T3* out3)    \
{                                   \
    if (out1) *out1 = source.Item1; \
    if (out2) *out2 = source.Item2; \
    if (out3) *out3 = source.Item3; \
}

#endif