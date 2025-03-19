#include <chrono>
#include <iostream>

#include "../include/utils.h"

int main() {
  int cols = 10;
  int rows = 10;
  Grid grid(cols, rows);
  MazeRenderer renderer(&grid, 800, 800);

  if (!renderer.init()) {
    std::cerr << "Failed to initialize renderer" << std::endl;
    return -1;
  }

  DFSAlgorithm dfs(&grid);
  dfs.setRenderer(&renderer);

  int startX = 5, startY = 5;
  grid.getCell(9, 9)->setEnd(true);
  dfs.generate(startX, startY);

  bool running = true;
  SDL_Event event;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }
    renderer.update();
  }

  renderer.cleanup();
  return 0;
}