#ifndef CHUNK_BUILDER_H
#define CHUNK_BUILDER_H

#include <vector>
#include <glm/glm.hpp>
#include <FastNoiseLite.h>
#include <map>
#include <tuple>
#include <bitset>
#include <world_building/marching_cubes.h>
#include <world_objects/baseWorldObject.h>
#include <settings.h>

// Builds marching cubes chunk and returns world object with
// the chunk as its mesh
class ChunkObject {
public:
	std::map<std::tuple<int, int, int>, char> chunkMap = {};
	glm::vec3 globalPosition;
	int chunkSize;
	OpenGLSettings currentSettings;
	WorldObject chunkWorldObject;

	ChunkObject(glm::vec3 position, FastNoiseLite noiseObject, int inputChunkSize, OpenGLSettings initialSettings);

	void generateChunk(FastNoiseLite noiseObject);

	void buildChunkFromValues();
};

#endif
