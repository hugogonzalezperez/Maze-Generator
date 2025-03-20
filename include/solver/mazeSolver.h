#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include "./utils.h"

class MazeSolver {
 public:
  virtual void solve(int startX, int startY, int EndX, int EndY) = 0;  // Método virtual puro
  virtual ~MazeSolver() = default;                 // Destructor virtual
};

#endif  // MAZE_SOLVER_H