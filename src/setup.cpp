// #include <setup.h>
// #include <iostream>
// #include <camera.h>
// #include <stb_image.h>

// GLFWwindow* Setup::setupGLFWWindow(OpenGLSettings activeSettings) {
// 	// glfw: initialize and configure
//     // ------------------------------
//     glfwInit();
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//     stbi_set_flip_vertically_on_load(true);

// 	window = glfwCreateWindow(activeSettings.SCR_WIDTH, activeSettings.SCR_HEIGHT, "gaming time", NULL, NULL);

// 	if (window == NULL)
//     {
//         std::cout << "Failed to create GLFW window" << std::endl;
//         glfwTerminate();
//     }

// 	glfwMakeContextCurrent(window);

// 	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

// 	// glad: load all OpenGL function pointers
//     // ---------------------------------------
//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//     {
//         std::cout << "Failed to initialize GLAD" << std::endl;
// 		glfwTerminate();
//     }

// 	glEnable(GL_DEPTH_TEST);

// 	return window;
// }

// void Setup::setCallBacks(Camera camera) {
// 	auto framebuffer_size_callback = [](GLFWwindow* window, int width, int height)
// 	{
// 		// make sure the viewport matches the new window dimensions; note that width and
// 		// height will be significantly larger than specified on retina displays.
// 		glViewport(0, 0, width, height);
// 	};

// 	glfwSetFramebufferSizeCallback(Setup::window, framebuffer_size_callback);
// }
