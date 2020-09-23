#pragma once

#include "grid.hpp"
#include "vector3.hpp"
#include <iostream>
#include <fstream>
#include <sstream>


struct Particles
{
  Double3s vertices;

  Particles(Grid const &grid) noexcept
  {
    Double3 p;

    for (auto z : grid.z)
    {
      p.z = z;

      for (auto y : grid.y)
      {
        p.y = y;

        for (auto x : grid.x)
        {
          p.x = x;
          vertices.push_back(p);
        }
      }
    }
  }

  Particles(char const *const file_name) noexcept
  {
    Double3 p;
    std::string readingLine;
    std::ifstream ifs(file_name);

    if(!ifs.is_open())
    {
      std::cout << "failed to open!" << std::endl;
    }else
    {
      std::getline(ifs, readingLine);
      // std::getline(ifs, readingLine);
      double tmpValue;
      double tmpArray[9];
      while (!ifs.eof())
      {
        std::stringstream separateText;
        std::getline(ifs, readingLine);
        separateText << readingLine;
        for (unsigned int i = 0; i < sizeof(tmpArray)/sizeof(*tmpArray); i++)
        {
          separateText >> tmpValue;
          tmpArray[i] = tmpValue;
        }

        if(tmpArray[0] == 0){
          p.x = tmpArray[1];
          p.y = tmpArray[2];
          p.z = tmpArray[3];
          vertices.push_back(p);
        }
      }
    }
  }
};
