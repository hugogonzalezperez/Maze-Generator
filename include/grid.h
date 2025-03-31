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
          grid_.push_back(std::make_shared<Cell>(x, y));
        }
      }
    } catch (const std::bad_alloc& e) {
      throw std::runtime_error("Error: No se pudo asignar memoria para el grid.");
    }
  }

  ~Grid() {
    try {
      grid_.clear();
    } catch (...) {
      // Evita que el destructor propague excepciones
    }
  }

  void resize(int newCols, int newRows) {
    if (newCols <= 0 || newRows <= 0) {
      throw std::invalid_argument("Error: Las dimensiones deben ser mayores que 0.");
    }

    cols_ = newCols;
    rows_ = newRows;

    // Limpiar el grid actual
    grid_.clear();

    // Reconstruir el grid con las nuevas dimensiones
    grid_.reserve(cols_ * rows_);
    for (int y = 0; y < rows_; ++y) {
      for (int x = 0; x < cols_; ++x) {
        grid_.push_back(std::make_shared<Cell>(x, y));
      }
    }
  }

  void setStart(int x, int y) {
    if (!isValidIndex(x, y)) {
      throw std::out_of_range("Error: Índices fuera de rango en setStart.");
    }

    start_ = getCell(x, y);
    start_->setStart(true);
  }

  inline std::shared_ptr<Cell> getStart() const { return start_; }

  void setEnd(int x, int y) {
    if (!isValidIndex(x, y)) {
      throw std::out_of_range("Error: Índices fuera de rango en setEnd.");
    }

    end_ = getCell(x, y);
    end_->setEnd(true);
  }

  inline std::shared_ptr<Cell> getEnd() const { return end_; }

  void setAlgorithmUsed(std::string algorithmUsed) {
    algorithmUsed_ = algorithmUsed;
  }

  inline std::string getAlgorithmUsed() const { return algorithmUsed_; }

  void setSeed(int seed) {
    seed_ = seed;
  }

  inline int getSeed() const { return seed_; }

  inline int getWidth() const { return cols_; }

  inline int getHeight() const { return rows_; }

  inline const std::vector<std::shared_ptr<Cell>>& getGrid() const { return grid_; }

  std::shared_ptr<Cell> getCell(int x, int y) {
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

  void removeWallBetween(std::shared_ptr<Cell> cell1, std::shared_ptr<Cell> cell2) {
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
    for (std::shared_ptr<Cell> cell : grid_) {
      cell->setVisited(false);
    }
  }

  void SetPath(std::vector<std::shared_ptr<Cell>> path) {
    for (std::shared_ptr<Cell> cell : path) {
      cell->setPath(true);
    }
  }

  std::shared_ptr<Cell> AnyHasNoWalls() {
    for (std::shared_ptr<Cell> cell : grid_) {
      if (!cell->hasWall(0) && !cell->hasWall(1) && !cell->hasWall(2) && !cell->hasWall(3)) {
        return cell;
      }
    }
    std::cout << "No cell has no walls." << std::endl;
    return nullptr;
  }

  std::vector<std::shared_ptr<Cell>> getUnvisitedNeighbors(std::shared_ptr<Cell> cell) {
    if (!cell) {
      throw std::runtime_error("Error: Puntero nulo en getUnvisitedNeighbors.");
    }

    std::vector<std::shared_ptr<Cell>> neighbors;
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
        std::shared_ptr<Cell> neighbor = getCell(nx, ny);
        if (!neighbor->isVisited()) {
          neighbors.push_back(neighbor);
        }
      }
    }

    return neighbors;
  }

  bool hasWallBetween(std::shared_ptr<Cell> cell1, std::shared_ptr<Cell> cell2) {
    if (!cell1 || !cell2) {
      throw std::runtime_error("Error: Intento de acceder a celdas nulas en hasWallBetween.");
    }

    int dx = cell2->getX() - cell1->getX();
    int dy = cell2->getY() - cell1->getY();

    switch (dx * 10 + dy) {  // Combinar dx y dy en un solo número para el switch
      case 10:               // Derecha
        return cell1->hasWall(1) || cell2->hasWall(3);
      case -10:  // Izquierda
        return cell1->hasWall(3) || cell2->hasWall(1);
      case 1:  // Abajo
        return cell1->hasWall(2) || cell2->hasWall(0);
      case -1:  // Arriba
        return cell1->hasWall(0) || cell2->hasWall(2);
      default:
        return true;  // No son vecinos directos, hay pared.
    }
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

  std::vector<std::shared_ptr<Cell>> grid_;
  int cols_, rows_;
  std::shared_ptr<Cell> start_;
  std::shared_ptr<Cell> end_;
  std::string algorithmUsed_;
  int seed_;
};

#endif  // GRID_H
