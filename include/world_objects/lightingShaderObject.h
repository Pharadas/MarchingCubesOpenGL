#ifndef LIGHTING_OBJECT_EXTENSION_H
#define LIGHTING_OBJECT_EXTENSION_H 

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <world_objects/baseWorldObject.h>
#include <shader_s.h>
#include <VBOVOA.h>

class LightingShaderObject : public WorldObject {
private:
	struct shaderValues {
		// set with values for testing
		std::pair<std::string, glm::vec3> lightPosition {"light.position", glm::vec3(0.0f, 0.0f, 0.0f)};

		std::pair<std::string, glm::vec3> lightAmbient {"light.ambient", glm::vec3(0.5f, 0.5f, 0.5f)};
		std::pair<std::string, glm::vec3> lightDiffuse {"light.diffuse", glm::vec3(0.5f, 0.1f, 0.1f)};
		std::pair<std::string, glm::vec3> lightSpecular {"light.specular", glm::vec3(0.3f, 0.2f, 0.1f)};

		std::pair<std::string, glm::vec3> materialAmbient {"material.ambient", glm::vec3(0.2f, 0.2f, 0.2f)};
		std::pair<std::string, glm::vec3> materialDiffuse {"material.diffuse", glm::vec3(0.1f, 0.2f, 0.3f)};
		std::pair<std::string, glm::vec3> materialSpecular {"material.specular", glm::vec3(0.3f, 0.2f, 0.1f)};
		std::pair<std::string, float> materialShininess {"material.shininess", 16.0f};
	};

public: 
	shaderValues objectShaderValues;

	// extension of world object class constructor
	// -------------------------------------------
	LightingShaderObject(std::vector<float> initialVerticesVector, const char* vertexPath, const char* fragmentPath, OpenGLSettings initialSettings, const char* geometryPath = nullptr);

	void renderObjectToScreen(Camera camera, glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
};
#endif