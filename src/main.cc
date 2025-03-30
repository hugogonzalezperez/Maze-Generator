#include <chrono>
#include <iostream>
#include <optional>

#include "../include/utils.h"

void generateMaze(Grid& grid, MazeRenderer& renderer, std::optional<unsigned int> seed, bool visualize) {
  DFSAlgorithm dfs(&grid, seed);
  dfs.setRenderer(visualize ? &renderer : nullptr);
  auto start = std::chrono::high_resolution_clock::now();
  dfs.generate(0, 0);
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "Tiempo de ejecución Generación: " << std::chrono::duration<double>(end - start).count() << " segundos" << std::endl;
}

int main(int argc, char* argv[]) {
  try {
    auto options = parseArguments(argc, argv);
    Grid grid(20, 20);
    MazeRenderer renderer(&grid, 800, 800);
    if (!renderer.init()) {
      std::cerr << "Failed to initialize renderer" << std::endl;
      return -1;
    }

    FileManager fileManager(&grid);

    if (options.loadFromFile) {
      fileManager.setInputFile(options.inputFile);
      fileManager.loadMaze();
      std::cout << "Maze loaded from file: " << options.inputFile << std::endl;
    } else {
      generateMaze(grid, renderer, options.seed, options.visualize);
    }

    if (options.seed) {
      grid.setSeed(options.seed.value());
      std::cout << "Seed set to: " << options.seed.value() << std::endl;
    }

    if (options.saveToFile) {
      fileManager.saveMaze();
      std::cout << "Maze saved to file." << std::endl;
    }

    bool running = true;
    SDL_Event event;
    while (running) {
      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
      }
      renderer.update();
    }
    renderer.cleanup();

  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  return 0;
}