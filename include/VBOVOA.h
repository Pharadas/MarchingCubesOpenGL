#ifndef VBOVBA_H
#define VBOVBA_H
#include <glad/glad.h>
#include <vector>
#include <iostream>

class VBOVOA {
	public:
		unsigned int  VBO, VAO;
		int numOfAttributes = 0;

		void genAndBindVBO(std::vector<float> vectorVertices) {	
			float vertices[(int) vectorVertices.size()];
			for (int i = 0; i < (int) vectorVertices.size(); i++) {
				vertices[i] = vectorVertices[i];
			}

			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}

		void rebindVBO(std::vector<float> vectorVertices) {
			float vertices[(int) vectorVertices.size()];
			for (int i = 0; i < (int) vectorVertices.size(); i++) {
				vertices[i] = vectorVertices[i];
			}

			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		}

		void generateAndBindVertexArrays() {
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
		}

		void enableAttributes(int startingPos, int floatsPerTriangle) {
			// position attribute
			glVertexAttribPointer(numOfAttributes, 3, GL_FLOAT, GL_FALSE, floatsPerTriangle * sizeof(float), (void*)(startingPos * sizeof(float)));
			glEnableVertexAttribArray(numOfAttributes);
			numOfAttributes++;
		}

			// texture coord attribute
			// glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			// glEnableVertexAttribArray(1);
};
#endif