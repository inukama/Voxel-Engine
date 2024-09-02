#ifndef GL
#define GL
	#include <glad/glad.h>
	#include <GLFW/glfw3.h>
#endif

#include <spdlog/spdlog.h>

#include <glm/glm.hpp>

#include "model.h"

Model::Model() {
	spdlog::trace("Model::Model() (Initialisation)");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);		
} 

void Model::buffer() {
	spdlog::info("Buffering mesh");
	
	glBindVertexArray(VAO);	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(Vertex), &data[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);
}

void Model::render() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, data.size());
	glBindVertexArray(0);
}

void Model::add_quad(enum Face face, glm::vec3 pos) {
	Vertex vtx;

	switch (face) {
		case TOP:
			vtx.normal = glm::vec3(0.0f, 1.0f, 0.0f);
			
			vtx.position = pos + glm::vec3(0.0, 1.0, 0.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(1.0, 1.0, 0.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(0.0, 1.0, 1.0);
			data.push_back(vtx);

			vtx.position = pos + glm::vec3(1.0, 1.0, 0.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(0.0, 1.0, 1.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(1.0, 1.0, 1.0);
			data.push_back(vtx);
			break;
	
		case BOTTOM:
			vtx.normal = glm::vec3(0.0f, -1.0f, 0.0f);
			
			vtx.position = pos + glm::vec3(0.0, 0.0, 0.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(1.0, 0.0, 0.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(0.0, 0.0, 1.0);
			data.push_back(vtx);

			vtx.position = pos + glm::vec3(1.0, 0.0, 0.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(0.0, 0.0, 1.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(1.0, 0.0, 1.0);
			data.push_back(vtx);
			break;

		case RIGHT:
			vtx.normal = glm::vec3(1.0f, 0.0f, 0.0f);
			
			vtx.position = pos + glm::vec3(1.0, 0.0, 0.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(1.0, 1.0, 0.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(1.0, 0.0, 1.0);
			data.push_back(vtx);

			vtx.position = pos + glm::vec3(1.0, 1.0, 0.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(1.0, 0.0, 1.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(1.0, 1.0, 1.0);
			data.push_back(vtx);
			break;
	
		case LEFT:
			vtx.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
			
			vtx.position = pos + glm::vec3(0.0, 0.0, 0.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(0.0, 1.0, 0.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(0.0, 0.0, 1.0);
			data.push_back(vtx);

			vtx.position = pos + glm::vec3(0.0, 1.0, 0.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(0.0, 0.0, 1.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(0.0, 1.0, 1.0);
			data.push_back(vtx);
			break;
			
		case FRONT:
			vtx.normal = glm::vec3(0.0f, 0.0f, -1.0f);
			
			vtx.position = pos + glm::vec3(0.0, 0.0, 1.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(1.0, 0.0, 1.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(0.0, 1.0, 1.0);
			data.push_back(vtx);

			vtx.position = pos + glm::vec3(1.0, 0.0, 1.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(0.0, 1.0, 1.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(1.0, 1.0, 1.0);
			data.push_back(vtx);
			break;

		case BACK:
			vtx.normal = glm::vec3(0.0f, 0.0f, 1.0f);
			
			vtx.position = pos + glm::vec3(0.0, 0.0, 0.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(1.0, 0.0, 0.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(0.0, 1.0, 0.0);
			data.push_back(vtx);

			vtx.position = pos + glm::vec3(1.0, 0.0, 0.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(0.0, 1.0, 0.0);
			data.push_back(vtx);
			vtx.position = pos + glm::vec3(1.0, 1.0, 0.0);
			data.push_back(vtx);
			break;

		default:
			break;
	}
}