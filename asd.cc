void generate(int startX, int startY) override {
  // Marcar la celda inicial como parte del laberinto
  Cell* startCell = grid_->getCell(startX, startY);
  startCell->setStart(true);
  startCell->setVisited(true);

  // Lista de celdas no visitadas
  std::vector<Cell*> unvisitedCells = grid_->getGrid();
  unvisitedCells.erase(std::remove_if(unvisitedCells.begin(), unvisitedCells.end(),
                                      [](Cell* cell) { return cell->isVisited(); }),
                       unvisitedCells.end());

  std::cout << "Grid cells: " << grid_->getGrid().size() << std::endl;
  std::cout << "Unvisited cells: " << unvisitedCells.size() << std::endl;

  std::uniform_int_distribution<> dist(0, unvisitedCells.size() - 1);
  Cell* currentCell = unvisitedCells[dist(gen_)];

  unvisitedCells.erase(std::remove_if(unvisitedCells.begin(), unvisitedCells.end(),
                                      [](Cell* cell) { return cell->isVisited(); }),
                       unvisitedCells.end());

  std::cout << "Grid cells: " << grid_->getGrid().size() << std::endl;
  std::cout << "Unvisited cells: " << unvisitedCells.size() << std::endl;
  while (!unvisitedCells.empty()) {
    // Elegir una celda aleatoria de las no visitadas
    std::uniform_int_distribution<> dist(0, unvisitedCells.size() - 1);
    Cell* currentCell = unvisitedCells[dist(gen_)];

    // Realizar una caminata aleatoria hasta alcanzar una celda visitada
    std::vector<Cell*> path;
    std::unordered_map<Cell*, Cell*> visitedDuringWalk;
    while (!currentCell->isVisited()) {
      path.push_back(currentCell);

      // Elegir un vecino aleatorio
      std::vector<Cell*> neighbors = grid_->getUnvisitedNeighbors(currentCell);
      if (neighbors.empty()) {
        break;  // No hay vecinos disponibles
      }
      std::shuffle(neighbors.begin(), neighbors.end(), gen_);
      Cell* nextCell = neighbors.front();

      // Eliminar ciclos en la caminata
      if (visitedDuringWalk.count(nextCell)) {
        auto it = std::find(path.begin(), path.end(), nextCell);
        path.erase(it, path.end());
      }

      visitedDuringWalk[currentCell] = nextCell;
      currentCell = nextCell;
    }

    // Marcar el camino como parte del laberinto
    for (size_t i = 0; i < path.size() - 1; ++i) {
      grid_->removeWallBetween(path[i], path[i + 1]);
      path[i]->setVisited(true);
    }
    if (!path.empty()) {
      path.back()->setVisited(true);
    }

    // Actualizar la lista de celdas no visitadas
    unvisitedCells.erase(std::remove_if(unvisitedCells.begin(), unvisitedCells.end(),
                                        [](Cell* cell) { return cell->isVisited(); }),
                         unvisitedCells.end());

    // Actualizar el renderizador si está configurado
    if (renderer_) {
      renderer_->update();
    }
  }
}