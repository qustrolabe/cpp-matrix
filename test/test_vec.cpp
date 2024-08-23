
#include <catch2/catch_test_macros.hpp>

#include "cpp_matrix.hpp"
using namespace qustrolabe;

TEST_CASE("Vec2Int initialization", "[Vec]") {
  using cpp_matrix::Vec;
  using cpp_matrix::Vec2Int;

  SECTION("List-initialization") {
    auto vec = Vec2Int({4, 2});

    REQUIRE(vec.get<0>() == 4);
    REQUIRE(vec.get<1>() == 2);
  }

  SECTION("Variadic args initialization") {
    auto vec = Vec2Int(5, 3);

    REQUIRE(vec.get<0>() == 5);
    REQUIRE(vec.get<1>() == 3);
  }
}

TEST_CASE("Vec2Int copying", "[Vec]") {
  using cpp_matrix::Vec;
  using cpp_matrix::Vec2Int;

  auto vec = Vec2Int({10, 72});

  auto vec_copy = vec;

  REQUIRE(vec_copy.get<0>() == 10);
  REQUIRE(vec_copy.get<1>() == 72);
}

TEST_CASE("Vec2Int component assignment", "[Vec]") {
  using cpp_matrix::Vec2Int;

  auto vec = Vec2Int({1, 2});

  vec.get<0>() *= 6;
  vec.get<1>() *= 6;

  REQUIRE(vec.get<0>() == 6);
  REQUIRE(vec.get<1>() == 12);
}

TEST_CASE("Vec2Int component get") {
  using cpp_matrix::Vec2Int;

  auto vec = Vec2Int(5, 7);

  auto& ref = vec.get<0>();
  const auto& const_ref = vec.get<1>();

  ref *= const_ref; // 5 * 7

  REQUIRE(vec.get<0>() == 35);
}