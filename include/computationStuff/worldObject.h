#include <VBOVOA.h>
#ifndef WORLD_OBJECT_H
#define WORLD_OBJECT_H

#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <shader_s.h>
#include <string>
#include <lightingShaderStruct.h>
#include <camera.h>
#include <settings.h>

class WorldObject {
	private:
	VBOVOA worldObjectVBOVBA;
	int floatsPerTriangle = 3;

	public:
	Shader objectShader;
	LightingShaderAttributes objectLightingAttributes;
	OpenGLSettings currentSettings;
	std::vector<float> verticesVector;
	glm::vec3 position = glm::vec3(0, 0, 0);
	float rotation;

	WorldObject(std::vector<float> initialVerticesVector) {
		verticesVector = initialVerticesVector;
		bindVBOVBA();
	}

	void renderObjectToScreen(Camera camera, glm::vec3 i, glm::vec3 scale) {
		objectShader.use();
		objectShader.setVec3("viewPos", camera.Position);
		objectShader.setVec3(objectLightingAttributes.lightPosition.first,
							 objectLightingAttributes.lightPosition.second);
		objectShader.setVec3(objectLightingAttributes.lightAmbient.first,
							 objectLightingAttributes.lightAmbient.second);
		objectShader.setVec3(objectLightingAttributes.lightDiffuse.first,
							 objectLightingAttributes.lightDiffuse.second);
		objectShader.setVec3(objectLightingAttributes.lightSpecular.first,
							 objectLightingAttributes.lightSpecular.second);

		objectShader.setVec3(objectLightingAttributes.materialAmbient.first,
							 objectLightingAttributes.materialAmbient.second);
		objectShader.setVec3(objectLightingAttributes.materialDiffuse.first, 
							 objectLightingAttributes.materialDiffuse.second);
		objectShader.setVec3(objectLightingAttributes.materialSpecular.first,
							 objectLightingAttributes.materialSpecular.second);
		objectShader.setFloat(objectLightingAttributes.materialShininess.first,
							 objectLightingAttributes.materialShininess.second);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) currentSettings.SCR_WIDTH / (float) currentSettings.SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, i);
		model = glm::scale(model, scale);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		objectShader.setMat4("projection", projection);
		objectShader.setMat4("view", view);
		objectShader.setMat4("model", model);

		glBindVertexArray(worldObjectVBOVBA.VAO);
		glDrawArrays(GL_TRIANGLES, 0, (int) verticesVector.size() / floatsPerTriangle);
	}

	// wrapper function for previous shader constructor
	// ------------------------------------------------
	void initializeShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr) {
		objectShader.bindShader(vertexPath, fragmentPath, geometryPath);
	}

	void bindVBOVBA() {
		worldObjectVBOVBA.genAndBindVBO(verticesVector);
		worldObjectVBOVBA.generateAndBindVertexArrays();
	}

	void addNormalizedTriangles() {
		std::vector<float> normalizedVertices {};
		glm::vec3 v1, v2, v3, normalVector;
		int originalVerticesSize = (int) verticesVector.size();

		for (int i = 0; i < originalVerticesSize; i += (floatsPerTriangle*2) + 3) {
			v1 = glm::vec3(verticesVector[i], verticesVector[i + 1], verticesVector[i + 2]);
			v2 = glm::vec3(verticesVector[i + 3], verticesVector[i + 4], verticesVector[i + 5]);
			v3 = glm::vec3(verticesVector[i + 6], verticesVector[i + 7], verticesVector[i + 8]);

			normalVector = GetNormal(v1, v2, v3);

			normalizedVertices.push_back(v1.x);
			normalizedVertices.push_back(v1.y);
			normalizedVertices.push_back(v1.z);

			normalizedVertices.push_back(normalVector.x);
			normalizedVertices.push_back(normalVector.y);
			normalizedVertices.push_back(normalVector.z);

			normalizedVertices.push_back(v2.x);
			normalizedVertices.push_back(v2.y);
			normalizedVertices.push_back(v2.z);

			normalizedVertices.push_back(normalVector.x);
			normalizedVertices.push_back(normalVector.y);
			normalizedVertices.push_back(normalVector.z);

			normalizedVertices.push_back(v3.x);
			normalizedVertices.push_back(v3.y);
			normalizedVertices.push_back(v3.z);

			normalizedVertices.push_back(normalVector.x);
			normalizedVertices.push_back(normalVector.y);
			normalizedVertices.push_back(normalVector.z);
		}

	verticesVector = normalizedVertices;
	floatsPerTriangle += 3;

	worldObjectVBOVBA.rebindVBO(verticesVector);

	worldObjectVBOVBA.enableAttributes(0, floatsPerTriangle);
	worldObjectVBOVBA.enableAttributes(3, floatsPerTriangle);
	}

	void deleteVertexArraysAndBuffers() {
		glDeleteVertexArrays(1, &worldObjectVBOVBA.VAO);
		glDeleteBuffers(1, &worldObjectVBOVBA.VBO);
	}

	private:
	glm::vec3 GetNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
		return glm::normalize(glm::cross(v1 - v2, v2 - v3));
	}

};

#endif