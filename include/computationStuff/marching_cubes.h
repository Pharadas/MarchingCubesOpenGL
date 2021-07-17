#ifndef MARCHING_CUBES_H
#define MARCHING_CUBES_H

#include <bitset>
#include <vector>

std::vector<float> getMarchingCubes(std::bitset<8> activeVertices);

#endif