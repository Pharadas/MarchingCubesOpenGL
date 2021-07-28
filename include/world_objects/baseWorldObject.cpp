#include <world_objects/baseWorldObject.h>
#include <vector>
#include <settings.h>

WorldObject::WorldObject(std::vector<float> initialVerticesVector, const char* vertexPath, const char* fragmentPath, OpenGLSettings initialSettings, const char* geometryPath) {
	// initializer constructor
	this->verticesVector = initialVerticesVector;
	bindVBOVBA();
	worldObjectVBOVBA.enableAttributes(0, floatsPerTriangle);
	objectShader.bindShader(vertexPath, fragmentPath, geometryPath);
	this->currentSettings = initialSettings;
	numOfTriangles = verticesVector.size() / floatsPerTriangle;
}

WorldObject::~WorldObject() {
	glDeleteVertexArrays(1, &worldObjectVBOVBA.VAO);
	glDeleteBuffers(1, &worldObjectVBOVBA.VBO);
}

void WorldObject::renderObjectToScreen(Camera camera, glm::vec3 scale = glm::vec3(1, 1, 1)) {
	// tell opengl's context to use this shader
	objectShader.use();
	objectShader.setVec3("viewPos", camera.Position);

	// get necessary matrices for multiplications we'll need later on
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) currentSettings.SCR_WIDTH / (float) currentSettings.SCR_HEIGHT, 0.01f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
	// model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

	// pass these values to the shader's uniforms
	objectShader.setMat4("projection", projection);
	objectShader.setMat4("view", view);
	objectShader.setMat4("model", model);

	// tell opengl to use this object's vbo to allow us to draw the triangles
	glBindVertexArray(worldObjectVBOVBA.VAO);
	glDrawArrays(GL_TRIANGLES, 0, numOfTriangles);
}

void WorldObject::bindVBOVBA() {
	worldObjectVBOVBA.genAndBindVBO(verticesVector);
	worldObjectVBOVBA.generateAndBindVertexArrays();
}

void WorldObject::rebindVBOWrapper() {
	worldObjectVBOVBA.rebindVBO(verticesVector);
}

void WorldObject::addNormalizedTriangles() {
	std::vector<float> normalizedVertices {};
	glm::vec3 v1, v2, v3, normalVector;
	int originalVerticesSize = (int) verticesVector.size();

	for (int i = 0; i < originalVerticesSize; i += floatsPerTriangle*3) {
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
	numOfTriangles = verticesVector.size() / floatsPerTriangle;

	worldObjectVBOVBA.rebindVBO(verticesVector);

	worldObjectVBOVBA.numOfAttributes = 0;
	worldObjectVBOVBA.enableAttributes(0, floatsPerTriangle);
	worldObjectVBOVBA.enableAttributes(3, floatsPerTriangle);
}

void WorldObject::addUVs() {
	std::vector<float> UVdVertices {};
	int originalVerticesSize = (int) verticesVector.size();

	for (int i = 0; i < originalVerticesSize; i += floatsPerTriangle * 3) {
		UVdVertices.push_back(verticesVector[i    ]);
		UVdVertices.push_back(verticesVector[i + 1]);
		UVdVertices.push_back(verticesVector[i + 2]);

		UVdVertices.push_back(0.0f);
		UVdVertices.push_back(0.0f);

		UVdVertices.push_back(verticesVector[i + 3]);
		UVdVertices.push_back(verticesVector[i + 4]);
		UVdVertices.push_back(verticesVector[i + 5]);

		UVdVertices.push_back(1.0f);
		UVdVertices.push_back(0.0f);

		UVdVertices.push_back(verticesVector[i + 6]);
		UVdVertices.push_back(verticesVector[i + 7]);
		UVdVertices.push_back(verticesVector[i + 8]);
		
		UVdVertices.push_back(1.0f);
		UVdVertices.push_back(1.0f);
	}

	verticesVector = UVdVertices;
	floatsPerTriangle += 2;
	numOfTriangles = verticesVector.size() / floatsPerTriangle;

	worldObjectVBOVBA.rebindVBO(verticesVector);

	worldObjectVBOVBA.numOfAttributes = 0;
	worldObjectVBOVBA.enableAttributes(0, floatsPerTriangle);
	worldObjectVBOVBA.enableAttributes(3, floatsPerTriangle);
}

glm::vec3 WorldObject::GetNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
	return glm::normalize(glm::cross(v1 - v2, v2 - v3));
}
