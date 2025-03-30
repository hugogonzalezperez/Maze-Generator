#ifndef WILSON_ALGORITHM_H
#define WILSON_ALGORITHM_H

#include "../utils.h"

class WilsonAlgorithm : public MazeGenerator {
 public:
  WilsonAlgorithm(Grid* grid, std::optional<unsigned int> seed = std::nullopt)
      : grid_(grid), gen_(seed.value_or(std::random_device{}())) {}

  void generate(int startX, int startY) override {
    std::vector<std::shared_ptr<Cell>> unvisitedCells = grid_->getGrid();

    std::shared_ptr<Cell> startCell = grid_->getCell(startX, startY);
    startCell->setStart(true);
    startCell->setVisited(true);

    // Lista de celdas no visitadas y eliminación de la celda inicial
    unvisitedCells.erase(std::remove(unvisitedCells.begin(), unvisitedCells.end(), startCell), unvisitedCells.end());

    // Caminatas aleatorias
    while (!unvisitedCells.empty()) {
      std::uniform_int_distribution<int> dist(0, unvisitedCells.size() - 1);
      std::shared_ptr<Cell> current = unvisitedCells[dist(gen_)];
      std::vector<std::shared_ptr<Cell>> path;
      std::unordered_map<std::shared_ptr<Cell>, std::shared_ptr<Cell>> pathMap;

      // Se marca como parte del camino
      path.push_back(current);
      pathMap[current] = nullptr;

      /* std::cout << path.size() << std::endl;
      std::cout << pathMap.size() << std::endl;
      for (std::shared_ptr<Cell> cell : path) {
        std::cout << *cell << std::endl;
      }

      for (auto& [cell, parent] : pathMap) {
        std::cout << cell << "  " << parent << std::endl;
      } */

      while (!current->isVisited()) {
        // Averiguar los vecinos no visitados
        std::vector<std::shared_ptr<Cell>> neighbors = grid_->getUnvisitedNeighbors(current);
        if (neighbors.empty()) break; // Si no hay vecinos disponibles se sale del bucle

        // Elegir un vecino aleatorio
        std::shuffle(neighbors.begin(), neighbors.end(), gen_);
        std::shared_ptr<Cell> next = neighbors.front();

        path.push_back(next);
        pathMap[next] = current;
        current = next;

        // Eliminar ciclos en la caminata
        auto it = std::find(path.begin(), path.end(), next);
        if (it != path.end()) {
          path.erase(it + 1, path.end());
        }

        for(std::shared_ptr<Cell> cell : path) { std::cout << cell << " "; }
        std::cout << std::endl;
      }

      // Conectar la caminata al árbol
      for (size_t i = 0; i < path.size() - 1; ++i) {
        grid_->removeWallBetween(path[i], path[i + 1]);
        path[i]->setVisited(true);
      }

      path.back()->setVisited(true);

      unvisitedCells.erase(std::remove_if(unvisitedCells.begin(), unvisitedCells.end(), [](std::shared_ptr<Cell> cell) { return cell->isVisited(); }), unvisitedCells.end());

      if (renderer_) {
        renderer_->update();
      }
    }
  }

  void setRenderer(MazeRenderer* renderer) {
    renderer_ = renderer;
  }

 private:
  Grid* grid_;
  std::mt19937 gen_;
  MazeRenderer* renderer_ = nullptr;
};

#endif  // WILSON_ALGORITHM_H