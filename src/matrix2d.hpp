#pragma once
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

namespace qustrolabe {
namespace cpp_matrix {

class ShapeMismatchException : public std::exception {
 public:
  ShapeMismatchException(const std::string& message) : message_(message) {}

  const char* what() const noexcept override { return message_.c_str(); }

 private:
  std::string message_;
};

template <typename SizeType>
struct Shape2D {
  SizeType rows;
  SizeType cols;

  auto operator<=>(const Shape2D&) const = default;
};

template <typename T>
class Matrix2D {
 public:
  using SizeType = int;

 public:
  Matrix2D(Shape2D<SizeType> shape, T init_value = {})
      : m_shape{shape}, m_data(shape.rows * shape.cols, init_value) {}
  explicit Matrix2D(SizeType rows, SizeType cols, T init_value = {})
      : Matrix2D(Shape2D{rows, cols}, init_value){};

  class Iterators {
   public:
    class MatrixRef {
     protected:
      MatrixRef(Matrix2D& matrix) : m_ref(matrix) {}
      Matrix2D& m_ref;
    };

    class InnerIterator : public MatrixRef {
     public:
      InnerIterator(SizeType row, SizeType col, bool direction,
                    Matrix2D& matrix)
          : m_row(row), m_col(col), m_direction(direction), MatrixRef(matrix) {}

      T& operator*() { return MatrixRef::m_ref.get(m_row, m_col); }

      InnerIterator& operator++() {
        if (m_direction) {
          ++m_col;
        } else {
          ++m_row;
        }
        return *this;
      }

      bool operator!=(InnerIterator& oth) const {
        return (m_row != oth.m_row) or (m_col != oth.m_col) or
               (m_direction != oth.m_direction) or
               (MatrixRef::m_ref != oth.m_ref);
      }

     private:
      SizeType m_row;
      SizeType m_col;
      bool m_direction;
    };

    class OuterIterator : public MatrixRef {
     public:
      OuterIterator(SizeType pos, bool direction, Matrix2D& ref)
          : m_pos(pos), m_direction(direction), MatrixRef(ref) {}

      auto& operator*() { return *this; }

      bool operator!=(OuterIterator& oth) const {
        return (m_pos != oth.m_pos) or (m_direction != oth.m_direction) or
               (MatrixRef::m_ref != oth.m_ref);
      }

      auto& operator++() {
        ++m_pos;
        return *this;
      }

      auto begin() {
        if (m_direction) {
          return InnerIterator(m_pos, 0, m_direction,
                               MatrixRef::m_ref);  // from Rows
        } else {
          return InnerIterator(0, m_pos, m_direction,
                               MatrixRef::m_ref);  // from Cols
        }
      }
      auto end() {
        if (m_direction) {
          return InnerIterator(m_pos, MatrixRef::m_ref.m_shape.cols,
                               m_direction,
                               MatrixRef::m_ref);  // from Rows
        } else {
          return InnerIterator(MatrixRef::m_ref.m_shape.rows, m_pos,
                               m_direction,
                               MatrixRef::m_ref);  // from Cols
        }
      }

     private:
      SizeType m_pos;
      bool m_direction; // TODO: try to move it to template
    };

    class Rows : public MatrixRef {
     public:
      Rows(Matrix2D& matrix) : MatrixRef(matrix) {}
      OuterIterator begin() { return OuterIterator(0, true, MatrixRef::m_ref); }
      OuterIterator end() {
        return OuterIterator(MatrixRef::m_ref.m_shape.rows, true,
                             MatrixRef::m_ref);
      }
    };

    class Cols : public MatrixRef {
     public:
      Cols(Matrix2D& matrix) : MatrixRef(matrix) {}

      OuterIterator begin() {
        return OuterIterator(0, false, MatrixRef::m_ref);
      }
      OuterIterator end() {
        return OuterIterator(MatrixRef::m_ref.m_shape.cols, false,
                             MatrixRef::m_ref);
      }
    };
  };  // Iterators

  Iterators::Cols getCols() { return Iterators::Cols(*this); }
  Iterators::Rows getRows() { return Iterators::Rows(*this); }

  const T& get(SizeType row, SizeType col) const {
    if (row < 0 or row >= m_shape.rows) throw std::out_of_range("Out of row");
    if (col < 0 or col >= m_shape.cols) throw std::out_of_range("Out of col");

    return m_data.at(row * m_shape.cols + col);
  }

  T& get(SizeType row, SizeType col) {
    if (row < 0 or row >= m_shape.rows) throw std::out_of_range("Out of row");
    if (col < 0 or col >= m_shape.cols) throw std::out_of_range("Out of col");

    return m_data.at(row * m_shape.cols + col);
  }

  bool operator==(const Matrix2D& other) const = default;

  auto rows() const { return m_shape.rows; }
  auto cols() const { return m_shape.cols; }
  auto shape() const { return m_shape; }

  auto& data() { return m_data; }
  //[1,2] operator

 private:
  Shape2D<SizeType> m_shape;
  std::vector<T> m_data;
};

template <typename T>
Matrix2D<T> Add(const Matrix2D<T>& lhs, const Matrix2D<T>& rhs) {
  using SizeType = Matrix2D<T>::SizeType;

  if (lhs.shape() != rhs.shape())
    throw ShapeMismatchException("Add(): Shape mismatch");

  auto result = Matrix2D<T>(lhs.shape());

  for (SizeType i = 0; i < lhs.rows(); i++) {
    for (SizeType j = 0; j < lhs.cols(); j++) {
      result.get(i, j) = lhs.get(i, j) + rhs.get(i, j);
    }
  }

  return result;
}

template <typename T>
Matrix2D<T> AddScalar(Matrix2D<T> matrix_copy, T scalar) {
  for (auto& e : matrix_copy.data()) {
    e += scalar;
  }

  return matrix_copy;
}

template <typename T>
Matrix2D<T> MultScalar(Matrix2D<T> matrix_copy, T scalar) {
  for (auto& e : matrix_copy.data()) {
    e *= scalar;
  }

  return matrix_copy;
}
template <typename T>
Matrix2D<T> Sub(Matrix2D<T> lhs, Matrix2D<T> rhs) {
  auto negative_rhs = MultScalar(rhs, -1);
  auto result = Add(lhs, negative_rhs);

  return result;
}

template <typename T>
Matrix2D<T> Transpose(const Matrix2D<T>& mat) {
  using SizeType = Matrix2D<T>::SizeType;
  auto mat_shape = mat.shape();
  Shape2D<SizeType> new_shape = {mat_shape.cols, mat_shape.rows};

  auto result = Matrix2D<T>(new_shape);

  for (SizeType row = 0; row < mat.rows(); row++) {
    for (SizeType col = 0; col < mat.cols(); col++) {
      result.get(col, row) = mat.get(row, col);
    }
  }

  return result;
}

template <typename T>
Matrix2D<T> DotProduct2D(const Matrix2D<T>& lhs, const Matrix2D<T>& rhs) {
  using SizeType = Matrix2D<T>::SizeType;
  auto lhs_shape = lhs.shape();
  auto rhs_shape = rhs.shape();

  if (lhs_shape.cols != rhs_shape.rows) {
    std::string message =
        std::format("ShapeMismatchException: {}x{} dot {}x{}", lhs_shape.rows,
                    lhs_shape.cols, rhs_shape.rows, rhs_shape.cols);
    throw ShapeMismatchException(message);
  }
  auto result_shape = Shape2D{lhs_shape.rows, rhs_shape.cols};
  auto result = Matrix2D<T>(result_shape);

  for (SizeType i = 0; i < result.rows(); i++) {
    for (SizeType j = 0; j < result.cols(); j++) {
      SizeType n = lhs.cols();
      T sum = 0;

      for (SizeType k = 0; k < n; k++) {
        sum += lhs.get(i, k) * rhs.get(k, j);
      }

      result.get(i, j) = sum;
    }
  }

  return result;
}

template <typename T, typename SizeType = typename Matrix2D<T>::SizeType>
Matrix2D<T> Rand2D(Shape2D<SizeType> shape) {
  std::random_device rd;
  std::minstd_rand engine(rd());
  std::uniform_int_distribution<int> distribution(1, 9);
  auto generate = [&engine, &distribution]() -> int {
    return distribution(engine);
  };

  auto result = Matrix2D<T>(shape);

  for (SizeType row = 0; row < result.rows(); row++) {
    for (SizeType col = 0; col < result.cols(); col++) {
      result.get(row, col) = generate() + 1;
    }
  }

  return result;
}

}  // namespace cpp_matrix
}  // namespace qustrolabe