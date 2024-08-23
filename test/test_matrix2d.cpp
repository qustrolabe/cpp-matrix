
#include <catch2/catch_test_macros.hpp>

#include "cpp_matrix.hpp"
using namespace qustrolabe;

TEST_CASE("Initialization", "[matrix2d]") {
  using cpp_matrix::Matrix2D;

  REQUIRE(Matrix2D<int>(10, 10) == Matrix2D<int>({10, 10}));

  SECTION("copying") {
    auto matrix1 = Matrix2D<int>({2, 2});
    matrix1.get(1, 1) = 42;

    auto matrix2 = matrix1;

    REQUIRE(matrix1.get(1, 1) == matrix2.get(1, 1));
  }

  SECTION("moving") {
    auto matrix1 = Matrix2D<int>({2, 2});
    matrix1.get(1, 1) = 42;

    auto matrix2 = std::move(matrix1);

    REQUIRE(matrix2.get(1, 1) == 42);
  }
}

TEST_CASE("Basic data iterator", "[matrix2d]") {
  using cpp_matrix::Matrix2D;
  using cpp_matrix::Rand2D;

  auto matrix = Rand2D<int>({4, 7});
  auto matrix_copy = matrix;

  auto copy_iterator = matrix_copy.data().begin();
  for (const auto& e : matrix.data()) {
    REQUIRE(e == *(copy_iterator++));
  }
}

TEST_CASE("Special iterators", "[matrix2d]") {
  using cpp_matrix::Matrix2D;
  using SizeType = Matrix2D<int>::SizeType;
  using cpp_matrix::Rand2D;

  SECTION("range-based and manual iterator") {
    auto matrix = Rand2D<int>({4, 7});
    auto matrix_copy = matrix;

    SECTION("iterating over rows") {
      auto copy_rows = matrix_copy.getRows().begin();

      for (auto& row : matrix.getRows()) {
        auto copy_row = copy_rows.begin();
        ++copy_rows;

        for (auto& e : row) {
          REQUIRE(e == *(copy_row));
          ++copy_row;
        }
      }
    }

    SECTION("iterating over cols") {
      auto copy_cols = matrix_copy.getCols().begin();

      for (auto& col : matrix.getCols()) {
        auto copy_col = copy_cols.begin();
        ++copy_cols;

        for (auto& e : col) {
          REQUIRE(e == *(copy_col));
          ++copy_col;
        }
      }
    }
  }

  SECTION("manual for loop") {
    auto matrix = Rand2D<int>({3, 2});

    SECTION("over rows") {
      std::vector<int> result1{};
      std::vector<int> result2{};
      result1.reserve(matrix.data().size());
      result2.reserve(matrix.data().size());

      for (SizeType row = 0; row < matrix.rows(); row++) {
        for (SizeType col = 0; col < matrix.cols(); col++) {
          result1.push_back(matrix.get(row, col));
        }
      }

      for (auto& row : matrix.getRows()) {
        for (auto& e : row) {
          result2.push_back(e);
        }
      }

      REQUIRE(result1 == result2);
    }

    SECTION("over cols") {
      std::vector<int> result1{};
      std::vector<int> result2{};
      result1.reserve(matrix.data().size());
      result2.reserve(matrix.data().size());

      for (SizeType col = 0; col < matrix.cols(); col++) {
        for (SizeType row = 0; row < matrix.rows(); row++) {
          result1.push_back(matrix.get(row, col));
        }
      }

      for (auto& col : matrix.getCols()) {
        for (auto& e : col) {
          result2.push_back(e);
        }
      }

      REQUIRE(result1 == result2);
    }
  }
}

TEST_CASE("Basic matrix addition", "[matrix2d]") {
  using cpp_matrix::Add;
  using cpp_matrix::Matrix2D;
  using SizeType = Matrix2D<int>::SizeType;

  Matrix2D<int> m1 = Matrix2D<int>(2, 2);
  Matrix2D<int> m2 = Matrix2D<int>({2, 2});

  {
    int counter = 0;
    for (SizeType i = 0; i < m1.rows(); i++) {
      for (SizeType j = 0; j < m1.cols(); j++) {
        m1.get(i, j) = counter++;
      }
    }

    for (SizeType i = 0; i < m2.rows(); i++) {
      for (SizeType j = 0; j < m2.cols(); j++) {
        m2.get(i, j) = counter++;
      }
    }
  }

  // 0 1 | 4 5 | 4 6
  // 2 3 | 6 7 | 8 10

  Matrix2D<int> addition_result = Add(m1, m2);

  REQUIRE(addition_result.get(0, 0) == 4);
  REQUIRE(addition_result.get(0, 1) == 6);
  REQUIRE(addition_result.get(1, 0) == 8);
  REQUIRE(addition_result.get(1, 1) == 10);
}

TEST_CASE("Randomized addition", "[matrix2d]") {
  using cpp_matrix::Matrix2D;
  using SizeType = Matrix2D<int>::SizeType;

  using cpp_matrix::Add;
  using cpp_matrix::Rand2D;

  Matrix2D<int> m1 = Rand2D<int>({5, 5});
  Matrix2D<int> m2 = Rand2D<int>({5, 5});

  Matrix2D<int> addition_result = Add(m1, m2);

  for (SizeType i = 0; i < addition_result.rows(); i++) {
    for (SizeType j = 0; j < addition_result.cols(); j++) {
      CHECK(addition_result.get(i, j) == (m1.get(i, j) + m2.get(i, j)));
    }
  }
}

TEST_CASE("Scalar integer operations", "[matrix2d]") {
  using cpp_matrix::AddScalar;
  using cpp_matrix::Matrix2D;
  using cpp_matrix::MultScalar;
  const int n1 = 5;
  const int n2 = 7;

  auto matrix = Matrix2D<int>({10, 10});
  {
    INFO("addition");
    matrix = AddScalar(matrix, n1);
    for (const auto& e : matrix.data()) {
      REQUIRE(e == n1);
    }
  }

  {
    INFO("multiplication");
    matrix = MultScalar(matrix, n2);
    for (const auto& e : matrix.data()) {
      REQUIRE(e == (n1 * n2));
    }
  }

  {
    INFO("multiply by 0");
    matrix = MultScalar(matrix, 0);

    auto zero_matrix = Matrix2D<int>(matrix.shape());

    REQUIRE(matrix == zero_matrix);
  }
}

TEST_CASE("Subtraction", "[matrix2d]") {
  using cpp_matrix::Matrix2D;
  using cpp_matrix::Rand2D;
  using cpp_matrix::Sub;

  using SizeType = Matrix2D<int>::SizeType;

  auto matrix1 = Matrix2D<int>({10, 10});
  auto matrix2 = Matrix2D<int>({10, 10});

  matrix1 = AddScalar(matrix1, 8);
  matrix2 = AddScalar(matrix2, 5);

  auto matrix3 = Sub(matrix1, matrix2);

  for (const auto& e : matrix3.data()) {
    REQUIRE(e == 3);
  }

  SECTION("randomized subtraction") {
    auto matrix1 = Rand2D<int>({10, 5});
    auto matrix2 = Rand2D<int>({10, 5});
    auto matrix3 = Sub(matrix1, matrix2);

    for (SizeType row = 0; row < matrix3.rows(); row++) {
      for (SizeType col = 0; col < matrix3.cols(); col++) {
        REQUIRE(matrix3.get(row, col) ==
                matrix1.get(row, col) - matrix2.get(row, col));
      }
    }
  }
  SECTION("randomized subtracting from itself") {
    auto matrix1 = Rand2D<int>({10, 5});
    auto matrix2 = Sub(matrix1, matrix1);

    for (const auto& e : matrix2.data()) {
      REQUIRE(e == 0);
    }
  }
}

TEST_CASE("Transpose", "[matrix2d]") {
  using cpp_matrix::Matrix2D;
  using cpp_matrix::Rand2D;
  using cpp_matrix::Transpose;

  using SizeType = Matrix2D<int>::SizeType;

  auto matrix = Matrix2D<int>({2, 10});
  auto transposed_matrix = Transpose(matrix);

  auto matrix_shape = matrix.shape();
  auto transposed_matrix_shape = transposed_matrix.shape();

  REQUIRE(matrix_shape.rows == transposed_matrix_shape.cols);
  REQUIRE(matrix_shape.cols == transposed_matrix_shape.rows);

  for (SizeType row = 0; row < matrix.rows(); row++) {
    for (SizeType col = 0; col < matrix.cols(); col++) {
      REQUIRE(matrix.get(row, col) == transposed_matrix.get(col, row));
    }
  }
}

TEST_CASE("Equality operator", "[matrix2d]") {
  using cpp_matrix::Matrix2D;
  using cpp_matrix::Rand2D;

  SECTION("determined") {
    auto matrix1 = Matrix2D<int>({5, 7});
    auto matrix2 = Matrix2D<int>({5, 7});

    matrix1.get(3, 3) = 42;
    matrix2.get(3, 3) = 42;

    REQUIRE(matrix1 == matrix2);

    REQUIRE(matrix1 == matrix1);
    REQUIRE(matrix2 == matrix2);
    REQUIRE(matrix2 == matrix1);
  }

  SECTION("randomized") {
    auto matrix1 = Rand2D<int>({15, 7});
    auto matrix2 = matrix1;

    matrix1.get(3, 3) = 42;
    matrix2.get(3, 3) = 42;

    REQUIRE(matrix1 == matrix2);

    REQUIRE(matrix1 == matrix1);
    REQUIRE(matrix2 == matrix2);
    REQUIRE(matrix2 == matrix1);
  }
}

TEST_CASE("Dot product", "[matrix2d]") {
  using cpp_matrix::DotProduct2D;
  using cpp_matrix::Matrix2D;
  using cpp_matrix::Rand2D;

  SECTION("shape assertion") {
    auto matrix1 = Rand2D<int>({2, 10});
    auto matrix2 = Rand2D<int>({10, 5});

    auto result = DotProduct2D(matrix1, matrix2);

    auto m1_shape = matrix1.shape();
    auto m2_shape = matrix2.shape();
    auto result_shape = result.shape();

    REQUIRE(result_shape.rows == m1_shape.rows);
    REQUIRE(result_shape.cols == m2_shape.cols);
  }

  SECTION("2x2 result assertion") {
    auto matrix1 = Rand2D<int>({2, 2});
    auto matrix2 = Rand2D<int>({2, 2});

    matrix1.get(0, 0) = 1;
    matrix1.get(0, 1) = 2;
    matrix1.get(1, 0) = 3;
    matrix1.get(1, 1) = 4;

    matrix2.get(0, 0) = 5;
    matrix2.get(0, 1) = 6;
    matrix2.get(1, 0) = 7;
    matrix2.get(1, 1) = 8;

    auto result = DotProduct2D(matrix1, matrix2);

    REQUIRE(result.get(0, 0) == 19);
    REQUIRE(result.get(0, 1) == 22);
    REQUIRE(result.get(1, 0) == 43);
    REQUIRE(result.get(1, 1) == 50);
  }
}