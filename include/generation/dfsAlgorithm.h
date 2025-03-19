#ifndef DFS_ALGORITHM_H
#define DFS_ALGORITHM_H

#include "../utils.h"

class DFSAlgorithm : public MazeGenerator {
 public:
  DFSAlgorithm(Grid* grid, std::optional<unsigned int> seed = std::nullopt)
      : grid_(grid), gen_(seed.value_or(std::random_device{}())) {}

  void generate(int startX, int startY) override {
    Cell* startCell = grid_->getCell(startX, startY);
    startCell->setStart(true);
    startCell->setVisited(true);
    stack_.push(startCell);

    while(!stack_.empty()) {
      Cell* currentCell = stack_.top();
      stack_.pop();

      std::vector<Cell*> neighbors = grid_->getUnvisitedNeighbors(currentCell);

      if(!neighbors.empty()) {
        std::shuffle(neighbors.begin(), neighbors.end(), gen_);
        stack_.push(currentCell);
        Cell* neighbor = neighbors.front();
        grid_->removeWallBetween(currentCell, neighbor);
        neighbor->setVisited(true);
        stack_.push(neighbor);

        if(renderer_) {
          renderer_->update();  // CUIDADO QUE NO SE CIERRA LA VENTANA
        }
      }
    }
  }

  void setRenderer(MazeRenderer* renderer) {
    renderer_ = renderer;
  }

 private:
  Grid* grid_;
  std::stack<Cell*> stack_;
  std::mt19937 gen_;
  MazeRenderer* renderer_;
};

#endif  // DFS_ALGORITHM_H