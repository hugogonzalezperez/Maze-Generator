#ifndef WALL_FOLLOW_H
#define WALL_FOLLOW_H

#include "../utils.h"

class WallFollow : public MazeSolver {
 public:
  WallFollow(Grid* grid) : grid_(grid) {}

  void solve(int startX, int startY, int endX, int endY) override {
    std::shared_ptr<Cell> current = grid_->getCell(startX, startY);
    current->setStart(true);
    std::shared_ptr<Cell> goal = grid_->getCell(endX, endY);
    goal->setEnd(true);

    path_.push_back(current);

    // Direcciones: 0 = Arriba, 1 = Derecha, 2 = Abajo, 3 = Izquierda
    int direction = 1;  // Empezamos moviéndonos a la derecha

    while (current != goal) {
      int rightDir = (direction + 1) % 4;
      int forwardDir = direction;

      if (!current->hasWall(rightDir)) {
        direction = rightDir;
      } else if (current->hasWall(forwardDir)) {
        direction = (direction + 3) % 4;  // Gira a la izquierda si no puede avanzar
        continue;
      }

      int dx = (direction == 1) - (direction == 3);
      int dy = (direction == 2) - (direction == 0);

      std::shared_ptr<Cell> next = grid_->getCell(current->getX() + dx, current->getY() + dy);

      if (next && !current->hasWall(direction)) {
        current = next;
        path_.push_back(current);
        current->setVisited(true);
        if (renderer_) renderer_->update();
      }
    }
    grid_->CleanVisited();
    grid_->SetPath(path_);
    if (renderer_) renderer_->update();
  }

  void setRenderer(MazeRenderer* renderer) {
    renderer_ = renderer;
  }

 private:
  std::shared_ptr<Cell> move(std::shared_ptr<Cell> cell, int dir) {
    int x = cell->getX();
    int y = cell->getY();
    switch (dir) {
      case 0:
        return grid_->getCell(x, y - 1);  // Arriba
      case 1:
        return grid_->getCell(x + 1, y);  // Derecha
      case 2:
        return grid_->getCell(x, y + 1);  // Abajo
      case 3:
        return grid_->getCell(x - 1, y);  // Izquierda
    }
    return cell;
  }

  Grid* grid_;
  std::mt19937 gen_;
  MazeRenderer* renderer_;
  std::vector<std::shared_ptr<Cell>> path_;
};

#endif  // WALL_FOLLOW_H