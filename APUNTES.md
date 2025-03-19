## OBTENCION DE LAS CELDAS

```
#include "../include/utils.h"

int main() {
  // Initialize the grid
  Grid mazeGrid(10, 10);  // Example size, can be adjusted

  Cell* Cell1 = mazeGrid.getCell(2, 2);
  Cell* Cell2 = mazeGrid.getCell(2, 3);
  
  std::cout << "Cell1: " << *Cell1 << std::endl;
  Cell1->printWalls();
  std::cout << "Cell2: " << *Cell2 << std::endl;
  Cell2->printWalls();
  
  mazeGrid.removeWallBetween(Cell1, Cell2);
  Cell1->setVisited(true);

  Cell* Cell3 = mazeGrid.getCell(2, 2);

  std::cout << "Cell3: " << *Cell3 << std::endl;
  Cell3->printWalls();

  std::cout << "Cell1: " << *Cell1 << std::endl;
  Cell1->printWalls();

  std::cout << sizeof(Cell1) << std::endl;
  std::cout << sizeof(*Cell1) << std::endl;

  /* // Initialize the renderer
  Renderer renderer(&mazeGrid);
  renderer.render();  // Initial render of the maze */

  std::cout << "Maze generation and solving completed." << std::endl;
  return 0;
}
```

Y la salida es esta:
➜  prueba ./bin/maze
Cell1: Cell(2, 2) Visited: 0
Cell walls (Top, Right, Bottom, Left): 1, 1, 1, 1
Cell2: Cell(2, 3) Visited: 0
Cell walls (Top, Right, Bottom, Left): 1, 1, 1, 1
Cell3: Cell(2, 2) Visited: 1
Cell walls (Top, Right, Bottom, Left): 1, 1, 0, 1
Cell1: Cell(2, 2) Visited: 1
Cell walls (Top, Right, Bottom, Left): 1, 1, 0, 1
8
8
24
Maze generation and solving completed.
➜  prueba 

Como puedes ver, el puntero Cell1 y Cell3 apuntan al mismo objeto el cual en uno la pared de abajo existe y en otro momento no. Como se puede ver, almacenar el objeto como puntero es lo más idoneo para ahorrar memoria ya que solo ocupa 8 bytes, en vez de 24 que ocupa el objeto Cell. Una vez traducida la dirección a objeto, si pesa 24 ya que copias el objeto en memoria, pero no se puede hacer más.


94430568986080 0x55e2548535e0
94430568986400 0x55e254853720

Ahora mismo el DFS Algorithm esta funcionando con punteros. No hay ninguna celda que no tenga paredes en ninguna prueba que haya hecho hasta ahora. Apunte: Si intento imprimir un puntero que esta apuntando a nullptr lanza una excepcion si intentas deferenciarlo(traer una copia del objeto al que apunta), si lo imprimes como puntero te devuelve 0.