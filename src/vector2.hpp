#pragma once

#include <cmath>
#include <ostream>
#include <vector>

template <class T>
struct Vector2
{
  T x;
  T y;

  Vector2 operator-(Vector2 const &lhs) const noexcept
  {
    return {x - lhs.x, y - lhs.y};
  }

  Vector2 operator^(int const lhs) const noexcept
  {
    return {std::pow(x, lhs), std::pow(y, lhs)};
  }

  bool operator==(Vector2 const &lhs) const noexcept
  {
    return x == lhs.x && y == lhs.y;
  }

  T sum() const noexcept
  {
    return x + y;
  }
};

using Double2 = Vector2<double>;
using Double2s = std::vector<Double2>;
using Size2 = Vector2<std::size_t>;

template <class T>
auto& operator<<(std::ostream& stream, Vector2<T> const &vector2) noexcept
{
  stream << '<' << vector2.x << ", " << vector2.y << ", " << 0 << '>';
  return stream;
}

template <class T>
auto& operator<<(std::ostream& stream, std::vector<Vector2<T>> const &vector2s) noexcept
{
  stream << vector2s.size() << ",\n";

  for (auto const &vector2 : vector2s)
  {
    stream << vector2 << ",\n";
  }

  return stream;
}
