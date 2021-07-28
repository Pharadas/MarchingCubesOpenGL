#include <world_objects/lightingShaderObject.h>

LightingShaderObject::LightingShaderObject(std::vector<float> initialVerticesVector, const char* vertexPath, const char* fragmentPath, OpenGLSettings initialSettings, const char* geometryPath) {
	// initializer constructor
	this->verticesVector = initialVerticesVector;
	bindVBOVBA();
	worldObjectVBOVBA.enableAttributes(0, floatsPerTriangle);
	objectShader.bindShader(vertexPath, fragmentPath, geometryPath);
	this->currentSettings = initialSettings;
	numOfTriangles = verticesVector.size() / floatsPerTriangle;
}

void LightingShaderObject::renderObjectToScreen(Camera camera, glm::vec3 position, glm::vec3 scale) {
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
