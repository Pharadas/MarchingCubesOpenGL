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

#include <texture.h>
#include <VBOVOA.h>
#include <setup.h> 
#include <settings.h>
#include <setup.h>
#include <computationStuff/worldObject.h>
#include <computationStuff/marching_cubes.h>

// void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
glm::vec3 GetNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

OpenGLSettings currentSettings;

float thisAngle = 0.0;

// camera
Camera camera(glm::vec3(0.0f, 3.0f, 3.0f));
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
	currentSettings.SCR_HEIGHT = 800;
	GLFWwindow* window = thisSetup.setupGLFWWindow(currentSettings);

	camera.lastX = currentSettings.SCR_WIDTH * 0.5;
	camera.lastY = currentSettings.SCR_HEIGHT * 0.5;

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightCubeShader;
	lightCubeShader.bindShader("shaders/lightingShaders/light_cube.vs", "shaders/lightingShaders/light_cube.fs");

	std::vector<float> cubeVertices {
		// positions		  // normals
        -0.5f, -0.5f, -0.5f, // 0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f, // 0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, // 0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, // 0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f, // 0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, // 0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f, // 0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f, // 0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f, // 0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f, // 0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f, // 0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, // 0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, // -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, // -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, // -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, // -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, // -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, // -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f, // 1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f, // 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f, // 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f, // 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f, // 1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f, // 1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f, // 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f, // 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f, // 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f, // 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, // 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, // 0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f, // 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f, // 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f, // 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f, // 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, // 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, // 0.0f,  1.0f,  0.0f
	};

	// noiseShit
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	noise.SetSeed(1);
	noise.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
	noise.SetFractalOctaves(1);
	noise.SetFrequency(1);

	std::vector<glm::vec3> cubePositions = {};
	std::map<std::tuple<int, int, int>, char> marchingCubesValues = {};

	int size = 10;
	std::vector<float> originalVertices = {};

// sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)) < size

	for (int x = -size; x < size; x++) {
		for (int y = -size; y < size; y++) {
			for (int z = -size; z < size; z++) {
				// float thisNoiseValue = noise.GetNoise((float) x, (float) y, (float) z);
				std::string byteString = "";
				std::vector<float> marchingCubesVertices;

				byteString += (noise.GetNoise((float) x + 1, (float) y + 1, (float) z) > -0.0603221) ? "1" : "0";
				byteString += (noise.GetNoise((float) x + 1, (float) y + 1, (float) z + 1) > -0.0603221) ? "1" : "0";
				byteString += (noise.GetNoise((float) x, (float) y + 1, (float) z + 1) > -0.0603221) ? "1" : "0";
				byteString += (noise.GetNoise((float) x, (float) y + 1, (float) z) > -0.0603221) ? "1" : "0";
				byteString += (noise.GetNoise((float) x + 1, (float) y, (float) z) > -0.0603221) ? "1" : "0";
				byteString += (noise.GetNoise((float) x + 1, (float) y, (float) z + 1) > -0.0603221) ? "1" : "0";
				byteString += (noise.GetNoise((float) x, (float) y, (float) z + 1) > -0.0603221) ? "1" : "0";
				byteString += (noise.GetNoise((float) x, (float) y, (float) z) > -0.0603221) ? "1" : "0";

				std::bitset<8> activeVertices(byteString);
				marchingCubesVertices = getMarchingCubes(activeVertices, glm::vec3(x, y, z));
				originalVertices.insert(originalVertices.end(), marchingCubesVertices.begin(), marchingCubesVertices.end());
				// if (thisNoiseValue > -0.0603221) {
				// if ((thisNoiseValue > -0.0) && (sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)) < size)) {
				// 	marchingCubesValues[std::make_tuple(x, y, z)] = '1';
				// } else {
				// 	marchingCubesValues[std::make_tuple(x, y, z)] = '0';
				// }
			}
		}
	}

	// std::string byteString = "";
	// std::cin >> byteString;
	// // std::string byteString = "11000000";
	// std::bitset<8> activeVertices(byteString);
	// originalVertices = getMarchingCubes(activeVertices, glm::vec3(0, 0, 0));

	// for (int x = -size; x < size - 1; x++) {
	// 	for (int y = -size; y < size - 1; y++) {
	// 		for (int z = -size; z < size - 1; z++) {
	// 			std::string byteString = "";

	// 			byteString += marchingCubesValues[std::make_tuple(x, 	  y, 	 z)];
	// 			byteString += marchingCubesValues[std::make_tuple(x	, y, 	 z - 1)];
	// 			byteString += marchingCubesValues[std::make_tuple(x - 1, y, 	 z - 1)];
	// 			byteString += marchingCubesValues[std::make_tuple(x - 1, y, 	 z)];
	// 			byteString += marchingCubesValues[std::make_tuple(x, 	  y - 1, z)];
	// 			byteString += marchingCubesValues[std::make_tuple(x	, y - 1, z - 1)];
	// 			byteString += marchingCubesValues[std::make_tuple(x - 1, y - 1, z - 1)];
	// 			byteString += marchingCubesValues[std::make_tuple(x - 1, y - 1, z)];

	// 			// std::cout << byteString << std::endl;

	// 			std::bitset<8> activeVertices(byteString);
	// 			std::vector<float> marchingCubesVertices;

	// 			marchingCubesVertices = getMarchingCubes(activeVertices, glm::vec3(x, y, z));
	// 			originalVertices.insert(originalVertices.end(), marchingCubesVertices.begin(), marchingCubesVertices.end());
	// 		}
	// 	}
	// }

	// for (auto i : originalVertices)

    Shader lightingShader;
	WorldObject testCube(originalVertices);
	testCube.currentSettings = currentSettings;
	testCube.objectShader = lightingShader;
	testCube.initializeShader("shaders/lightingShaders/colors.vs", "shaders/lightingShaders/colors.fs");
	testCube.addNormalizedTriangles();
	testCube.objectLightingAttributes.lightPosition.second = glm::vec3(1.0f, 1.0f, 2.0f);
	testCube.objectLightingAttributes.lightAmbient.second = glm::vec3(0.5, 0.5, 0.5);
	testCube.objectLightingAttributes.lightDiffuse.second = glm::vec3(0.1f, 0.2f, 0.3f);
	testCube.objectLightingAttributes.lightSpecular.second = glm::vec3(1.0f, 1.0f, 1.0f);

	testCube.objectLightingAttributes.materialAmbient.second = glm::vec3(0.3f, 0.3f, 0.3f);
	testCube.objectLightingAttributes.materialDiffuse.second = glm::vec3(1.0f, 0.5f, 0.31f);
	testCube.objectLightingAttributes.materialSpecular.second = glm::vec3(0.5f, 0.5f, 0.5f);
	testCube.objectLightingAttributes.materialShininess.second = 32.0f;

	glm::vec3 debugCubesPositions[] = {
		glm::vec3(-0.5, -0.5, -0.5),
		glm::vec3(-0.5, -0.5, 0.5),
		glm::vec3(0.5, -0.5, 0.5),
		glm::vec3(0.5, -0.5, -0.5),

		glm::vec3(-0.5, 0.5, -0.5),
		glm::vec3(-0.5, 0.5, 0.5),
		glm::vec3(0.5, 0.5, 0.5),
		glm::vec3(0.5, 0.5, -0.5),
	};

	WorldObject debugCubeObject(cubeVertices);

	debugCubeObject.currentSettings = currentSettings;
	debugCubeObject.objectShader = lightingShader;
	debugCubeObject.initializeShader("shaders/lightingShaders/colors.vs", "shaders/lightingShaders/colors.fs");
	debugCubeObject.addNormalizedTriangles();

	debugCubeObject.objectLightingAttributes.lightPosition.second = glm::vec3(1.0f, 1.0f, 2.0f);
	debugCubeObject.objectLightingAttributes.lightAmbient.second = glm::vec3(0.5, 0.5, 0.5);
	debugCubeObject.objectLightingAttributes.lightDiffuse.second = glm::vec3(0.1f, 0.2f, 0.3f);
	debugCubeObject.objectLightingAttributes.lightSpecular.second = glm::vec3(1.0f, 1.0f, 1.0f);

	debugCubeObject.objectLightingAttributes.materialAmbient.second = glm::vec3(0.3f, 0.3f, 0.3f);
	debugCubeObject.objectLightingAttributes.materialDiffuse.second = glm::vec3(1.0f, 0.5f, 0.31f);
	debugCubeObject.objectLightingAttributes.materialSpecular.second = glm::vec3(0.5f, 0.5f, 0.5f);
	debugCubeObject.objectLightingAttributes.materialShininess.second = 32.0f;

	// float vertices[(int) normalizedVertices.size()];
	// for (int i = 0; i < (int) normalizedVertices.size(); i++) {
	// 	vertices[i] = normalizedVertices[i];
	// }

	VBOVOA cubeVBOVOA;
	cubeVBOVOA.genAndBindVBO(cubeVertices);
	cubeVBOVOA.generateAndBindVertexArrays();
	// std::cout << "cube VOA" << std::endl;
	cubeVBOVOA.enableAttributes(0, 3);

	Texture currentTexture;
	currentTexture.loadTexture("textures/videoman.jpg");

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
        // glBindTexture(GL_TEXTURE_2D, currentTexture.texture);

		// for (auto i : cubePositions) {
		testCube.objectLightingAttributes.lightPosition.second = lightPos;
		testCube.renderObjectToScreen(camera, glm::vec3(0, 0, 0), glm::vec3(1.0f));
		// }

		// for (int i = 0; i < 8; i++) {
		// 	if (byteString[i] == '1') {
		// 		debugCubeObject.objectLightingAttributes.materialAmbient.second = glm::vec3(0.0f, 0.8f, 0.0f);
		// 		debugCubeObject.renderObjectToScreen(camera, debugCubesPositions[i], glm::vec3(0.1f));
		// 	} else {
		// 		debugCubeObject.objectLightingAttributes.materialAmbient.second = glm::vec3(0.8f, 0.0f, 0.0f);
		// 		debugCubeObject.renderObjectToScreen(camera, debugCubesPositions[i], glm::vec3(0.1f));
		// 	}
		// }

		// for (int i = 0; i < 8; i++) {
		// 	debugCubeObject[i].renderObjectToScreen(camera, debugCubesPositions[i], glm::vec3(0.1f));
		// 	std::cout << debugCubesPositions[i].x << ' ' << debugCubesPositions[i].y << ' ' << debugCubesPositions[i].z << std::endl;
		// }

		// // pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) currentSettings.SCR_WIDTH / (float) currentSettings.SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 model = glm::mat4(1.0f);
		const float radius = 32.0f;
		// float lightX = sin(glfwGetTime() * 0.1) * radius;
		// float lightZ = cos(glfwGetTime() * 0.1) * radius;
		// lightPos = glm::vec3(lightX, 1.0f, lightZ);
		lightPos.z = 10.0f;
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		/////////////////////////////////////////////

		// lighting cube shader bindings and transformations
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection); // we will use same projection as before
		lightCubeShader.setMat4("view", view); // we will use same view as before
		lightCubeShader.setMat4("model", model);

		glBindVertexArray(cubeVBOVOA.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);		

        // thisAngle += 20 * deltaTime;
		// lightPos.y += 0.2f * deltaTime;
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
	testCube.deleteVertexArraysAndBuffers();
    glDeleteVertexArrays(1, &cubeVBOVOA.VAO);
    glDeleteBuffers(1, &cubeVBOVOA.VBO);

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
}

glm::vec3 GetNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	glm::vec3 temp1, temp2;
	glm::vec3 normal;

	temp1 = v1 - v2;
	temp2 = v2 - v3;

	// std::cout << temp1.x << " " << temp1.y << " " << temp1.z << std::endl;
	// std::cout << temp2.x << " " << temp2.y << " " << temp2.z << std::endl;
	// assume * represents cross-product
	normal = glm::normalize(glm::cross(temp1, temp2));
	// normal = temp1 * temp2;

	return normal;
}
