#ifndef MAZE_RENDERER_H
#define MAZE_RENDERER_H

#include "./utils.h"

class MazeRenderer {
 public:
  MazeRenderer(Grid* grid, int height, int width) : grid_(grid), height_(height), width_(width), window_(nullptr), renderer_(nullptr) {
    cellHeight_ = height_ / grid_->getHeight();
    cellWidth_ = width_ / grid_->getWidth();
  }

  ~MazeRenderer() { cleanup(); }

  Grid* getGrid() const { return grid_; }
  int getWidth() const { return width_; }
  int getHeight() const { return height_; }
  int getCellHeight() const { return cellHeight_; }
  int getCellWidth() const { return cellWidth_; }
  SDL_Window* getWindow() const { return window_; }
  SDL_Renderer* getRenderer() const { return renderer_; }

  void setGrid(Grid* grid) { grid_ = grid; }
  void setWidth(int width) { width_ = width; }
  void setHeight(int height) { height_ = height; }
  void setCellHeight(int height) { cellHeight_ = height; }
  void setCellWidth(int width) { cellWidth_ = width; }
  void setWindow(SDL_Window* window) { window_ = window; }
  void setRenderer(SDL_Renderer* renderer) { renderer_ = renderer; }

  void resizeCells(int height, int width) {
    cellHeight_ = height / grid_->getHeight();
    cellWidth_ = width / grid_->getWidth();
  }

  bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
      return false;
    }

    window_ = SDL_CreateWindow("Maze Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               width_, height_, SDL_WINDOW_SHOWN);
    if (!window_) {
      std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
      return false;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_) {
      std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
      return false;
    }
    return true;
  }

  void render() {
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
    SDL_RenderClear(renderer_);

    for (std::shared_ptr<Cell> cell : grid_->getGrid()) {
      drawCell(cell);
      drawWalls(cell);
    }

    SDL_RenderPresent(renderer_);
  }

  void update() {
    render();
    SDL_Delay(50);
  }

  void cleanup() {
    if (renderer_) {
      SDL_DestroyRenderer(renderer_);
      renderer_ = nullptr;
    }
    if (window_) {
      SDL_DestroyWindow(window_);
      window_ = nullptr;
    }
    SDL_Quit();
  }

 private:
  void drawCell(std::shared_ptr<Cell> cell) {
    SDL_Rect rect = {cell->getX() * cellWidth_, cell->getY() * cellHeight_, cellWidth_, cellHeight_};

    if (cell->isVisited()) {
      SDL_SetRenderDrawColor(renderer_, 255, 136, 0, 255);  // Orange
    } else if (cell->isStart()) {
      SDL_SetRenderDrawColor(renderer_, 0, 255, 0, 255);  // Green
    } else if (cell->isEnd()) {
      SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);  // Red
    } else if (cell->isPath()) {
      SDL_SetRenderDrawColor(renderer_, 0, 0, 255, 255);  // Blue
    } else {
      return;  // No need to draw if none of the conditions are met
    }

    SDL_RenderFillRect(renderer_, &rect);
  }

  void drawWalls(std::shared_ptr<Cell> cell) {
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    int x = cell->getX() * cellWidth_;
    int y = cell->getY() * cellHeight_;

    if (cell->hasWall(0)) SDL_RenderDrawLine(renderer_, x, y, x + cellWidth_, y);
    if (cell->hasWall(1)) SDL_RenderDrawLine(renderer_, x + cellWidth_, y, x + cellWidth_, y + cellHeight_);
    if (cell->hasWall(2)) SDL_RenderDrawLine(renderer_, x, y + cellHeight_, x + cellWidth_, y + cellHeight_);
    if (cell->hasWall(3)) SDL_RenderDrawLine(renderer_, x, y, x, y + cellHeight_);
  }

  Grid* grid_;
  int cellHeight_;
  int cellWidth_;
  int width_;
  int height_;
  SDL_Window* window_;
  SDL_Renderer* renderer_;
};

#endif  // MAZE_RENDERER_H