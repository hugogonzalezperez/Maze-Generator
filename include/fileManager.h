#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "./utils.h"

class FileManager {
 public:
  FileManager(Grid* grid = nullptr) {
    if (grid == nullptr) {
      throw std::runtime_error("Error: No se ha proporcionado un objeto Grid.");
    }

    grid_ = grid;
  }


  void setInputFile(const std::string& inputFile) {
    if (inputFile.empty()) {
      throw std::runtime_error("Error: No se ha proporcionado un fichero de entrada.");
    }

    if (!inputFile.ends_with(".lab")) {
      throw std::runtime_error("Error: El archivo de entrada debe tener extensión .lab");
    }

    if (!std::filesystem::exists(inputFile)) {
      throw std::runtime_error("Error: El archivo de entrada no existe.");
    }

    inputFile_ = inputFile;
  }

  void saveMaze() {
    // Crear la carpeta "test" si no existe
    std::string directory = "test";
    if (std::filesystem::create_directories(directory)) {
      std::cout << "Directorio creado: " << directory << std::endl;
    }

    // Crear el archivo de salida
    outputFile_ = "test/" + std::to_string(grid_->getWidth()) + "x" + std::to_string(grid_->getHeight()) + grid_->getAlgorithmUsed() + "_" + std::to_string(grid_->getSeed()) + ".lab";

    std::ofstream file(outputFile_);
    if (!file.is_open()) {
      throw std::runtime_error("Error: No se pudo abrir el archivo para escritura.");
    }

    // Escribir dimensiones
    file << grid_->getWidth() << "\n";
    file << grid_->getHeight() << "\n";

    // Escribir la representación matricial del laberinto
    for (int y = 0; y < grid_->getHeight(); ++y) {
      for (int x = 0; x < grid_->getWidth(); ++x) {
        file << grid_->getCell(x, y)->getWallNumber() << " ";
      }
      file << "\n";
    }

    file.close();
  }

  void loadMaze() {
    std::ifstream file(inputFile_);
    if (!file.is_open()) {
      throw std::runtime_error("Error: No se pudo abrir el archivo para lectura.");
    }

    int width, height;
    file >> width >> height;

    if (width <= 0 || height <= 0) {
      throw std::runtime_error("Error: Dimensiones inválidas en el archivo.");
    }

    // Redimensionar el grid si las dimensiones no coinciden
    if (grid_->getWidth() != width || grid_->getHeight() != height) {
      grid_->resize(width, height);
    }

    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        int wallNumber;
        file >> wallNumber;
        grid_->getCell(x, y)->setWalls(wallNumber);
      }
    }

    file.close();
  }

 private:
  std::string inputFile_;
  std::string outputFile_;
  Grid* grid_;
};

#endif  // FILE_MANAGER_H
