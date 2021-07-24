#ifndef LIGHTING_OBJECT_EXTENSION_H
#define LIGHTING_OBJECT_EXTENSION_H 

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <world_objects/baseWorldObjectClass.h>
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
	LightingShaderObject(std::vector<float> initialVerticesVector, const char* vertexPath, const char* fragmentPath, OpenGLSettings initialSettings, const char* geometryPath = nullptr) : WorldObject(initialVerticesVector, vertexPath, fragmentPath, initialSettings, geometryPath = nullptr) {
		objectShader.bindShader("shaders/lightingShaders/colors.vs", "shaders/lightingShaders/colors.fs");
	}

	void renderObjectToScreen(Camera camera, glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) {
		objectShader.use();

		// values specific to this shader
		objectShader.setVec3("viewPos", camera.Position);
		objectShader.setVec3(objectShaderValues.lightPosition.first,
							 objectShaderValues.lightPosition.second);
		objectShader.setVec3(objectShaderValues.lightAmbient.first,
							 objectShaderValues.lightAmbient.second);
		objectShader.setVec3(objectShaderValues.lightDiffuse.first,
							 objectShaderValues.lightDiffuse.second);
		objectShader.setVec3(objectShaderValues.lightSpecular.first,
							 objectShaderValues.lightSpecular.second);

		objectShader.setVec3(objectShaderValues.materialAmbient.first,
							 objectShaderValues.materialAmbient.second);
		objectShader.setVec3(objectShaderValues.materialDiffuse.first, 
							 objectShaderValues.materialDiffuse.second);
		objectShader.setVec3(objectShaderValues.materialSpecular.first,
							 objectShaderValues.materialSpecular.second);
		objectShader.setFloat(objectShaderValues.materialShininess.first,
							 objectShaderValues.materialShininess.second);

		// general values for 3d rendering
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) currentSettings.SCR_WIDTH / (float) currentSettings.SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		objectShader.setMat4("projection", projection);
		objectShader.setMat4("view", view);
		objectShader.setMat4("model", model);

		glBindVertexArray(worldObjectVBOVBA.VAO);
		glDrawArrays(GL_TRIANGLES, 0, numOfTriangles);
	}

};
#endif