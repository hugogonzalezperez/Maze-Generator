#ifndef A_STAR_H
#define A_STAR_H

#include "../utils.h"

struct Node {
  std::shared_ptr<Cell> cell;
  double f;  // f = g + h
};

class AStarAlgorithm : public MazeSolver {
 public:
  AStarAlgorithm(Grid* grid) : grid_(grid) {}

  void solve() override {
    auto start = grid_->getStart();
    auto end = grid_->getEnd();

    if (!start || !end) {
      throw std::runtime_error("Error: Start or end cell is null.");
    }

    // Priority queue para A* (min-heap)
    auto compare = [](const Node& a, const Node& b) { return a.f > b.f; };
    std::priority_queue<Node, std::vector<Node>, decltype(compare)> openSet(compare);

    // Mapas para almacenar costos y padres
    std::unordered_map<std::shared_ptr<Cell>, double> gScore;
    std::unordered_map<std::shared_ptr<Cell>, std::shared_ptr<Cell>> cameFrom;

    // Inicializar
    gScore[start] = 0;
    openSet.push({start, heuristic(start, end)});

    while (!openSet.empty()) {
      auto current = openSet.top().cell;
      openSet.pop();

      if (current == end) {
        reconstructPath(cameFrom, end);
        return;
      }

      for (auto neighbor : grid_->getUnvisitedNeighbors(current)) {
        if (grid_->hasWallBetween(current, neighbor)) {
          continue;  // Si hay una pared, no se puede mover al vecino
        }

        double tentative_gScore = gScore[current] + 1;  // Asumiendo costo uniforme

        if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor]) {
          cameFrom[neighbor] = current;
          gScore[neighbor] = tentative_gScore;
          double fScore = tentative_gScore + heuristic(neighbor, end);
          openSet.push({neighbor, fScore});
        }
      }

      current->setVisited(true);
      if (renderer_) {
        renderer_->update();
      }
    }
    throw std::runtime_error("Error: No se encontró un camino.");
  }

  void setRenderer(MazeRenderer* renderer) {
    renderer_ = renderer;
  }

 private:
  Grid* grid_;
  MazeRenderer* renderer_;

  double heuristic(std::shared_ptr<Cell> a, std::shared_ptr<Cell> b) {
    int dx = a->getX() - b->getX();
    int dy = a->getY() - b->getY();
    return std::sqrt(dx * dx + dy * dy);
  }

  // Reconstruir el camino desde el mapa `cameFrom`
  void reconstructPath(std::unordered_map<std::shared_ptr<Cell>, std::shared_ptr<Cell>>& cameFrom,
                       std::shared_ptr<Cell> current) {
    std::vector<std::shared_ptr<Cell>> path;
    while (cameFrom.find(current) != cameFrom.end()) {
      path.push_back(current);
      current = cameFrom[current];
    }
    path.push_back(current);  // Agregar el nodo inicial
    std::reverse(path.begin(), path.end());

    grid_->CleanVisited();
    grid_->SetPath(path);
    if (renderer_) renderer_->update();
  }
};

#endif  // A_STAR_H