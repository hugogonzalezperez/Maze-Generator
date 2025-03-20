#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <algorithm>
#include <array>
#include <bitset>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <random>
#include <stack>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "./cell.h"
#include "./grid.h"
#include "./renderer.h"
#include "./generation/mazeGenerator.h"
#include "./generation/dfsAlgorithm.h"
#include "./generation/wilsonAlgorithm.h"
#include "./solver/mazeSolver.h"
#include "./solver/wallFollow.h"

#endif  // UTILS_H