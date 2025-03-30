#ifndef DFS_ALGORITHM_H
#define DFS_ALGORITHM_H

#include "../utils.h"

class DFSAlgorithm : public MazeGenerator {
 public:
  DFSAlgorithm(Grid* grid, std::optional<unsigned int> seed = std::nullopt)
      : grid_(grid), gen_(seed.value_or(std::random_device{}())) {}

  void generate(int startX, int startY) override {
    grid_->setAlgorithmUsed("DFS");
    grid_->setSeed(gen_());

    std::shared_ptr<Cell> startCell = grid_->getCell(startX, startY);
    startCell->setVisited(true);
    stack_.push(startCell);

    while(!stack_.empty()) {
      std::shared_ptr<Cell> currentCell = stack_.top();
      stack_.pop();

      std::vector<std::shared_ptr<Cell>> neighbors = grid_->getUnvisitedNeighbors(currentCell);

      if(!neighbors.empty()) {
        std::shuffle(neighbors.begin(), neighbors.end(), gen_);
        stack_.push(currentCell);
        std::shared_ptr<Cell> neighbor = neighbors.front();
        grid_->removeWallBetween(currentCell, neighbor);
        neighbor->setVisited(true);
        stack_.push(neighbor);

        if(renderer_) {
          renderer_->update();  // CUIDADO QUE NO SE CIERRA LA VENTANA
        }
      }
    }

    grid_->CleanVisited();
  }

  void setRenderer(MazeRenderer* renderer) {
    renderer_ = renderer;
  }

 private:
  Grid* grid_;
  std::mt19937 gen_;
  std::stack<std::shared_ptr<Cell>> stack_;
  MazeRenderer* renderer_;
};

#endif  // DFS_ALGORITHM_H