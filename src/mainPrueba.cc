#include <chrono>
#include <iostream>

#include "../include/utils.h"

// TODO: REVISAR LA IMPLEMENTACIÓN DE WILSON Y QUE SE PUEDA CERRAR LA VENTANA EN MITAD DE LA GENERACIÓN

int main() {
  std::vector<Cell*> sizes;

  for (int y = 0; y < 5; ++y) {
    for (int x = 0; x < 5; ++x) {
      sizes.push_back(new Cell(x, y));
    }
  }

  Cell* initial = sizes[0];
  initial->setVisited(true);
  
  std::cout << "Sizes: " << sizes.size() << std::endl;

  auto start = std::chrono::high_resolution_clock::now();

  sizes.erase(std::remove_if(sizes.begin(), sizes.end(), [](Cell* cell) { return cell->isVisited(); }), sizes.end());

  //sizes.erase(std::remove(sizes.begin(), sizes.end(), initial), sizes.end());

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Tiempo de ejecución: " << elapsed.count() << " segundos" << std::endl;

  std::cout << "Sizes: " << sizes.size() << std::endl;

  return 0;
}