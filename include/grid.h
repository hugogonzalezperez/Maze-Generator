#ifndef GRID_H
#define GRID_H

#include "./utils.h"

class Grid {
 public:
  Grid(int cols, int rows)
      : cols_(cols), rows_(rows) {
    try {
      grid_.reserve(cols_ * rows_ + cols_);
      for (int y = 0; y < rows_; ++y) {
        for (int x = 0; x < cols_; ++x) {
          grid_.push_back(new Cell(x, y));
        }
      }
    } catch (const std::bad_alloc& e) {
      throw std::runtime_error("Error: No se pudo asignar memoria para el grid.");
    }
  }

  ~Grid() {
    try {
      for (Cell* cell : grid_) {
        delete cell;
      }
      grid_.clear();
    } catch (...) {
      // Evita que el destructor propague excepciones
    }
  }

  void setStart(int x, int y) {
    if (!isValidIndex(x, y)) {
      throw std::out_of_range("Error: Índices fuera de rango en setStart.");
    }

    start_ = getCell(x, y);
    start_->setStart(true);
  }

  inline Cell* getStart() const { return start_; }

  void setEnd(int x, int y) {
    if (!isValidIndex(x, y)) {
      throw std::out_of_range("Error: Índices fuera de rango en setEnd.");
    }

    end_ = getCell(x, y);
    end_->setEnd(true);
  }

  inline Cell* getEnd() const { return end_; }

  inline int getWidth() const { return cols_; }

  inline int getHeight() const { return rows_; }

  inline const std::vector<Cell*>& getGrid() const { return grid_; }

  Cell* getCell(int x, int y) {
    if (!isValidIndex(x, y)) {
      throw std::out_of_range("Error: Índices fuera de rango en getCell.");
    }
    return grid_[index(x, y)];
  }

  void removeWallBetween(int x1, int y1, int x2, int y2) {
    if (!isValidIndex(x1, y1) || !isValidIndex(x2, y2)) {
      throw std::out_of_range("Error: Índices fuera de rango en removeWallBetween.");
    }

    removeWallBetween(getCell(x1, y1), getCell(x2, y2));
  }

  void removeWallBetween(Cell* cell1, Cell* cell2) {
    if (!cell1 || !cell2) {
      throw std::runtime_error("Error: Intento de acceder a celdas nulas en removeWallBetween.");
    }

    int dx = cell2->getX() - cell1->getX();
    int dy = cell2->getY() - cell1->getY();

    if (dx == 1 && dy == 0) {  // Derecha
      cell1->removeWall(1);
      cell2->removeWall(3);
    } else if (dx == -1 && dy == 0) {  // Izquierda
      cell1->removeWall(3);
      cell2->removeWall(1);
    } else if (dx == 0 && dy == 1) {  // Abajo
      cell1->removeWall(2);
      cell2->removeWall(0);
    } else if (dx == 0 && dy == -1) {  // Arriba
      cell1->removeWall(0);
      cell2->removeWall(2);
    }
  }

  void CleanVisited() {
    for (Cell* cell : grid_) {
      cell->setVisited(false);
    }
  }

  void SetPath(std::vector<Cell*> path) {
    for (Cell* cell : path) {
      cell->setPath(true);
    }
  }

  Cell* AnyHasNoWalls() {
    for (Cell* cell : grid_) {
      if (!cell->hasWall(0) && !cell->hasWall(1) && !cell->hasWall(2) && !cell->hasWall(3)) {
        return cell;
      }
    }
    std::cout << "No cell has no walls." << std::endl;
    return nullptr;
  }

  std::vector<Cell*> getUnvisitedNeighbors(Cell* cell) {
    if (!cell) {
      throw std::runtime_error("Error: Puntero nulo en getUnvisitedNeighbors.");
    }

    std::vector<Cell*> neighbors;
    int x = cell->getX();
    int y = cell->getY();

    struct Direction {
      int dx, dy;
    };

    static const Direction directions[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (const auto& dir : directions) {
      int nx = x + dir.dx;
      int ny = y + dir.dy;

      if (isValidIndex(nx, ny)) {
        Cell* neighbor = getCell(nx, ny);
        if (!neighbor->isVisited()) {
          neighbors.push_back(neighbor);
        }
      }
    }

    return neighbors;
  }

  friend std::ostream& operator<<(std::ostream& os, const Grid& grid) {
    os << "Grid(" << grid.cols_ << ", " << grid.rows_ << ")";
    return os;
  }

 private:
  int index(int x, int y) const {
    return y * cols_ + x;
  }

  bool isValidIndex(int x, int y) const {
    return x >= 0 && x < cols_ && y >= 0 && y < rows_;
  }

  std::vector<Cell*> grid_;
  int cols_, rows_;
  Cell* start_;
  Cell* end_;
};

#endif  // GRID_H
