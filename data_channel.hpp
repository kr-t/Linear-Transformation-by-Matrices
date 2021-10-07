#ifndef DATA_CHANNEL_HPP
#define DATA_CHANNEL_HPP

#include <fstream>
#include <iostream>
#include <future>
#include <vector>
#include "pipe.hpp"

/* Type definitions */
using matrix = std::vector<std::vector<int>>;
using tensor = std::vector<std::vector<std::vector<int>>>;

/* Constans */
const unsigned int VECTOR_DIMENSION_SIZE = 3;
const unsigned int MATRIX_DIMENSION_SIZE = 4;

/* Extern function declarations */
extern tensor create_tensor(int z, int y, int x, int value = int{});
extern matrix create_matrix(int y, int x, int value = int{});
extern void startDataFlow(unsigned int numberOfMatrices, unsigned int numberOfVectors, matrix vectors, tensor matrices);

#endif /* DATA_CHANNEL_HPP */
