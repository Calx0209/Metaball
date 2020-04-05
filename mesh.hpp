#pragma once

#include "vector2.hpp"
#include "vector3.hpp"
#include <algorithm>
#include <ostream>

struct Mesh
{
  // Double2s vertices;
  Double3s vertices;
  Int3s indices;

  void add_tri(Double3 const &v1, Double3 const &v2, Double3 const &v3) noexcept
  {
    auto const i1 = add_vertex(v1);
    auto const i2 = add_vertex(v2);
    auto const i3 = add_vertex(v3);
    indices.push_back({i1, i2, i3});
  }

  void add_rect(Double3 const &v1, Double3 const &v2, Double3 const &v3, Double3 const &v4) noexcept
  {
    add_tri(v1, v2, v3);
    add_tri(v1, v3, v4);
  }

private:
  int add_vertex(Double3 const &v) noexcept
  {
    auto const begin = vertices.begin();
    auto const end = vertices.end();
    auto const found = std::find(begin, end, v);

    if (found == end)
    {
      vertices.push_back(v);
      return vertices.size() - 1;
    }
    
    return found - begin;
  }
};

auto& operator<<(std::ostream& stream, Mesh const &mesh) noexcept
{
  stream <<
R"/(mesh2 {
  vertex_vectors {
)/" << mesh.vertices <<
R"/(  }

face_indices {
)/" << mesh.indices <<
R"/(  }

  // texture {
  //   Water
  //   pigment {
  //     SkyBlue
  //   }
  // }
  material { texture { pigment { Col_Glass_Dark_Green } finish{ F_Glass1 } } interior { ior 1.5 } }
})/";

  return stream;
}
