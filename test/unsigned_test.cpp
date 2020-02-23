#include "catch2/catch.hpp"
#include "strong_flags/strong_flags.hpp"
#include <type_traits>

STRONG_FLAGS_DEFINE_FLAGS(Type1, unsigned int, Flag0, Flag1, Flag2);

TEST_CASE("single_flag", "[int-based]") {
    auto t1 = Type1::Flag0;

    REQUIRE(t1.test_any(Type1::Flag0) == true);
    REQUIRE(t1.test_all(Type1::Flag0) == true);

    REQUIRE(t1.test_any(Type1::Flag1) == false);

    REQUIRE(t1.to_underlying_type() == 0x1U);

    REQUIRE(t1 == Type1::Flag0);

    REQUIRE(t1.test(Type1::Flag0_bit) == true);
    REQUIRE(t1.test(Type1::Flag2_bit) == false);

    const auto t2(t1);

    REQUIRE(t2 == t1);
    REQUIRE(Type1::Flag0 != Type1::Flag2);
}

TEST_CASE("Multiple_flags", "[int-based]") {
    auto t1 = Type1::Flag0 | Type1::Flag2;

    REQUIRE(t1.test_any(Type1::Flag0) == true);
    REQUIRE(t1.test_any(Type1::Flag1) == false);
    REQUIRE(t1.test_any(Type1::Flag2) == true);

    REQUIRE(t1.test_all(Type1::Flag0 | Type1::Flag1) == false);
    REQUIRE(t1.test_all(Type1::Flag0 | Type1::Flag2) == true);

    Type1::type t2;
    REQUIRE(static_cast<unsigned int>(t2) == 0U);

    t2 = t1;
    REQUIRE(t2 == t1);
    REQUIRE(t2.to_underlying_type() == 0b101);
}

TEST_CASE("Mutators_single", "[int-based]") {
    auto t1 = Type1::from_bit(Type1::Flag1_bit);

    t1.set(Type1::Flag1_bit);
    REQUIRE(t1 == Type1::Flag1);
    t1.set(Type1::Flag0_bit);
    REQUIRE(t1 == (Type1::Flag0 | Type1::Flag1));

    t1.clear(Type1::Flag2_bit);
    REQUIRE(t1 == (Type1::Flag0 | Type1::Flag1));
    t1.clear(Type1::Flag1_bit);
    REQUIRE(t1 == Type1::Flag0);

    t1.toggle(Type1::Flag2_bit);
    REQUIRE(t1 == (Type1::Flag0 | Type1::Flag2));
    t1.toggle(Type1::Flag0_bit);
    REQUIRE(t1 == Type1::Flag2);
}

TEST_CASE("Mutators_multiple", "[int-based]") {
    auto t1 = Type1::from_underlying_type(0b010);
    auto t2(t1);
    REQUIRE(t1 == Type1::Flag1);


    t1.set(Type1::Flag0 | Type1::Flag1);
    t2 |= Type1::Flag0 | Type1::Flag1;
    REQUIRE(t1 == t2);
    REQUIRE(t1 == (Type1::Flag0 | Type1::Flag1));

    t1.clear(Type1::Flag1 | Type1::Flag2);
    t2 &= ~(Type1::Flag1 | Type1::Flag2);

    REQUIRE(t1 == t2);
    REQUIRE(t1 == Type1::Flag0);

    t1.toggle(Type1::Flag0 | Type1::Flag2);
    t2 ^= Type1::Flag0 | Type1::Flag2;

    REQUIRE(t1 == t2);
    REQUIRE(t1 == Type1::Flag2);
}

TEST_CASE("type_traits", "[int-based]") {
    REQUIRE(std::is_trivially_copyable<Type1::type>::value == true);
    REQUIRE(std::is_standard_layout<Type1::type>::value == true);
    REQUIRE(sizeof(Type1::type) == sizeof(Type1::type::underlying_type));
}

TEST_CASE("constexpr", "[int-based]") {
    constexpr Type1::type empty;
    constexpr auto t1 = Type1::Flag0;
    constexpr auto t2 = Type1::from_underlying_type(5U);
    constexpr auto t3 = Type1::from_bit(1);

    constexpr auto n1 = t1.to_underlying_type();
    constexpr auto n2 = static_cast<Type1::type::underlying_type>(t2);

    constexpr bool b1 = t1 == t2;
    constexpr bool b2 = t1 != t3;
    constexpr bool b3 = t1.test(0);
    constexpr bool b4 = t1.test_any(t2);
    constexpr bool b5 = t1.test_all(t3);

    constexpr auto t4 = (t1 & t2) | (t3 ^ ~empty);
}



