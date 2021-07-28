#include <world_building/chunk_object.h>

ChunkObject::ChunkObject(glm::vec3 position, FastNoiseLite noiseObject, int inputChunkSize, OpenGLSettings initialSettings) {
	this->globalPosition = position;
	this->chunkSize = inputChunkSize;
	this->currentSettings = initialSettings;

	generateChunk(noiseObject);
	buildChunkFromValues();
}

void ChunkObject::generateChunk(FastNoiseLite noiseObject) {
	glm::vec3 globalChunkCoords(globalPosition.x * chunkSize, globalPosition.y * chunkSize, globalPosition.z * chunkSize);

	// generate hashmap of chunk values using perlin noise
	for (int x = -chunkSize * 0.5; x < chunkSize * 0.5 + 1; x++) {
		for (int y = -chunkSize * 0.5; y < chunkSize * 0.5 + 1; y++) {
			for (int z = -chunkSize * 0.5; z < chunkSize * 0.5 + 1; z++) {
				if (noiseObject.GetNoise((float) x + globalChunkCoords.x, (float) y + globalChunkCoords.y, (float) z + globalChunkCoords.z) > -0.0) {
					chunkMap[std::make_tuple(x, y, z)] = '1';
				} else {
					chunkMap[std::make_tuple(x, y, z)] = '0';
				}
			}
		}
	}
}

void ChunkObject::buildChunkFromValues() {
	std::vector<float> chunkVertices = {};

	// build vertices from generated hashmap
	for (int x = -chunkSize * 0.5; x < chunkSize * 0.5; x++) {
		for (int y = -chunkSize * 0.5; y < chunkSize * 0.5; y++) {
			for (int z = -chunkSize * 0.5; z < chunkSize * 0.5; z++) {
				std::string byteString = "";

				// comentario innecesario
				byteString += chunkMap[std::make_tuple(x + 1,  y + 1,  z)];
				byteString += chunkMap[std::make_tuple(x + 1,  y + 1,  z + 1)];
				byteString += chunkMap[std::make_tuple(x,  y + 1,  z + 1)];
				byteString += chunkMap[std::make_tuple(x,  y + 1,  z)];
				byteString += chunkMap[std::make_tuple(x + 1,  y,  z)];
				byteString += chunkMap[std::make_tuple(x + 1,  y,  z + 1)];
				byteString += chunkMap[std::make_tuple(x,  y,  z + 1)];
				byteString += chunkMap[std::make_tuple(x,  y,  z)];

				std::bitset<8> activeVertices(byteString);
				std::vector<float> marchingCubesVertices;

				marchingCubesVertices = getMarchingCubes(activeVertices, glm::vec3(x, y, z));
				chunkVertices.insert(chunkVertices.end(), marchingCubesVertices.begin(), marchingCubesVertices.end());
			}
		}
	}

	WorldObject chunkObject(chunkVertices, "shaders/vertexShader.vs", "shaders/fragmentShader.fs", currentSettings);
	chunkObject.position = globalPosition * glm::vec3(chunkSize);
	chunkObject.addUVs();
	chunkWorldObject = chunkObject;
	// chunkWorldObject = &chunkObject;
	// chunkObject.addNormalizedTriangles();
}
