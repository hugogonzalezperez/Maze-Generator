#include <chrono>
#include <iostream>

#include "../include/utils.h"

// TODO: REVISAR LA IMPLEMENTACIÓN DE WILSON Y QUE SE PUEDA CERRAR LA VENTANA EN MITAD DE LA GENERACIÓN

int main() {
  int cols = 10;
  int rows = 10;
  Grid grid(cols, rows);
  MazeRenderer renderer(&grid, 800, 800);

  if (!renderer.init()) {
    std::cerr << "Failed to initialize renderer" << std::endl;
    return -1;
  }

  auto start = std::chrono::high_resolution_clock::now();

  DFSAlgorithm dfs(&grid);
  dfs.setRenderer(&renderer);

  dfs.generate(0, 0);

  WallFollow wallFollow(&grid);
  wallFollow.setRenderer(&renderer);

  wallFollow.solve(0, 0, cols - 1, rows - 1);

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Tiempo de ejecución: " << elapsed.count() << " segundos" << std::endl;

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