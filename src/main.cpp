#include "grid.hpp"
#include "marching_cube.hpp"
#include "metaball.hpp"
#include "particles.hpp"
#include "povray.hpp"

#include <chrono>

int main()
{
  std::chrono::system_clock::time_point start,end;
  start = std::chrono::system_clock::now();

  Grid const grid{
      {0, 3, 160},
      {0, 3, 160},
      {0, 3, 160}};
  Particles const grid_particles{grid};

  // Grid const fluid{
  //     {1, 2, 2},
  //     {1, 2, 2},
  //     {1, 2, 2}};
  // Particles const fluid_particles{fluid};
  // Metaball metaball{grid, fluid_particles.vertices, 0.40};
  // MarchingCube marching_cube{metaball};
  // Povray povray{"pov/test.pov"};
  // // povray.spheres(grid_particles.vertices, 0.02, "Gray");
  // povray.spheres(fluid_particles.vertices, 0.03, "Orange");
  // povray.metaball(metaball, 1, 0.01, "Red");
  // povray.mesh(marching_cube.mesh);
  
  Particles const inputFile{"../output00000.prof"};
  Metaball metaball{grid, inputFile.vertices, 0.007};
  MarchingCube marching_cube{metaball};
  Povray povray{"pov/test.pov"};
  // povray.spheres(grid_particles.vertices, 0.01, "Gray");
  povray.spheres(inputFile.vertices, 0.0125, "Orange");
  // povray.metaball(metaball, 1, 0.01, "Red");
  povray.mesh(marching_cube.mesh);

  end = std::chrono::system_clock::now();
  auto elapsed = std::chrono::duration_cast< std::chrono::minutes >(end - start).count();
  std::cout << elapsed << "Min" << std::endl;
  return 0;
}
