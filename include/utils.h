#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>

#include <algorithm>
#include <array>
#include <bitset>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <queue>

#include <random>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "./cell.h"
#include "./grid.h"
#include "./fileManager.h"
#include "./renderer.h"
#include "./generation/mazeGenerator.h"
#include "./generation/dfsAlgorithm.h"
#include "./generation/wilsonAlgorithm.h"
#include "./solver/mazeSolver.h"
#include "./solver/wallFollow.h"
#include "./solver/aStar.h"

// Estructura para almacenar las opciones del programa
struct ProgramOptions {
  bool loadFromFile = false;
  bool saveToFile = false;
  bool visualize = false;
  std::optional<unsigned int> seed = std::nullopt;
  std::string inputFile = "";
};

// Función para procesar los argumentos de la línea de comandos
ProgramOptions parseArguments(int argc, char* argv[]) {
  ProgramOptions options;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "-f") {
      if (i + 1 < argc && argv[i + 1][0] != '-') {
        options.inputFile = argv[++i];
        options.loadFromFile = true;
      } else {
        options.saveToFile = true;
      }
    } else if (arg == "-s" && i + 1 < argc) {
      try {
        options.seed = std::stoul(argv[++i]);
      } catch (const std::exception&) {
        throw std::invalid_argument("Error: La semilla debe ser un número entero positivo.");
      }
    } else if (arg == "-v") {
      options.visualize = true;
    } else {
      throw std::invalid_argument("Uso: ./maze [-f [file]] [-s <seed>]");
    }
  }
  if (options.loadFromFile && options.seed) {
    throw std::invalid_argument("Error: No se puede usar -f con un archivo y -s al mismo tiempo.");
  }
  return options;
}

void selectStartAndEnd(Grid& grid, MazeRenderer& renderer) {
  bool selecting = true;
  bool startSelected = false;
  bool endSelected = false;
  SDL_Event event;

  std::cout << "Haz clic izquierdo para seleccionar la celda de inicio." << std::endl;
  std::cout << "Haz clic derecho para seleccionar la celda de final." << std::endl;

  while (selecting) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        selecting = false;
        exit(0);  // Salir del programa si se cierra la ventana
      } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        // Calcular la celda seleccionada
        int cellX = mouseX / renderer.getCellWidth();
        int cellY = mouseY / renderer.getCellHeight();

        if (grid.getCell(cellX, cellY)) {
          if (event.button.button == SDL_BUTTON_LEFT && !startSelected) {
            // Click izquierdo: marcar como inicio
            grid.setStart(cellX, cellY);
            startSelected = true;
            std::cout << "Celda marcada como inicio: (" << cellX << ", " << cellY << ")" << std::endl;
          } else if (event.button.button == SDL_BUTTON_RIGHT && !endSelected) {
            // Click derecho: marcar como final
            grid.setEnd(cellX, cellY);
            endSelected = true;
            std::cout << "Celda marcada como final: (" << cellX << ", " << cellY << ")" << std::endl;
          }
        }

        // Salir del bucle si ambas celdas han sido seleccionadas
        if (startSelected && endSelected) {
          selecting = false;
        }
      }
    }

    renderer.update();  // Actualizar la visualización del laberinto
  }
}

void generateMaze(Grid& grid, MazeRenderer& renderer, std::optional<unsigned int> seed, bool visualize) {
  DFSAlgorithm dfs(&grid, seed);
  dfs.setRenderer(visualize ? &renderer : nullptr);
  auto start = std::chrono::high_resolution_clock::now();
  dfs.generate(0, 0);
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "Tiempo de ejecución Generación: " << std::chrono::duration<double>(end - start).count() << " segundos" << std::endl;
}

#endif  // UTILS_H