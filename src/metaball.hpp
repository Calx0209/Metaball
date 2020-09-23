#pragma once

#include "grid.hpp"
#include "vector3.hpp"
#include <iostream>
#include <vector>

#include <omp.h>

struct Metaball
{
  Grid grid;
  std::vector<double> values;

  Metaball(Grid const &grid, Double3s const &particles, double const r) noexcept
      : grid{grid}, values(grid.size)
  {
    auto const r2 = r * r;
    #pragma omp parallel for
    for (std::size_t iz = 0; iz < grid.z.size; ++iz)
    {
      double const z = grid.z[iz];

      for (std::size_t iy = 0; iy < grid.y.size; ++iy)
      {
        double const y = grid.y[iy];

        for (std::size_t ix = 0; ix < grid.x.size; ++ix)
        {
          double const x = grid.x[ix];
          Double3 const q = {x, y, z};
          // std::cout << q << std::endl;
          auto &value = operator[]({ix, iy, iz});
          value = 0;

          for (auto p : particles)
          {
            auto const d = ((q - p) ^ 2).sum();
            // double xx = std::pow(q.x - p.x, 2.0);
            // double yy = std::pow(q.y - p.y, 2.0);
            // double zz = std::pow(q.z - p.z, 2.0);
            // auto d = xx+yy+zz;

            if (d == 0)
            {
              value += 1;
            }
            else
            {
              value += r2 / d;
            }
          }
          // if(value >= 1){
          //     std::cout << q.x << ", " << q.y << ", " << q.z << ", " << value << std::endl;
          // }
        }
      }
    }
  }

  double &operator[](Size3 const &i) noexcept
  {
    return values[i.x + i.y * grid.x.size + i.z * xy_size];
  }

  void print() noexcept
  {
    for (std::size_t iz = 0; iz < grid.z.size; ++iz)
    {
      for (std::size_t iy = 0; iy < grid.y.size; ++iy)
      {
        for (std::size_t ix = 0; ix < grid.x.size; ++ix)
        {
          std::cout << operator[]({ix, iy, iz}) << ", ";
        }
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

private:
  decltype(grid.x.size) xy_size = grid.x.size * grid.y.size;
};
