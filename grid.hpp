#pragma once

#include "axis.hpp"
#include <iostream>

struct Grid
{
  Axis x;
  Axis y;
  Axis z;
  std::size_t size;

  Grid(Axis const &x, Axis const &y, Axis const &z) noexcept
    : x{x}, y{y}, z{z}, size{x.size * y.size * z.size}
  {
  }

  void print() const noexcept
  {
    std::cout << size << ",\n";

    for (auto iy : y)
    {
      for (auto ix : x)
      {
        for(auto iz : z){
          std::cout << '<' << ix << ", " << iy << ", " << iz << ">, ";
        }
      }
      std::cout << std::endl;
    }
  }
};
