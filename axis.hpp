#pragma once

#include <iostream>
#include <vector>

struct Axis
{
  std::size_t size;

  Axis(double const start, double const end, int const n) noexcept
  {
    auto const d = (end - start) / (n - 1);

    for (int i = 0; i < n; ++i)
    {
      data.push_back(start + d*i);
    }
    
    size = data.size();
  }

  auto begin() const noexcept
  {
    return data.begin();
  }

  auto end() const noexcept
  {
    return data.end();
  }

  double operator[](std::size_t const i) const noexcept
  {
    return data[i];
  }

  void print() const noexcept
  {
    std::cout << size << ",\n";

    for (auto v : data)
    {
      std::cout << v << ", ";
    }

    std::cout << std::endl;
  }

private:
  std::vector<double> data;
};
