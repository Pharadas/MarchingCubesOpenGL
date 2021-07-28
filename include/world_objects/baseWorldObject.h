#ifndef WORLD_OBJECT_H
#define WORLD_OBJECT_H

#include <VBOVOA.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <shader_s.h>
#include <string>
#include <camera/camera.h>
#include <settings.h>

class WorldObject {
	protected:
	int numOfTriangles;
	VBOVOA worldObjectVBOVBA;
	OpenGLSettings currentSettings;

	public:
	Shader objectShader; // put it back in private after test dumbshit
	int floatsPerTriangle = 3;
	float rotation = 0.0f;
	std::vector<float> verticesVector;
	glm::vec3 position = glm::vec3(0, 0, 0);

	WorldObject() {};
	WorldObject(std::vector<float> initialVerticesVector, const char* vertexPath, const char* fragmentPath, OpenGLSettings initialSettings, const char* geometryPath = nullptr);
	~WorldObject();

	virtual void renderObjectToScreen(Camera camera, glm::vec3 scale);

	void bindVBOVBA();

	void rebindVBOWrapper();

	// Create vector<float> of object's vertices values with their normals and update enabled attributes
	// -------------------------------------------------------------------------------------------------
	void addNormalizedTriangles();

	void addUVs();

	void deleteVertexArraysAndBuffers();

	private:
	glm::vec3 GetNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
};

#endif