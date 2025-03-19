#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

#include "./utils.h"

class MazeGenerator {
 public:
  virtual void generate(int startX, int startY) = 0;  // Método virtual puro
  virtual ~MazeGenerator() = default;  // Destructor virtual
};

#endif  // MAZE_GENERATOR_H
