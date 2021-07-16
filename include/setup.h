#ifndef SETUP
#define SETUP

#include <GLFW/glfw3.h>
#include <settings.h>

class Setup {

	public:
		GLFWwindow* window;

		GLFWwindow* setupGLFWWindow(OpenGLSettings activeSettings) {
			// glfw: initialize and configure
			// ------------------------------
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			stbi_set_flip_vertically_on_load(true);

			window = glfwCreateWindow(activeSettings.SCR_WIDTH, activeSettings.SCR_HEIGHT, "bitch's ass", NULL, NULL);

			if (window == NULL)
			{
				std::cout << "Failed to create GLFW window" << std::endl;
				glfwTerminate();
			}

			glfwMakeContextCurrent(window);

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			// glad: load all OpenGL function pointers
			// ---------------------------------------
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				std::cout << "Failed to initialize GLAD" << std::endl;
				glfwTerminate();
			}

			glEnable(GL_DEPTH_TEST);

			return window;
		}

		void setCallBacks(Camera camera);

		void setInputMode();
};

#endif
