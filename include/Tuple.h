#ifndef DEFINE_TUPLE
#define DEFINE_TUPLE

#include "Macros.h"

#define TUPLE_DEFINE(...) JOIN(_,TUPLE,NARGS(__VA_ARGS__),DEFINE)(__VA_ARGS__)

// Declares a tuple of the given types.
#define t(...) JOIN(_, __VA_ARGS__)

#define TUPLE_2_DEFINE(T1, T2) typedef struct JOIN(_,tag,ValueTuple,T1,T2) { \
    T1 Item1;   \
    T2 Item2;   \
} t(T1,T2);     \
[[maybe_unused]] static inline t(T1,T2) new(t(T1,T2))(T1 item1, T2 item2) { return (t(T1,T2)) {item1, item2}; } \
[[maybe_unused]] static inline void JOIN(_,Tuple,T1,T2,_dtor)(t(T1,T2) source, T1* out1, T2* out2) \
{                                   \
    if (out1) *out1 = source.Item1; \
    if (out2) *out2 = source.Item2; \
}

#define TUPLE_3_DEFINE(T1, T2, T3) typedef struct JOIN(_,tag,ValueTuple,T1,T2,T3) { \
    T1 Item1;   \
    T2 Item2;   \
    T3 Item3;   \
} t(T1,T2,T3);  \
[[maybe_unused]] static inline t(T1,T2,T3) new(t(T1,T2,T3))(T1 item1, T2 item2, T3 item3) { return (t(T1,T2,T3)) {item1, item2, item3}; } \
[[maybe_unused]] static inline void JOIN(_,Tuple,T1,T2,T3,_dtor)(t(T1,T2,T3) source, T1* out1, T2* out2, T3* out3) \
{                                   \
    if (out1) *out1 = source.Item1; \
    if (out2) *out2 = source.Item2; \
    if (out3) *out3 = source.Item3; \
}

#define TUPLE_4_DEFINE(T1, T2, T3, T4) typedef struct JOIN(_,tag,ValueTuple,T1,T2,T3,T4) { \
    T1 Item1;   \
    T2 Item2;   \
    T3 Item3;   \
    T4 Item4;   \
} t(T1,T2,T3,T4); \
[[maybe_unused]] static inline t(T1,T2,T3,T4) new(t(T1,T2,T3,T4))(T1 item1, T2 item2, T3 item3, T4 item4) { return (t(T1,T2,T3,T4)) {item1, item2, item3, item4}; } \
[[maybe_unused]] static inline void JOIN(_,Tuple,T1,T2,T3,T4,_dtor)(t(T1,T2,T3,T4) source, T1* out1, T2* out2, T3* out3, T4* out4) \
{                                   \
    if (out1) *out1 = source.Item1; \
    if (out2) *out2 = source.Item2; \
    if (out3) *out3 = source.Item3; \
    if (out4) *out4 = source.Item4; \
}

#define TUPLE_5_DEFINE(T1, T2, T3, T4, T5) typedef struct JOIN(_,tag,ValueTuple,T1,T2,T3,T4,T5) { \
    T1 Item1;   \
    T2 Item2;   \
    T3 Item3;   \
    T4 Item4;   \
    T5 Item5;   \
} t(T1,T2,T3,T4,T5); \
[[maybe_unused]] static inline t(T1,T2,T3,T4,T5) new(t(T1,T2,T3,T4,T5))(T1 item1, T2 item2, T3 item3, T4 item4, T5 item5) { return (t(T1,T2,T3,T4,T5)) {item1, item2, item3, item4, item5}; } \
[[maybe_unused]] static inline void JOIN(_,Tuple,T1,T2,T3,T4,T5,_dtor)(t(T1,T2,T3,T4,T5) source, T1* out1, T2* out2, T3* out3, T4* out4, T5* out5) \
{                                   \
    if (out1) *out1 = source.Item1; \
    if (out2) *out2 = source.Item2; \
    if (out3) *out3 = source.Item3; \
    if (out4) *out4 = source.Item4; \
    if (out5) *out5 = source.Item5; \
}

#define TUPLE_6_DEFINE(T1, T2, T3, T4, T5, T6) typedef struct JOIN(_,tag,ValueTuple,T1,T2,T3,T4,T5,T6) { \
    T1 Item1;   \
    T2 Item2;   \
    T3 Item3;   \
    T4 Item4;   \
    T5 Item5;   \
    T6 Item6;   \
} t(T1,T2,T3,T4,T5,T6); \
[[maybe_unused]] static inline t(T1,T2,T3,T4,T5,T6) new(t(T1,T2,T3,T4,T5,T6))(T1 item1, T2 item2, T3 item3, T4 item4, T5 item5, T6 item6) { return (t(T1,T2,T3,T4,T5,T6)) {item1, item2, item3, item4, item5, item6}; } \
[[maybe_unused]] static inline void JOIN(_,Tuple,T1,T2,T3,T4,T5,T6,_dtor)(t(T1,T2,T3,T4,T5,T6) source, T1* out1, T2* out2, T3* out3, T4* out4, T5* out5, T6* out6) \
{                                   \
    if (out1) *out1 = source.Item1; \
    if (out2) *out2 = source.Item2; \
    if (out3) *out3 = source.Item3; \
    if (out4) *out4 = source.Item4; \
    if (out5) *out5 = source.Item5; \
    if (out6) *out6 = source.Item6; \
}

#define TUPLE_7_DEFINE(T1, T2, T3, T4, T5, T6, T7) typedef struct JOIN(_,tag,ValueTuple,T1,T2,T3,T4,T5,T6,T7) { \
    T1 Item1;   \
    T2 Item2;   \
    T3 Item3;   \
    T4 Item4;   \
    T5 Item5;   \
    T6 Item6;   \
    T7 Item7;   \
} t(T1,T2,T3,T4,T5,T6,T7); \
[[maybe_unused]] static inline t(T1,T2,T3,T4,T5,T6,T7) new(t(T1,T2,T3,T4,T5,T6,T7))(T1 item1, T2 item2, T3 item3, T4 item4, T5 item5, T6 item6, T7 item7) { return (t(T1,T2,T3,T4,T5,T6,T7)) {item1, item2, item3, item4, item5, item6, item7}; } \
[[maybe_unused]] static inline void JOIN(_,Tuple,T1,T2,T3,T4,T5,T6,T7,_dtor)(t(T1,T2,T3,T4,T5,T6,T7) source, T1* out1, T2* out2, T3* out3, T4* out4, T5* out5, T6* out6, T7* out7) \
{                                   \
    if (out1) *out1 = source.Item1; \
    if (out2) *out2 = source.Item2; \
    if (out3) *out3 = source.Item3; \
    if (out4) *out4 = source.Item4; \
    if (out5) *out5 = source.Item5; \
    if (out6) *out6 = source.Item6; \
    if (out7) *out7 = source.Item7; \
}

#define TUPLE_8_DEFINE(T1, T2, T3, T4, T5, T6, T7, T8) typedef struct JOIN(_,tag,ValueTuple,T1,T2,T3,T4,T5,T6,T7,T8) { \
    T1 Item1;   \
    T2 Item2;   \
    T3 Item3;   \
    T4 Item4;   \
    T5 Item5;   \
    T6 Item6;   \
    T7 Item7;   \
    T8 Item8;   \
} t(T1,T2,T3,T4,T5,T6,T7,T8); \
[[maybe_unused]] static inline t(T1,T2,T3,T4,T5,T6,T7,T8) new(t(T1,T2,T3,T4,T5,T6,T7,T8))(T1 item1, T2 item2, T3 item3, T4 item4, T5 item5, T6 item6, T7 item7, T8 item8) { return (t(T1,T2,T3,T4,T5,T6,T7,T8)) {item1, item2, item3, item4, item5, item6, item7, item8}; } \
[[maybe_unused]] static inline void JOIN(_,Tuple,T1,T2,T3,T4,T5,T6,T7,T8,_dtor)(t(T1,T2,T3,T4,T5,T6,T7,T8) source, T1* out1, T2* out2, T3* out3, T4* out4, T5* out5, T6* out6, T7* out7, T8* out8) \
{                                   \
    if (out1) *out1 = source.Item1; \
    if (out2) *out2 = source.Item2; \
    if (out3) *out3 = source.Item3; \
    if (out4) *out4 = source.Item4; \
    if (out5) *out5 = source.Item5; \
    if (out6) *out6 = source.Item6; \
    if (out7) *out7 = source.Item7; \
    if (out8) *out8 = source.Item8; \
}

#endif