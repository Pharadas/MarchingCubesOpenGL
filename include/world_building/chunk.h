#ifndef CHUNK_BUILDER_H
#define CHUNK_BUILDER_H

#include <vector>
#include <glm/glm.hpp>
#include <FastNoiseLite.h>
#include <world_objects/lightingShaderObjectClass.h>
#include <map>
#include <tuple>
#include <bitset>
#include <world_building/marching_cubes.h>
#include <settings.h>

// Builds marching cubes chunk and returns world object with
// the chunk as its mesh
LightingShaderObject buildChunk(glm::vec3 position, FastNoiseLite noiseObject, int chunkSize, OpenGLSettings currentSettings) {
	std::vector<float> chunkVertices = {};
	std::map<std::tuple<int, int, int>, char> marchingCubesValues = {};

	// generate hashmap of chunk values using perlin noise
	for (int x = -chunkSize + position.x; x < chunkSize + position.x ; x++) {
		for (int y = -chunkSize + position.y; y < chunkSize + position.y ; y++) {
			for (int z = -chunkSize + position.z; z < chunkSize + position.z ; z++) {
				if (noiseObject.GetNoise((float) x, (float) y, (float) z) > -0.0) {
					marchingCubesValues[std::make_tuple(x, y, z)] = '1';
				} else {
					marchingCubesValues[std::make_tuple(x, y, z)] = '0';
				}
			}
		}
	}

	// build vertices from generated hashmap
	for (int x = -chunkSize + position.x; x < chunkSize + position.x  - 1; x++) {
		for (int y = -chunkSize + position.y; y < chunkSize + position.y  - 1; y++) {
			for (int z = -chunkSize + position.z; z < chunkSize + position.z  - 1; z++) {
				std::string byteString = "";

				// comentario innecesario
				byteString += marchingCubesValues[std::make_tuple(x + 1,  y + 1,  z)];
				byteString += marchingCubesValues[std::make_tuple(x + 1,  y + 1,  z + 1)];
				byteString += marchingCubesValues[std::make_tuple(x,  y + 1,  z + 1)];
				byteString += marchingCubesValues[std::make_tuple(x,  y + 1,  z)];
				byteString += marchingCubesValues[std::make_tuple(x + 1,  y,  z)];
				byteString += marchingCubesValues[std::make_tuple(x + 1,  y,  z + 1)];
				byteString += marchingCubesValues[std::make_tuple(x,  y,  z + 1)];
				byteString += marchingCubesValues[std::make_tuple(x,  y,  z)];

				std::bitset<8> activeVertices(byteString);
				std::vector<float> marchingCubesVertices;

				marchingCubesVertices = getMarchingCubes(activeVertices, glm::vec3(x, y, z));
				chunkVertices.insert(chunkVertices.end(), marchingCubesVertices.begin(), marchingCubesVertices.end());
			}
		}
	}

	LightingShaderObject chunkObject(chunkVertices, "shaders/lightingShaders/colors.vs", "shaders/lightingShaders/colors.fs", currentSettings);
	chunkObject.position = position;
	chunkObject.addNormalizedTriangles();
	return chunkObject;
}

#endif
