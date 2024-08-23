#pragma once
#include <array>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

namespace qustrolabe {
namespace cpp_matrix {

template <typename T, std::size_t ROWS = 0, std::size_t COLS = 0>
class Matrix2DArray {
 public:
  using SizeType = std::size_t;
  Matrix2DArray(T init_value = {}) : m_data{} {
    for (auto& row : m_data) {
      row.fill(init_value);
    }
  }

  [[nodiscard]] constexpr T& get(SizeType row, SizeType col) {
    return m_data[row][col];
  }

 private:
  std::array<std::array<T, COLS>, ROWS> m_data;
};
}  // namespace cpp_matrix
}  // namespace qustrolabe