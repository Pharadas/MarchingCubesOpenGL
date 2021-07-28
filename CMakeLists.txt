cmake_minimum_required(VERSION 3.10)

# project name
project("GamingTime")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

file(GLOB SOURCES "*/.cpp"
				  "src/*.cpp"
				  "src/*.c"
				  "include/*.cpp"
				  "include/world_building/chunk_object.cpp"
				  "include/world_building/marching_cubes.cpp"
				  "include/world_objects/baseWorldObject.cpp"
				  "include/world_objects/lightingShaderObject.cpp")

INCLUDE_DIRECTORIES(include/)
LINK_DIRECTORIES(lib/)

add_executable("${PROJECT_NAME}" "${SOURCES}")
TARGET_LINK_LIBRARIES(GamingTime glfw3dll)
TARGET_LINK_LIBRARIES(GamingTime opengl32)
TARGET_LINK_LIBRARIES(GamingTime glew32)
TARGET_LINK_LIBRARIES(GamingTime freeglut_static)

# install(TARGETS "${PROJECT_NAME}" DESTINATION build)
# install(FILES "src/main.cpp" DESTINATION src)

# add_executable(src/main src/main.cxx)