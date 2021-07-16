#include <string>
#include <vector>
#include <glm/glm.hpp>

struct LightingShaderAttributes {
	std::pair<std::string, glm::vec3> lightPosition {"light.position", glm::vec3(0.0f, 0.0f, 0.0f)};

	std::pair<std::string, glm::vec3> lightAmbient {"light.ambient", glm::vec3(0.0f, 0.0f, 0.0f)};
	std::pair<std::string, glm::vec3> lightDiffuse {"light.diffuse", glm::vec3(0.0f, 0.0f, 0.0f)};
	std::pair<std::string, glm::vec3> lightSpecular {"light.specular", glm::vec3(0.0f, 0.0f, 0.0f)};

	std::pair<std::string, glm::vec3> materialAmbient {"material.ambient", glm::vec3(0.0f, 0.0f, 0.0f)};
	std::pair<std::string, glm::vec3> materialDiffuse {"material.diffuse", glm::vec3(0.0f, 0.0f, 0.0f)};
	std::pair<std::string, glm::vec3> materialSpecular {"material.specular", glm::vec3(0.0f, 0.0f, 0.0f)};
	std::pair<std::string, float> materialShininess {"material.shininess", 32.0f};
};