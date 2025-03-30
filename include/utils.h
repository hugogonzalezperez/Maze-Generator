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

#endif  // UTILS_H