#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "strong_flags/strong_flags.hpp"

STRONG_FLAGS_DEFINE_FLAGS(Type1, unsigned int, Flag0, Flag1);

TEST_CASE("instanciation", "[base]") {
    auto t1 = Type1::Flag0;
}





