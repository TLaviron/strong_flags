
#ifndef INCLUDE_STRONG_FLAGS_H_
#define INCLUDE_STRONG_FLAGS_H_

#include <type_traits>
#include <utility>
#include <climits>

namespace strong_flags {


template<typename FlagType, typename Integer, std::size_t N>
class impl {

public:
    using this_type = impl<FlagType, Integer, N>;
    using bit_type = std::size_t;
    using underlying_type = Integer;

    constexpr impl() noexcept : m_value { } {
    }

    constexpr impl(const FlagType& rhs) noexcept : m_value { static_cast<const this_type&>(rhs).m_value } {
    }

    static constexpr FlagType from_underlying_type(underlying_type value) noexcept {
        FlagType res;
        static_cast<this_type&>(res).m_value = value & s_mask;
        return res;
    }

    static constexpr FlagType from_bit(bit_type bit) noexcept {
        return from_underlying_type(s_one << bit);
    }

    constexpr bool operator==(const FlagType& rhs) noexcept {
        return m_value == static_cast<const this_type&>(rhs).m_value;
    }

    constexpr bool operator!=(const FlagType& rhs) noexcept {
        return m_value != static_cast<const this_type&>(rhs).m_value;
    }

    constexpr bool test(bit_type bit) noexcept {
        return ((1 << bit) & m_value) != 0;
    }

    constexpr bool test_any(const FlagType& rhs) noexcept {
        return (m_value & static_cast<const this_type&>(rhs).m_value) != 0;
    }

    constexpr bool test_all(const FlagType& rhs) noexcept {
        const auto& other = static_cast<const this_type&>(rhs);
        return (m_value & other.m_value) == other.m_value;
    }

    FlagType& operator|=(const FlagType& rhs) noexcept {
        m_value |= static_cast<const this_type&>(rhs).m_value;
        return *static_cast<FlagType*>(this);
    }

    FlagType& operator&=(const FlagType& rhs) noexcept {
        m_value &= static_cast<const this_type&>(rhs).m_value;
        return *static_cast<FlagType*>(this);
    }

    FlagType& operator^=(const FlagType& rhs) noexcept {
        m_value ^= static_cast<const this_type&>(rhs).m_value;
        return *static_cast<FlagType*>(this);
    }

    constexpr FlagType operator~() noexcept {
        return from_underlying_type((~m_value) & s_mask);
    }

private:
    Integer m_value;

    using unsigned_type = typename std::make_unsigned<underlying_type>::type;

    static constexpr bit_type s_bitsize = sizeof(underlying_type) * CHAR_BIT;
    static constexpr auto s_mask = static_cast<underlying_type>((~static_cast<unsigned_type>(0)) >> (s_bitsize - N));
    static constexpr auto s_one = static_cast<underlying_type>(1);

    static_assert(std::is_integral<Integer>::value,
            "Underlying type must be integer type or bitset type");
    static_assert(s_bitsize >= N, "Integer type too small");
};

template<typename FlagType, typename Integer, std::size_t N>
constexpr FlagType operator|(const impl<FlagType, Integer, N>& lhs, const impl<FlagType, Integer, N>& rhs) noexcept {
    impl<FlagType, Integer, N> res(lhs);
    return res |= rhs;
}

template<typename FlagType, typename Integer, std::size_t N>
constexpr FlagType operator&(const impl<FlagType, Integer, N>& lhs, const impl<FlagType, Integer, N>& rhs) noexcept {
    impl<FlagType, Integer, N> res(lhs);
    return res &= rhs;
}

template<typename FlagType, typename Integer, std::size_t N>
constexpr FlagType operator^(const impl<FlagType, Integer, N>& lhs, const impl<FlagType, Integer, N>& rhs) noexcept {
    impl<FlagType, Integer, N> res(lhs);
    return res ^= rhs;
}

template<typename FlagType, template<std::size_t> typename Bitset, std::size_t N>
class impl<FlagType, Bitset<N>, N> {
public:
    using this_type = impl<FlagType, Bitset<N>, N>;
    using bit_type = std::size_t;
    using underlying_type = Bitset<N>;

    constexpr impl() noexcept : m_value { } {
    }

    constexpr impl(const FlagType& rhs) noexcept : m_value { static_cast<const this_type&>(rhs).m_value } {
    }

    constexpr bool operator==(const FlagType& rhs) noexcept {
        return m_value == static_cast<const this_type&>(rhs).m_value;
    }

    constexpr bool operator!=(const FlagType& rhs) noexcept {
        return m_value != static_cast<const this_type&>(rhs).m_value;
    }

    constexpr bool test(bit_type bit) {
        return m_value[bit];
    }

    constexpr bool test_any(const FlagType& rhs) noexcept {
        return (m_value & static_cast<const this_type&>(rhs).m_value).any();
    }

    constexpr bool test_all(const FlagType& rhs) noexcept {
        return (m_value & static_cast<const this_type&>(rhs).m_value).all();
    }

    FlagType& operator|=(const FlagType& rhs) noexcept {
        m_value |= static_cast<const this_type&>(rhs).m_value;
        return *static_cast<FlagType*>(this);
    }

    FlagType& operator&=(const FlagType& rhs) noexcept {
        m_value |= static_cast<const this_type&>(rhs).m_value;
        return *static_cast<FlagType*>(this);
    }

    FlagType& operator^=(const FlagType& rhs) noexcept {
        m_value |= static_cast<const this_type&>(rhs).m_value;
        return *static_cast<FlagType*>(this);
    }

    FlagType operator~() noexcept {
        return from_underlying_type(~m_value);
    }

private:
    Bitset<N> m_value;

    static_assert(std::is_base_of<this_type, FlagType>::value, "Flagtype should derive from implementation class");
};

}
#define STRONG_FLAGS_ARG_COUNT_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17,     \
    _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39,   \
    _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61,   \
    _62, _63, _64, count, ...) count

#define STRONG_FLAGS_ARG_COUNT(...)                                                                                 \
    STRONG_FLAGS_ARG_COUNT_IMPL(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48,    \
            47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, \
            21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define STRONG_FLAGS_PASTE_IMPL(a, b) a##b

#define STRONG_FLAGS_PASTE(a, b) STRONG_FLAGS_PASTE_IMPL(a, b)

#define STRONG_FLAGS_MAKE_FLAG_BIT(name, bit) static constexpr type::bit_type STRONG_FLAGS_PASTE(name, _bit) = (bit)
#define STRONG_FLAGS_MAKE_FLAG_VALUE(name, bit) static constexpr type name = type::from_bit(bit)

#define STRONG_FLAGS_MAKE_FLAG(name, bit)       \
    STRONG_FLAGS_MAKE_FLAG_BIT(name, bit);      \
    STRONG_FLAGS_MAKE_FLAG_VALUE(name, bit)

#define STRONG_FLAGS_MAKE_FLAGS_1(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 1)
#define STRONG_FLAGS_MAKE_FLAGS_2(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 2);                        \
    STRONG_FLAGS_MAKE_FLAGS_1(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_3(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 3);                        \
    STRONG_FLAGS_MAKE_FLAGS_2(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_4(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 4);                        \
    STRONG_FLAGS_MAKE_FLAGS_3(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_5(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 5);                        \
    STRONG_FLAGS_MAKE_FLAGS_4(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_6(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 6);                        \
    STRONG_FLAGS_MAKE_FLAGS_5(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_7(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 7);                        \
    STRONG_FLAGS_MAKE_FLAGS_6(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_8(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 8);                        \
    STRONG_FLAGS_MAKE_FLAGS_7(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_9(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 9);                        \
    STRONG_FLAGS_MAKE_FLAGS_8(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_10(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 10);                      \
    STRONG_FLAGS_MAKE_FLAGS_9(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_11(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 11);                      \
    STRONG_FLAGS_MAKE_FLAGS_10(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_12(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 12);                      \
    STRONG_FLAGS_MAKE_FLAGS_11(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_13(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 13);                      \
    STRONG_FLAGS_MAKE_FLAGS_12(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_14(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 14);                      \
    STRONG_FLAGS_MAKE_FLAGS_13(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_15(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 15);                      \
    STRONG_FLAGS_MAKE_FLAGS_14(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_16(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 16);                      \
    STRONG_FLAGS_MAKE_FLAGS_15(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_17(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 17);                      \
    STRONG_FLAGS_MAKE_FLAGS_16(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_18(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 18);                      \
    STRONG_FLAGS_MAKE_FLAGS_17(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_19(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 19);                      \
    STRONG_FLAGS_MAKE_FLAGS_18(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_20(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 20);                      \
    STRONG_FLAGS_MAKE_FLAGS_19(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_21(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 21);                      \
    STRONG_FLAGS_MAKE_FLAGS_20(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_22(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 22);                      \
    STRONG_FLAGS_MAKE_FLAGS_21(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_23(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 23);                      \
    STRONG_FLAGS_MAKE_FLAGS_22(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_24(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 24);                      \
    STRONG_FLAGS_MAKE_FLAGS_23(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_25(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 25);                      \
    STRONG_FLAGS_MAKE_FLAGS_24(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_26(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 26);                      \
    STRONG_FLAGS_MAKE_FLAGS_25(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_27(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 27);                      \
    STRONG_FLAGS_MAKE_FLAGS_26(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_28(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 28);                      \
    STRONG_FLAGS_MAKE_FLAGS_27(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_29(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 29);                      \
    STRONG_FLAGS_MAKE_FLAGS_28(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_30(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 30);                      \
    STRONG_FLAGS_MAKE_FLAGS_29(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_31(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 31);                      \
    STRONG_FLAGS_MAKE_FLAGS_30(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_32(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 32);                      \
    STRONG_FLAGS_MAKE_FLAGS_31(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_33(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 33);                      \
    STRONG_FLAGS_MAKE_FLAGS_32(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_34(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 34);                      \
    STRONG_FLAGS_MAKE_FLAGS_33(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_35(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 35);                      \
    STRONG_FLAGS_MAKE_FLAGS_34(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_36(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 36);                      \
    STRONG_FLAGS_MAKE_FLAGS_35(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_37(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 37);                      \
    STRONG_FLAGS_MAKE_FLAGS_36(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_38(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 38);                      \
    STRONG_FLAGS_MAKE_FLAGS_37(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_39(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 39);                      \
    STRONG_FLAGS_MAKE_FLAGS_38(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_40(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 40);                      \
    STRONG_FLAGS_MAKE_FLAGS_39(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_41(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 41);                      \
    STRONG_FLAGS_MAKE_FLAGS_40(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_42(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 42);                      \
    STRONG_FLAGS_MAKE_FLAGS_41(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_43(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 43);                      \
    STRONG_FLAGS_MAKE_FLAGS_42(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_44(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 44);                      \
    STRONG_FLAGS_MAKE_FLAGS_43(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_45(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 45);                      \
    STRONG_FLAGS_MAKE_FLAGS_44(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_46(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 46);                      \
    STRONG_FLAGS_MAKE_FLAGS_45(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_47(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 47);                      \
    STRONG_FLAGS_MAKE_FLAGS_46(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_48(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 48);                      \
    STRONG_FLAGS_MAKE_FLAGS_47(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_49(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 49);                      \
    STRONG_FLAGS_MAKE_FLAGS_48(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_50(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 50);                      \
    STRONG_FLAGS_MAKE_FLAGS_49(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_51(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 51);                      \
    STRONG_FLAGS_MAKE_FLAGS_50(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_52(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 52);                      \
    STRONG_FLAGS_MAKE_FLAGS_51(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_53(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 53);                      \
    STRONG_FLAGS_MAKE_FLAGS_52(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_54(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 54);                      \
    STRONG_FLAGS_MAKE_FLAGS_53(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_55(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 55);                      \
    STRONG_FLAGS_MAKE_FLAGS_54(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_56(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 56);                      \
    STRONG_FLAGS_MAKE_FLAGS_55(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_57(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 57);                      \
    STRONG_FLAGS_MAKE_FLAGS_56(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_58(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 58);                      \
    STRONG_FLAGS_MAKE_FLAGS_57(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_59(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 59);                      \
    STRONG_FLAGS_MAKE_FLAGS_58(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_60(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 60);                      \
    STRONG_FLAGS_MAKE_FLAGS_59(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_61(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 61);                      \
    STRONG_FLAGS_MAKE_FLAGS_60(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_62(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 62);                      \
    STRONG_FLAGS_MAKE_FLAGS_61(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_63(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 63);                      \
    STRONG_FLAGS_MAKE_FLAGS_62(count, __VA_ARGS__)
#define STRONG_FLAGS_MAKE_FLAGS_64(count, flag, ...) STRONG_FLAGS_MAKE_FLAG(flag, count - 64);                      \
    STRONG_FLAGS_MAKE_FLAGS_63(count, __VA_ARGS__)

#define STRONG_FLAGS_MAKE_FLAGS(count, ...) \
    STRONG_FLAGS_PASTE(STRONG_FLAGS_MAKE_FLAGS_, count)(count, __VA_ARGS__)



#define STRONG_FLAGS_DEFINE_CLASS(underlying_type, bitsize)                                                 \
    class type : public ::strong_flags::impl<type, underlying_type, bitsize> {                              \
    private:                                                                                                \
        using base_type = ::strong_flags::impl<type, underlying_type, bitsize>;                             \
                                                                                                            \
    public:                                                                                                 \
        using base_type::base_type;                                                                         \
    }

#define STRONG_FLAGS_DEFINE_FLAGS(name, underlying_type, ...)                                                \
namespace name{                                                                                             \
    STRONG_FLAGS_DEFINE_CLASS(underlying_type, STRONG_FLAGS_ARG_COUNT(__VA_ARGS__));\
\
    STRONG_FLAGS_MAKE_FLAGS(STRONG_FLAGS_ARG_COUNT(__VA_ARGS__), __VA_ARGS__);\
}\


#endif /* INCLUDE_STRONG_FLAGS_H_ */
