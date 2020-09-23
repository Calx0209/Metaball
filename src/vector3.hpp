#pragma once

#include <vector>
#include <cmath>

template <class T>
struct Vector3
{
  T x;
  T y;
  T z;

  Vector3 operator-(Vector3 const &lhs) const noexcept
  {
    return {x - lhs.x, y - lhs.y, z - lhs.z};
  }

  Vector3 operator^(int const lhs) const noexcept
  {
    return {std::pow(x, lhs), std::pow(y, lhs), std::pow(z, lhs)};
  }

  bool operator==(Vector3 const &lhs) const noexcept
  {
    return x == lhs.x && y == lhs.y && z == lhs.z;
  }

  T sum() const noexcept
  {
    return x + y + z;
  }
};

using Int3 = Vector3<int>;
using Int3s = std::vector<Int3>;
using Double3 = Vector3<double>;
using Double3s = std::vector<Double3>;
using Size3 = Vector3<std::size_t>;

template <class T>
auto &operator<<(std::ostream &stream, Vector3<T> const &vector3) noexcept
{
  stream << '<' << vector3.x << ", " << vector3.y << ", " << vector3.z << '>';
  return stream;
}

template <class T>
auto &operator<<(std::ostream &stream, std::vector<Vector3<T>> const &vector3s) noexcept
{
  stream << vector3s.size() << ",\n";

  for (auto const &vector3 : vector3s)
  {
    stream << vector3 << ",\n";
  }

  return stream;
}
