#ifndef CUBIC_MACROS
#define CUBIC_MACROS

#define E1(x) x
#define E2(x) E1(E1(E1(E1(E1(E1(E1(E1(E1(E1(x)))))))))) // 10^1
#define E3(x) E2(E2(E2(E2(E2(E2(E2(E2(E2(E2(x)))))))))) // 10^2
#define E4(x) E3(E3(E3(E3(E3(E3(E3(E3(E3(E3(x)))))))))) // 10^3
#define EVAL(x) E4(E4(E4(E4(E4(E4(E4(E4(E4(E4(x)))))))))) // 10^4

// 1 step
#define EMPTY()
// 2 steps
#define DEFER(id) id EMPTY()
// 4 steps
#define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()

// EMPTY() ->

// DEFER(x) -> x EMPTY()
// x EMPTY() -> x

// OBSTRUCT(x) -> x DEFER(EMPTY)()
// x DEFER(EMPTY)() -> x EMPTY EMPTY() ()
// x EMPTY EMPTY() () -> x EMPTY ()
// x EMPTY () -> x

#define IDENTITY(x) x

#define EXPAND(...) __VA_ARGS__
#define UNWRAP(...) __VA_ARGS__

#define VOID(...)

// Source - https://stackoverflow.com/a/62984543
// Posted by Nero
// Retrieved 2026-03-10, License - CC BY-SA 4.0

#define __VANISH
#define __ESC_I(...) __VAN##__VA_ARGS__
#define __ESC(...) __ESC_I(__VA_ARGS__)
#define ISH(...) ISH __VA_ARGS__
// Removes one set of parantheses from the given argument.
#define DEPAREN(x) __ESC(ISH x)

#pragma region Concat

#define __CAT2_I(_1, _2) _1##_2
#define __CAT2(_1, _2) __CAT2_I(_1, _2)
#define __CAT3_I(_1, _2, _3) _1##_2##_3
#define __CAT3(_1, _2, _3) __CAT3_I(_1, _2, _3)
#define __CAT4_I(_1, _2, _3, _4) _1##_2##_3##_4
#define __CAT4(_1, _2, _3, _4) __CAT4_I(_1, _2, _3, _4)
#define __CAT5_I(_1, _2, _3, _4, _5) _1##_2##_3##_4##_5
#define __CAT5(_1, _2, _3, _4, _5) __CAT5_I(_1, _2, _3, _4, _5)
#define __CAT6_I(_1, _2, _3, _4, _5, _6) _1##_2##_3##_4##_5##_6
#define __CAT6(_1, _2, _3, _4, _5, _6) __CAT6_I(_1, _2, _3, _4, _5, _6)
#define __CAT7_I(_1, _2, _3, _4, _5, _6, _7) _1##_2##_3##_4##_5##_6##_7
#define __CAT7(_1, _2, _3, _4, _5, _6, _7) __CAT7_I(_1, _2, _3, _4, _5, _6, _7)
#define __CAT8_I(_1, _2, _3, _4, _5, _6, _7, _8) _1##_2##_3##_4##_5##_6##_7##_8
#define __CAT8(_1, _2, _3, _4, _5, _6, _7, _8) __CAT8_I(_1, _2, _3, _4, _5, _6, _7, _8)
#define __CAT9_I(_1, _2, _3, _4, _5, _6, _7, _8, _9) _1##_2##_3##_4##_5##_6##_7##_8##_9
#define __CAT9(_1, _2, _3, _4, _5, _6, _7, _8, _9) __CAT9_I(_1, _2, _3, _4, _5, _6, _7, _8, _9)
#define __CAT10_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10
#define __CAT10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10) __CAT10_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10)
#define __CAT11_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11
#define __CAT11(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) __CAT11_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11)
#define __CAT12_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12
#define __CAT12(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) __CAT12_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12)
#define __CAT13_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13
#define __CAT13(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) __CAT13_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13)
#define __CAT14_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14
#define __CAT14(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) __CAT14_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14)
#define __CAT15_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15
#define __CAT15(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) __CAT15_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15)
#define __CAT16_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16
#define __CAT16(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) __CAT16_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16)
#define __CAT17_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17
#define __CAT17(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) __CAT17_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17)
#define __CAT18_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17##_18
#define __CAT18(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) __CAT18_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18)
#define __CAT19_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17##_18##_19
#define __CAT19(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19) __CAT19_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19)
#define __CAT20_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17##_18##_19##_20
#define __CAT20(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20) __CAT20_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20)
#define __CAT21_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17##_18##_19##_20##_21
#define __CAT21(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21) __CAT21_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21)
#define __CAT22_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17##_18##_19##_20##_21##_22
#define __CAT22(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22) __CAT22_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22)
#define __CAT23_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17##_18##_19##_20##_21##_22##_23
#define __CAT23(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23) __CAT23_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23)
#define __CAT24_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17##_18##_19##_20##_21##_22##_23##_24
#define __CAT24(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24) __CAT24_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24)
#define __CAT25_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17##_18##_19##_20##_21##_22##_23##_24##_25
#define __CAT25(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25) __CAT25_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25)
#define __CAT26_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17##_18##_19##_20##_21##_22##_23##_24##_25##_26
#define __CAT26(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26) __CAT26_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26)
#define __CAT27_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17##_18##_19##_20##_21##_22##_23##_24##_25##_26##_27
#define __CAT27(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27) __CAT27_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27)
#define __CAT28_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17##_18##_19##_20##_21##_22##_23##_24##_25##_26##_27##_28
#define __CAT28(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28) __CAT28_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28)
#define __CAT29_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17##_18##_19##_20##_21##_22##_23##_24##_25##_26##_27##_28##_29
#define __CAT29(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29) __CAT29_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29)
#define __CAT30_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17##_18##_19##_20##_21##_22##_23##_24##_25##_26##_27##_28##_29##_30
#define __CAT30(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30) __CAT30_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30)
#define __CAT31_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17##_18##_19##_20##_21##_22##_23##_24##_25##_26##_27##_28##_29##_30##_31
#define __CAT31(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31) __CAT31_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31)
#define __CAT32_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32) _1##_2##_3##_4##_5##_6##_7##_8##_9##_10##_11##_12##_13##_14##_15##_16##_17##_18##_19##_20##_21##_22##_23##_24##_25##_26##_27##_28##_29##_30##_31##_32
#define __CAT32(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32) __CAT32_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32)

#define __CAT_SELECT(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, NAME, ...) NAME

// Concatenates the values of up to 32 symbols.
#define CAT(...) __CAT_SELECT(,##__VA_ARGS__, __CAT32, __CAT31, __CAT30, __CAT29, __CAT28, __CAT27, __CAT26, __CAT25, __CAT24, __CAT23, __CAT22, __CAT21, __CAT20, __CAT19, __CAT18, __CAT17, __CAT16, __CAT15, __CAT14, __CAT13, __CAT12, __CAT11, __CAT10, __CAT9, __CAT8, __CAT7, __CAT6, __CAT5, __CAT4, __CAT3, __CAT2, IDENTITY, EMPTY)(__VA_ARGS__)

#pragma endregion

#pragma region Select

#define __SELECT_0(id, ...) id
#define __SELECT_1(_0, id, ...) id
#define __SELECT_2(_0, _1, id, ...) id
#define __SELECT_3(_0, _1, _2, id, ...) id
#define __SELECT_4(_0, _1, _2, _3, id, ...) id
#define __SELECT_5(_0, _1, _2, _3, _4, id, ...) id
#define __SELECT_6(_0, _1, _2, _3, _4, _5, id, ...) id
#define __SELECT_7(_0, _1, _2, _3, _4, _5, _6, id, ...) id
#define __SELECT_8(_0, _1, _2, _3, _4, _5, _6, _7, id, ...) id
#define __SELECT_9(_0, _1, _2, _3, _4, _5, _6, _7, _8, id, ...) id
#define __SELECT_10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, id, ...) id
#define __SELECT_11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, id, ...) id
#define __SELECT_12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, id, ...) id
#define __SELECT_13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, id, ...) id
#define __SELECT_14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, id, ...) id
#define __SELECT_15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, id, ...) id
#define __SELECT_16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, id, ...) id
#define __SELECT_17(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, id, ...) id
#define __SELECT_18(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, id, ...) id
#define __SELECT_19(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, id, ...) id
#define __SELECT_20(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, id, ...) id
#define __SELECT_21(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, id, ...) id
#define __SELECT_22(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, id, ...) id
#define __SELECT_23(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, id, ...) id
#define __SELECT_24(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, id, ...) id
#define __SELECT_25(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, id, ...) id
#define __SELECT_26(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, id, ...) id
#define __SELECT_27(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, id, ...) id
#define __SELECT_28(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, id, ...) id
#define __SELECT_29(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, id, ...) id
#define __SELECT_30(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, id, ...) id
#define __SELECT_31(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, id, ...) id
// Selects the item at a zero-based index in the variadic argument list. Up to 32 parameters.
#define SELECT(INDEX, ...) CAT(__SELECT_, INDEX)(__VA_ARGS__)

#pragma endregion

#define qEXTRACT_0(...)
#define qEXTRACT_1(id, ...) id
#define qEXTRACT_2(id, ...) id, qEXTRACT_1(__VA_ARGS__)
#define qEXTRACT_3(id, ...) id, qEXTRACT_2(__VA_ARGS__)
#define qEXTRACT_4(id, ...) id, qEXTRACT_3(__VA_ARGS__)
#define qEXTRACT_5(id, ...) id, qEXTRACT_4(__VA_ARGS__)
#define qEXTRACT_6(id, ...) id, qEXTRACT_5(__VA_ARGS__)
#define qEXTRACT_7(id, ...) id, qEXTRACT_6(__VA_ARGS__)
#define qEXTRACT_8(id, ...) id, qEXTRACT_7(__VA_ARGS__)
#define qEXTRACT_9(id, ...) id, qEXTRACT_8(__VA_ARGS__)
#define qEXTRACT_10(id, ...) id, qEXTRACT_9(__VA_ARGS__)
#define qEXTRACT_11(id, ...) id, qEXTRACT_10(__VA_ARGS__)
#define qEXTRACT_12(id, ...) id, qEXTRACT_11(__VA_ARGS__)
#define qEXTRACT_13(id, ...) id, qEXTRACT_12(__VA_ARGS__)
#define qEXTRACT_14(id, ...) id, qEXTRACT_13(__VA_ARGS__)
#define qEXTRACT_15(id, ...) id, qEXTRACT_14(__VA_ARGS__)
#define qEXTRACT_16(id, ...) id, qEXTRACT_15(__VA_ARGS__)
#define qEXTRACT_17(id, ...) id, qEXTRACT_16(__VA_ARGS__)
#define qEXTRACT_18(id, ...) id, qEXTRACT_17(__VA_ARGS__)
#define qEXTRACT_19(id, ...) id, qEXTRACT_18(__VA_ARGS__)
#define qEXTRACT_20(id, ...) id, qEXTRACT_19(__VA_ARGS__)
#define qEXTRACT_21(id, ...) id, qEXTRACT_20(__VA_ARGS__)
#define qEXTRACT_22(id, ...) id, qEXTRACT_21(__VA_ARGS__)
#define qEXTRACT_23(id, ...) id, qEXTRACT_22(__VA_ARGS__)
#define qEXTRACT_24(id, ...) id, qEXTRACT_23(__VA_ARGS__)
#define qEXTRACT_25(id, ...) id, qEXTRACT_24(__VA_ARGS__)
#define qEXTRACT_26(id, ...) id, qEXTRACT_25(__VA_ARGS__)
#define qEXTRACT_27(id, ...) id, qEXTRACT_26(__VA_ARGS__)
#define qEXTRACT_28(id, ...) id, qEXTRACT_27(__VA_ARGS__)
#define qEXTRACT_29(id, ...) id, qEXTRACT_28(__VA_ARGS__)
#define qEXTRACT_30(id, ...) id, qEXTRACT_29(__VA_ARGS__)
#define qEXTRACT_31(id, ...) id, qEXTRACT_30(__VA_ARGS__)
#define qEXTRACT_32(id, ...) id, qEXTRACT_31(__VA_ARGS__)
// Extracts the first COUNT items from a list. Up to 32 items.
#define EXTRACT(COUNT, ...) CAT(qEXTRACT_, COUNT)(__VA_ARGS__)

// Source - https://stackoverflow.com/a/24010059
// Posted by Alex Celeste, modified by community.
// Retrieved 2026-03-10, License - CC BY-SA 3.0

// count arguments
#define qNARGS_(_32, _31, _30, _29, _28, _27, _26, _25, _24, _23, _22, _21, _20, _19, _18, _17, _16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, N, ...) N
// Get the number of variadic arguments. Up to 32 arguments.
#define NARGS(...) qNARGS_(__VA_ARGS__ __VA_OPT__(,) 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

// Get last argument from a list. Up to 32 arguments.
#define LAST_ARG(...) SELECT(NARGS(__VA_ARGS__), _, __VA_ARGS__)

#define __JOIN_31(SEPARATOR, _1, ...) CAT(SEPARATOR, _1)
#define __JOIN_30(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_31(SEPARATOR, __VA_ARGS__)))
#define __JOIN_29(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_30(SEPARATOR, __VA_ARGS__)))
#define __JOIN_28(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_29(SEPARATOR, __VA_ARGS__)))
#define __JOIN_27(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_28(SEPARATOR, __VA_ARGS__)))
#define __JOIN_26(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_27(SEPARATOR, __VA_ARGS__)))
#define __JOIN_25(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_26(SEPARATOR, __VA_ARGS__)))
#define __JOIN_24(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_25(SEPARATOR, __VA_ARGS__)))
#define __JOIN_23(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_24(SEPARATOR, __VA_ARGS__)))
#define __JOIN_22(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_23(SEPARATOR, __VA_ARGS__)))
#define __JOIN_21(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_22(SEPARATOR, __VA_ARGS__)))
#define __JOIN_20(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_21(SEPARATOR, __VA_ARGS__)))
#define __JOIN_19(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_20(SEPARATOR, __VA_ARGS__)))
#define __JOIN_18(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_19(SEPARATOR, __VA_ARGS__)))
#define __JOIN_17(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_18(SEPARATOR, __VA_ARGS__)))
#define __JOIN_16(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_17(SEPARATOR, __VA_ARGS__)))
#define __JOIN_15(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_16(SEPARATOR, __VA_ARGS__)))
#define __JOIN_14(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_15(SEPARATOR, __VA_ARGS__)))
#define __JOIN_13(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_14(SEPARATOR, __VA_ARGS__)))
#define __JOIN_12(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_13(SEPARATOR, __VA_ARGS__)))
#define __JOIN_11(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_12(SEPARATOR, __VA_ARGS__)))
#define __JOIN_10(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_11(SEPARATOR, __VA_ARGS__)))
#define __JOIN_9(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_10(SEPARATOR, __VA_ARGS__)))
#define __JOIN_8(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_9(SEPARATOR, __VA_ARGS__)))
#define __JOIN_7(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_8(SEPARATOR, __VA_ARGS__)))
#define __JOIN_6(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_7(SEPARATOR, __VA_ARGS__)))
#define __JOIN_5(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_6(SEPARATOR, __VA_ARGS__)))
#define __JOIN_4(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_5(SEPARATOR, __VA_ARGS__)))
#define __JOIN_3(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_4(SEPARATOR, __VA_ARGS__)))
#define __JOIN_2(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_3(SEPARATOR, __VA_ARGS__)))
#define __JOIN_1(SEPARATOR, _1, ...) CAT(CAT(SEPARATOR, _1), __VA_OPT__(__JOIN_2(SEPARATOR, __VA_ARGS__)))
// Joins the values of each argument together with the given separator, up to 32 times.
#define JOIN(SEPARATOR, _1, ...) CAT(_1, __VA_OPT__(__JOIN_1(SEPARATOR, __VA_ARGS__)))

#pragma region For each

#define __FOREACH_1(id, _0, ...) id(_0)
#define __FOREACH_2(id, _0, ...) id(_0) __FOREACH_1(id, __VA_ARGS__)
#define __FOREACH_3(id, _0, ...) id(_0) __FOREACH_2(id, __VA_ARGS__)
#define __FOREACH_4(id, _0, ...) id(_0) __FOREACH_3(id, __VA_ARGS__)
#define __FOREACH_5(id, _0, ...) id(_0) __FOREACH_4(id, __VA_ARGS__)
#define __FOREACH_6(id, _0, ...) id(_0) __FOREACH_5(id, __VA_ARGS__)
#define __FOREACH_7(id, _0, ...) id(_0) __FOREACH_6(id, __VA_ARGS__)
#define __FOREACH_8(id, _0, ...) id(_0) __FOREACH_7(id, __VA_ARGS__)
#define __FOREACH_9(id, _0, ...) id(_0) __FOREACH_8(id, __VA_ARGS__)
#define __FOREACH_10(id, _0, ...) id(_0) __FOREACH_9(id, __VA_ARGS__)
#define __FOREACH_11(id, _0, ...) id(_0) __FOREACH_10(id, __VA_ARGS__)
#define __FOREACH_12(id, _0, ...) id(_0) __FOREACH_11(id, __VA_ARGS__)
#define __FOREACH_13(id, _0, ...) id(_0) __FOREACH_12(id, __VA_ARGS__)
#define __FOREACH_14(id, _0, ...) id(_0) __FOREACH_13(id, __VA_ARGS__)
#define __FOREACH_15(id, _0, ...) id(_0) __FOREACH_14(id, __VA_ARGS__)
#define __FOREACH_16(id, _0, ...) id(_0) __FOREACH_15(id, __VA_ARGS__)
#define __FOREACH_17(id, _0, ...) id(_0) __FOREACH_16(id, __VA_ARGS__)
#define __FOREACH_18(id, _0, ...) id(_0) __FOREACH_17(id, __VA_ARGS__)
#define __FOREACH_19(id, _0, ...) id(_0) __FOREACH_18(id, __VA_ARGS__)
#define __FOREACH_20(id, _0, ...) id(_0) __FOREACH_19(id, __VA_ARGS__)
#define __FOREACH_21(id, _0, ...) id(_0) __FOREACH_20(id, __VA_ARGS__)
#define __FOREACH_22(id, _0, ...) id(_0) __FOREACH_21(id, __VA_ARGS__)
#define __FOREACH_23(id, _0, ...) id(_0) __FOREACH_22(id, __VA_ARGS__)
#define __FOREACH_24(id, _0, ...) id(_0) __FOREACH_23(id, __VA_ARGS__)
#define __FOREACH_25(id, _0, ...) id(_0) __FOREACH_24(id, __VA_ARGS__)
#define __FOREACH_26(id, _0, ...) id(_0) __FOREACH_25(id, __VA_ARGS__)
#define __FOREACH_27(id, _0, ...) id(_0) __FOREACH_26(id, __VA_ARGS__)
#define __FOREACH_28(id, _0, ...) id(_0) __FOREACH_27(id, __VA_ARGS__)
#define __FOREACH_29(id, _0, ...) id(_0) __FOREACH_28(id, __VA_ARGS__)
#define __FOREACH_30(id, _0, ...) id(_0) __FOREACH_29(id, __VA_ARGS__)
#define __FOREACH_31(id, _0, ...) id(_0) __FOREACH_30(id, __VA_ARGS__)
#define __FOREACH_32(id, _0, ...) id(_0) __FOREACH_31(id, __VA_ARGS__)
// Execute a macro for each of the parameters in a collection, up to 32 times.
#define FOREACH(id, ...) __VA_OPT__(CAT(__FOREACH_, NARGS(__VA_ARGS__))(id, __VA_ARGS__))

#define __FOREACH_ARGS_1(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0)))
#define __FOREACH_ARGS_2(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_1(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_3(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_2(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_4(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_3(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_5(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_4(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_6(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_5(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_7(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_6(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_8(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_7(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_9(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_8(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_10(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_9(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_11(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_10(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_12(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_11(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_13(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_12(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_14(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_13(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_15(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_14(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_16(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_15(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_17(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_16(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_18(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_17(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_19(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_18(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_20(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_19(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_21(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_20(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_22(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_21(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_23(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_22(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_24(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_23(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_25(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_24(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_26(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_25(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_27(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_26(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_28(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_27(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_29(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_28(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_30(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_29(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_31(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_30(id, BASEARGS, __VA_ARGS__)
#define __FOREACH_ARGS_32(id, BASEARGS, _0, ...) id(EXPAND(DEPAREN(BASEARGS), DEPAREN(_0))) __FOREACH_ARGS_31(id, BASEARGS, __VA_ARGS__)
// Execute a macro for each of the parameters in a collection, up to 32 times. Allows extra parameters in front of the list parameter.
#define FOREACH_WITH_ARGS(id, BASEARGS, ...) __VA_OPT__(CAT(__FOREACH_ARGS_, NARGS(__VA_ARGS__)(id, BASEARGS, __VA_ARGS__)))

#define __FOREACH_INDEX_1(idx, id, _0, ...) id(idx, _0)
#define __FOREACH_INDEX_2(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_1(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_3(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_2(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_4(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_3(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_5(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_4(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_6(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_5(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_7(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_6(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_8(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_7(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_9(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_8(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_10(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_9(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_11(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_10(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_12(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_11(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_13(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_12(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_14(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_13(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_15(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_14(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_16(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_15(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_17(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_16(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_18(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_17(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_19(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_18(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_20(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_19(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_21(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_20(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_22(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_21(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_23(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_22(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_24(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_23(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_25(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_24(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_26(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_25(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_27(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_26(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_28(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_27(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_29(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_28(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_30(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_29(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_31(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_30(CAT(INC_,idx), id, __VA_ARGS__)
#define __FOREACH_INDEX_32(idx, id, _0, ...) id(idx, _0) __FOREACH_INDEX_31(CAT(INC_,idx), id, __VA_ARGS__)
// Executes a macro for each item in a collection, passing the index of the item as the first param.
#define FOREACH_INDEX(start_index, id, ...) __VA_OPT__(CAT(__FOREACH_INDEX_, NARGS(__VA_ARGS__))(start_index, id, __VA_ARGS__))

#pragma endregion

#pragma region Dec / Inc

#define DEC_0 0
#define DEC_1 0
#define DEC_2 1
#define DEC_3 2
#define DEC_4 3
#define DEC_5 4
#define DEC_6 5
#define DEC_7 6
#define DEC_8 7
#define DEC_9 8
#define DEC_10 9
#define DEC_11 10
#define DEC_12 11
#define DEC_13 12
#define DEC_14 13
#define DEC_15 14
#define DEC_16 15
#define DEC_17 16
#define DEC_18 17
#define DEC_19 18
#define DEC_20 19
#define DEC_21 20
#define DEC_22 21
#define DEC_23 22
#define DEC_24 23
#define DEC_25 24
#define DEC_26 25
#define DEC_27 26
#define DEC_28 27
#define DEC_29 28
#define DEC_30 29
#define DEC_31 30
#define DEC_32 31
#define DEC(x) CAT(DEC_,x)

#define INC_0 1
#define INC_1 2
#define INC_2 3
#define INC_3 4
#define INC_4 5
#define INC_5 6
#define INC_6 7
#define INC_7 8
#define INC_8 9
#define INC_9 10
#define INC_10 11
#define INC_11 12
#define INC_12 13
#define INC_13 14
#define INC_14 15
#define INC_15 16
#define INC_16 17
#define INC_17 18
#define INC_18 19
#define INC_19 20
#define INC_20 21
#define INC_21 22
#define INC_22 23
#define INC_23 24
#define INC_24 25
#define INC_25 26
#define INC_26 27
#define INC_27 28
#define INC_28 29
#define INC_29 30
#define INC_30 31
#define INC_31 32
#define INC_32 32
#define INC(x) CAT(INC_,x)

#pragma endregion

#pragma region Other useful things

#define __APPEND_COMMA(...) __VA_OPT__(__VA_ARGS__,)
#define __PREPEND_COMMA(...) __VA_OPT__(,__VA_ARGS__)
#define TRIMEND(...) EXTRACT(DEC(NARGS(__VA_ARGS__)),__VA_ARGS__) __PREPEND_COMMA(SELECT(DEC(NARGS(__VA_ARGS__)),__VA_ARGS__))
#define TRIMSTART(x, ...) __APPEND_COMMA(x) __VA_ARGS__
#define TRIM(...) TRIMEND(TRIMSTART(__VA_ARGS__))

#define APPEND(ARG,ARGS) (TRIMSTART(DEPAREN(ARGS), ARG))
#define PREPEND(ARG,ARGS) (TRIMEND(ARG, DEPAREN(ARGS)))
#define CATARGS(L_ARGS,R_ARGS) (DEFER(TRIM)(DEPAREN(L_ARGS), DEPAREN(R_ARGS)))

// Concatenates the line number to an identifier in order to make it unique to a macro.
#define UNIQ(x) CAT(x,__LINE__)

#define NOT_0 ~,
#define NOT(x) SELECT(2,CAT(NOT_,x),1,0)
#define COMPL_0 1
#define COMPL_1 0
#define COMPL(x) CAT(COMPL_,x)
#define BOOL(x) COMPL(NOT(x))

#define IF_0(t,f) f
#define IF_1(t,f) t
#define IF(EXPR,IF,ELSE) CAT(IF_,BOOL(EXPR))(IF,ELSE)

#define INFER_var ,
#define INFER_auto ,
#define IS_INFERRED(TYPE) SELECT(2, CAT(INFER_,TYPE), 1, 0)

// CAST macro for turning a type name into a cast
// unless it is `auto` or `var`
#define CAST(TYPE) IF(IS_INFERRED(TYPE),,(TYPE))

#define __TO_FOR_DECL(TYPE_NAME_INIT) for(TYPE_NAME_INIT; once_; once_ = 0)
#define PREDEFINE(TYPE_NAME_INIT...) for(int once_ = 1; once_; once_ = 0) FOREACH(__TO_FOR_DECL, TYPE_NAME_INIT)

#define PREPEND_INSTR(INSTR) if(1) { INSTR; goto UNIQ(INSERT); } else UNIQ(INSERT):

#pragma endregion

#endif
