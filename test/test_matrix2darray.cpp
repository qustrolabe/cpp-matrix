
#include <catch2/catch_test_macros.hpp>

#include "cpp_matrix.hpp"
using namespace qustrolabe;

TEST_CASE("Matrix2DArray init", "[matrix2darray]") {
  using cpp_matrix::Matrix2DArray;
  using SizeType = Matrix2DArray<int>::SizeType;

  SECTION("Default init") {
    auto matrix = Matrix2DArray<int, 10, 10>();

    for (std::size_t row = 0; row < 10; row++) {
      for (std::size_t col = 0; col < 10; col++) {
        REQUIRE(matrix.get(row, col) == 0);
      }
    }
  }

  SECTION("Init value") {
    auto matrix = Matrix2DArray<int, 10, 10>(42);

    for (std::size_t row = 0; row < 10; row++) {
      for (std::size_t col = 0; col < 10; col++) {
        REQUIRE(matrix.get(row, col) == 42);
      }
    }
  }
}