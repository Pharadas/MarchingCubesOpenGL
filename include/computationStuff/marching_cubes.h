#ifndef MARCHING_CUBES_H
#define MARCHING_CUBES_H

#include <bitset>
#include <vector>
#include <glm/glm.hpp>

std::vector<float> getMarchingCubes(std::bitset<8> activeVertices, glm::vec3 position);

#endif