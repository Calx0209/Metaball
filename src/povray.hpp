#pragma once

#include "mesh.hpp"
#include "metaball.hpp"
#include <string>
#include <fstream>

struct Povray
{
  Povray(std::string const &file_name) noexcept : pov_file{file_name}
  {
    pov_file <<
        R"/(#include "colors.inc"
#include "textures.inc"
#include "glass.inc"

camera {
  perspective
  location <0.5, 0.5, -2>
  look_at<0.5, 0.5, 0>
  // location <10, 10, -5>
  // look_at<5.5, 5.5, 5.5>
}

light_source {
  <-1, 1, 0>
  color <1, 1, 1>
  parallel
}

sky_sphere{
  pigment{
      gradient y
      color_map{
        [0.0 color rgb<1.0, 1.0, 1.0>*0.5]
        [0.2 color rgb<1.0, 1.0, 1.0>*0.2]
      }
  }
}

plane{ y, 0.0
  texture {
    pigment{ checker color White color Gray70 scale 0.2 }
  }

}

)/";
  }

  ~Povray() noexcept
  {
    pov_file << std::endl;
  }

  void spheres(Double3s const &points, double const r, std::string const &color) noexcept
  {
    for (auto p : points)
    {
      pov_file << "sphere {\n"
              << p << ", " << r << R"/(
                texture {
                  Water
                  pigment { )/" << color
              << R"/(
                          }
                        }
                  }

              )/";
    }
  }

  void metaball(Metaball &metaball, double const t, double const &r, std::string const &color) noexcept
  {
    for (std::size_t iz = 0; iz < metaball.grid.z.size; ++iz)
    {
      double z = metaball.grid.z[iz];

      for (std::size_t iy = 0; iy < metaball.grid.y.size; ++iy)
      {
        double y = metaball.grid.y[iy];

        for (std::size_t ix = 0; ix < metaball.grid.x.size; ++ix)
        {
          double x = metaball.grid.x[ix];

          if (metaball[{ix, iy, iz}] > t)
          {
            pov_file << "sphere {\n"
                     << "  <" << x << ", " << y << ", " << z << ">, " << r << R"/(
                        texture {
                          Water
                          pigment {
                          )/" << color << R"/(
                          }
                        }
                      }
                    )/";
          }
        }
      }
    }
  }

  void mesh(Mesh const &mesh) noexcept
  {
    pov_file << mesh;
  }

private:
  std::ofstream pov_file;
};
