# Maze Generator

Maze Generator is an application that allows generating, visualizing, and solving mazes using different algorithms. The project includes support for loading mazes from files, generating new mazes with specific seeds, and saving generated mazes for later use.

## Features

- Maze generation using the Depth-First Search (DFS) algorithm.
- Maze visualization with SDL2.
- Loading mazes from `.lab` format files.
- Saving generated mazes to files.
- Support for setting specific seeds for reproducible maze generation.

---

## Technologies Used

- **C++20**: Main programming language.
- **SDL2**: Library for graphical maze visualization.
- **CMake**: Build system for compiling the project.
- **STL**: Extensive use of C++ Standard Library (e.g., `std::vector`, `std::optional`, etc.).

---

## Installation

### Prerequisites

Ensure the following packages are installed on your system:

1. **SDL2**:
   - On Debian/Ubuntu-based systems:
     ```bash
     sudo apt update
     sudo apt install libsdl2-dev
     ```
   - On Fedora-based systems:
     ```bash
     sudo dnf install SDL2-devel
     ```

2. **C++20-compatible compiler**:
   - GCC 10 or later:
     ```bash
     sudo apt install g++
     ```
   - Or Clang 10 or later:
     ```bash
     sudo apt install clang
     ```

3. **CMake**:
   - On Debian/Ubuntu-based systems:
     ```bash
     sudo apt install cmake
     ```

---

## Compilation

1. Clone this repository:
   ```bash
   git clone https://github.com/your-username/maze-generator.git
   cd maze-generator
   ```

2. Compile the project using the provided Makefile:
   ```bash
   make
   ```

3. The executable will be generated in the `bin` directory.

---

## Execution Options

The program supports the following command-line options:

- `-f [file]`: Loads a maze from a `.lab` file. If no file is specified, the generated maze is saved to a new file.
- `-s <seed>`: Sets a specific seed for maze generation.
- `-v`: Enables visualization of the generated or loaded maze.

### Usage Examples

- Load a maze from a file:
  ```bash
  ./bin/maze -f maze.lab
  ```
- Generate a maze and save it to a new file:
  ```bash
  ./bin/maze -f
  ```
- Generate a maze with a specific seed and save it:
  ```bash
  ./bin/maze -f -s 12345
  ```
- Visualize a generated maze:
  ```bash
  ./bin/maze -v
  ```

---

## `.lab` File Format

`.lab` files follow this structure:

- First line: Maze width.
- Second line: Maze height.
- Remaining lines: Matrix representation of the maze, where each cell contains a number representing the walls.

Example:
```
10
10
11 9 5 3 9 1 5 5 5 3
10 10 11 10 10 14 9 5 3 14
12 6 8 6 8 5 4 7 12 3
...
```

---

## Project Description

Maze Generator is an educational and practical tool for exploring maze generation and solving algorithms. It uses the DFS algorithm to generate mazes and allows real-time visualization through SDL2. The project also supports saving and loading mazes, making it ideal for reproducible experiments and maze storage.

