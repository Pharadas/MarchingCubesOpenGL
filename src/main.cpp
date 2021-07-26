#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_s.h>
#include <camera.h>

#include <string>
#include <vector>
#include <FastNoiseLite.h>
#include <iostream>
#include <bitset>
#include <map>
#include <tuple>

#include <gameplay_functions/line_triangle_intersection.h>
#include <texture.h>
#include <VBOVOA.h>
#include <setup.h> 
#include <settings.h>
#include <setup.h>
#include <world_objects/baseWorldObjectClass.h>
#include <world_objects/lightingShaderObjectClass.h>
#include <world_building/marching_cubes.h>
#include <world_building/chunk.h>

void processInput(GLFWwindow* window);
glm::vec3 GetNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

OpenGLSettings currentSettings;

float thisAngle = 0.0;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, -3.0f));
Setup thisSetup;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	// settings
	currentSettings.SCR_WIDTH = 1000;
	currentSettings.SCR_HEIGHT = 1000;
	GLFWwindow* window = thisSetup.setupGLFWWindow(currentSettings);

	camera.lastX = currentSettings.SCR_WIDTH * 0.5;
	camera.lastY = currentSettings.SCR_HEIGHT * 0.5;

	std::vector<float> cubeVertices {
		// positions		  // normals
        -0.5f, -0.5f, -0.5f, // 0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f, // 0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, // 0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, // 0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f, // 0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, // 0.0f,  0.0f, -1.0f,

         0.5f,  0.5f,  0.5f, // 0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, // 0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f, // 0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f, // 0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, // 0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f, // 0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f, // -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, // -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, // -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, // -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, // -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, // -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f, // 1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f, // 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f, // 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f, // 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f, // 1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f, // 1.0f,  0.0f,  0.0f,

         0.5f, -0.5f, -0.5f, // 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, // 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f, // 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, // 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f, // 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, // 0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f, // 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f, // 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f, // 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f, // 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, // 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, // 0.0f,  1.0f,  0.0f
	};

	// build and compile our shader zprogram
    // ------------------------------------
	WorldObject lightCube(cubeVertices, "shaders/lightingShaders/light_cube.vs", "shaders/lightingShaders/light_cube.fs", currentSettings);
	lightPos.z = 20.0f;
	lightCube.position = lightPos;

	// noiseShit
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	noise.SetSeed(1);
	noise.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
	noise.SetFractalOctaves(4);
	noise.SetFrequency(0.25);

	std::vector<WorldObject> chunks = {};
	int renderDistance = 1;
	int chunkSize = 5;

	// for (int x = -renderDistance; x < renderDistance; x++) {
	// 	// chunks.push_back(buildChunk(glm::vec3(x * chunkSize, 0.0f, 0.0f), noise, chunkSize, currentSettings));
	// 	for (int y = -renderDistance; y < renderDistance; y++) {
	// 			chunks.push_back(buildChunk(glm::vec3(x, y, z), noise, chunkSize, currentSettings));
	// 		for (int z = -renderDistance; z < renderDistance; z++) {
	// 		}
	// 	}
	// }

	chunks.push_back(buildChunk(glm::vec3(0, 0, 0), noise, chunkSize, currentSettings));

	std::vector<float> singleTriangleVertices = {
		 0.5f,  0.5f,  0.5f, // 0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f,  0.5f, // 0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f,  0.5f, // 0.0f,  0.0f, -1.0f,
	};

	// WorldObject testTriangle(singleTriangleVertices, "shaders/vertexShader.vs", "shaders/fragmentShader.fs", currentSettings);
	// testTriangle.addUVs();
	// testTriangle.position.x += 1;

	// chunks.push_back(buildChunk(glm::vec3(0, 0, 0), noise, chunkSize, currentSettings));
	WorldObject rayHitObject(cubeVertices, "shaders/vertexShader.vs", "shaders/fragmentShader.fs", currentSettings);

	Texture currentTexture("textures/videoman.jpg");
	// testTriangle.objectShader.use();
	// testTriangle.objectShader.setInt("texture1", 0);
	chunks[0].objectShader.use();
	chunks[0].objectShader.setInt("texture1", 0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, currentTexture.texture);

		std::pair<bool, glm::vec3> hitRayPosition = meshRayCasting(camera.Position, 
																   glm::normalize(camera.Front),
																   chunks[0].verticesVector,
																   chunks[0].floatsPerTriangle,
																   chunks[0].position);

		if (hitRayPosition.first) {
			rayHitObject.position = hitRayPosition.second;
			rayHitObject.renderObjectToScreen(camera, glm::vec3(0.1f));
		}

		// testTriangle.renderObjectToScreen(camera, glm::vec3(1.0f));

		// render all objects
		for (auto i : chunks) {
			// i.objectShaderValues.lightPosition.second = lightPos;
			i.renderObjectToScreen(camera, glm::vec3(1.0f));
		}

		lightCube.renderObjectToScreen(camera, glm::vec3(1.0f));

		// swap current buffer and get events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
	for (auto i : chunks) {
		i.deleteVertexArraysAndBuffers();
	}
	lightCube.deleteVertexArraysAndBuffers();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
	GLdouble xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);

	camera.ProcessMouseMovement(xPos, yPos);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5 * deltaTime;
	// para no volar, solo multiplicar la velocidad (de deltatime) con el camera front X y Z pero dejando Y en 0
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	// if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
	// 	for (int x = (int) camera.Position.x - 2; x < (int) camera.Position.x + 2; x++) {
	// 		for (int y = (int) camera.Position.y  - 2; y < (int) camera.Position.y + 2; y++) {
	// 			for (int z = (int) camera.Position.z - 2; z < (int) camera.Position.z + 2; z++) {
	// 				marchingCubesValues[std::make_tuple(x, y, z)] = '0'; // falta volver a construir el original vertices del hashmap de valores
	// 				// testCube.worldObjectVBOVBA.rebindVBO(originalVertices);
	// 			}
	// 		}
	// 	}

	// 	originalVertices = {};

	// 	for (int x = -5; x < 5 - 1; x++) {
	// 		for (int y = -5; y < 5 - 1; y++) {
	// 			for (int z = -5; z < 5 - 1; z++) {
	// 				std::string byteString = "";

	// 				byteString += marchingCubesValues[std::make_tuple(x + 1,  y + 1,  z)];
	// 				byteString += marchingCubesValues[std::make_tuple(x + 1,  y + 1,  z + 1)];
	// 				byteString += marchingCubesValues[std::make_tuple(x,  y + 1,  z + 1)];
	// 				byteString += marchingCubesValues[std::make_tuple(x,  y + 1,  z)];
	// 				byteString += marchingCubesValues[std::make_tuple(x + 1,  y,  z)];
	// 				byteString += marchingCubesValues[std::make_tuple(x + 1,  y,  z + 1)];
	// 				byteString += marchingCubesValues[std::make_tuple(x,  y,  z + 1)];
	// 				byteString += marchingCubesValues[std::make_tuple(x,  y,  z)];

	// 				// std::cout << byteString << std::endl;

	// 				std::bitset<8> activeVertices(byteString);
	// 				std::vector<float> marchingCubesVertices;

	// 				marchingCubesVertices = getMarchingCubes(activeVertices, glm::vec3(x, y, z));
	// 				originalVertices.insert(originalVertices.end(), marchingCubesVertices.begin(), marchingCubesVertices.end());
	// 			}
	// 		}
	// 	}
	
	// 	testCube.verticesVector = originalVertices;
	// 	testCube.floatsPerTriangle = 3;
	// 	testCube.addNormalizedTriangles();
	// 	testCube.rebindVBOWrapper();
	// }
}
