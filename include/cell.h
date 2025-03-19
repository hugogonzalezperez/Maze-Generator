#ifndef CELL_H
#define CELL_H

#include "./utils.h"

class Cell {
 public:
  Cell(int x, int y) : x_(x), y_(y), visited_(false), isStart_(false), isEnd_(false) {}

  // SETTERS AND GETTERS
  int getX() const { return x_; }
  int getY() const { return y_; }

  void setVisited(bool visited) { visited_ = visited; }
  bool isVisited() const { return visited_; }

  void setStart(bool isStart) { isStart_ = isStart; }
  bool isStart() const { return isStart_; }

  void setEnd(bool isEnd) { isEnd_ = isEnd; }
  bool isEnd() const { return isEnd_; }

  void setWall(int index) { walls_.set(index); }
  bool hasWall(int index) const { return walls_.test(index); }

  void removeWall(int wallIndex) {
    if (wallIndex >= 0 && wallIndex < 4) {
      walls_.reset(wallIndex);
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const Cell &cell) {
    os << "Cell(" << cell.x_ << ", " << cell.y_ << ") " << "Visited: " << cell.visited_;
    os << "\nCell walls (Top, Right, Bottom, Left): "
       << cell.walls_[0] << ", " << cell.walls_[1] << ", " << cell.walls_[2] << ", " << cell.walls_[3] << std::endl;
    return os;
  }

 private:
  int x_, y_;
  std::bitset<4> walls_ = 0b1111;  // top, right, bottom, left
  bool visited_;
  bool isStart_;
  bool isEnd_;
};

#endif  // CELL_H
