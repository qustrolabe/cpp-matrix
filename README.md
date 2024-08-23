# C++23 matrix library
***Created for educational purposes-only***

## Example usage
```cpp
#include <print>
#include "cpp_matrix.hpp"

int main() {
  using namespace qustrolabe::cpp_matrix;

  auto print_matrix = [](auto& m) {
    for (auto& row : m.getRows()) {
      for (auto& e : row) {
        std::print("{} ", e);
      }
      std::println();
    }
    std::println();
  };

  auto m1 = Matrix2D<int>({3, 4});
  auto m2 = Rand2D<int>({3, 4});
  auto m3 = Rand2D<int>({2, 4});

  print_matrix(m1);
  print_matrix(m2);
  print_matrix(m3);

  m1 = Add(m1, m2);
  m3 = Transpose(m3);

  auto result = DotProduct2D(m1, m3);

  print_matrix(result);
}
```
 
 ## Building

 ```bash
conan install . --output-folder build --build=missing --settings=build_type=Debug  
```

```bash
cd build
```

```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"
```

```bash
cmake --build .
 ```

### Requirements
-- C++23