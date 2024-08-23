#pragma once

namespace qustrolabe {
namespace cpp_matrix {

#include <cmath>
#include <tuple>

template <typename... Components>
struct Vec;

template <typename... Components>
struct Vec<std::tuple<Components...>> {
  std::tuple<Components...> components;

  Vec() : components() {}

  Vec(Components... args) : components(args...) {}

  template <std::size_t Index>
  decltype(auto) get(this auto& self) {
    return std::get<Index>(self.components);
  }

  bool operator==(const Vec& other) const {
    return components == other.components;
  }

  bool operator!=(const Vec& other) const { return !(*this == other); }
};

using Vec2 = Vec<std::tuple<double, double>>;
using Vec3 = Vec<std::tuple<double, double, double>>;
using Vec4 = Vec<std::tuple<double, double, double, double>>;

using Vec2Int = Vec<std::tuple<int, int>>;
using Vec3Int = Vec<std::tuple<int, int, int>>;
using Vec4Int = Vec<std::tuple<int, int, int, int>>;

}  // namespace cpp_matrix
}  // namespace qustrolabe