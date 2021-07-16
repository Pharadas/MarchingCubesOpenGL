#include <glfw/glfw3.h>

// class PlayerMovement {
// 	public:
// 		void processInput(GLFWwindow *window, ) {
// 			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
// 				glfwSetWindowShouldClose(window, true);

// 			const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
// 			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
// 				cameraPos += cameraSpeed * cameraFront;
// 			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
// 				cameraPos -= cameraSpeed * cameraFront;
// 			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
// 				cameraPos -= glm::cross(cameraFront, cameraUp) * cameraSpeed;
// 			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
// 				cameraPos += glm::cross(cameraFront, cameraUp) * cameraSpeed;
// 			if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
// 				cameraPos += cameraSpeed * cameraUp;
// 			if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
// 				cameraPos -= cameraSpeed * cameraUp;
// 		}
// };