#ifndef GL
#define GL
	#include <glad/glad.h>
	#include <GLFW/glfw3.h>
#endif

#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>

#include "spdlog/spdlog.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "noise.h"

#include "callbacks.h"
#include "inputs.h"
#include "shader.h"
#include "game.h"

#define WIDTH 1200
#define HEIGHT 600

#define CHUNK_SIZE 16

#define ASPECT ((float) WIDTH)/((float) HEIGHT)
#define DIMS glm::vec<2, double>(WIDTH, HEIGHT);

#define MAX_BLOCK_COUNT 60000

#define JUMP_POWER 15.0


// Function declarations
float clamp(float input, float min, float max);
GLFWwindow* initialise(void);


int main(void) {
	spdlog::set_level(spdlog::level::debug);

	GLFWwindow* window = initialise();
	if (window == nullptr) return -1;

	Shader program = Shader("resources/shader.vs", "resources/shader.fs");
	program.use();

	// TODO: create timekeeping class
	const double time_0 = glfwGetTime();
	double time_delta = 0.0;
	double time_before = time_0;
	double time_after;

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	spdlog::info("Generating buffers");
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);	

	spdlog::info("Buffering cube model");
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int width;
	int height;
	int nrChannels;
	// ROOT is a preprocessor directive filled by xmake
	std::string root_dir = ROOT;
	std::string img_dir = "/resources/textures/grass.jpg";
	std::string full_img_dir = root_dir + img_dir;
	spdlog::info("Loading image data from directory: {}", full_img_dir);
	float *image_data = stbi_loadf(full_img_dir.c_str(), &width, &height, &nrChannels, 0);
	
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (image_data && nrChannels == 3) {
		spdlog::info("Buffering image data");
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, image_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		spdlog::error("Failed to load texture");
	}	

	stbi_image_free(image_data);

	glm::mat4 mat_projection = glm::perspective(glm::radians(45.0f), ASPECT, 0.1f, 100.0f);
	unsigned int u_projection = glGetUniformLocation(program.id(), "projection");
	glUniformMatrix4fv(u_projection, 1, false, (const float*) &mat_projection[0]);

	glm::vec3 camera_position = glm::vec3(0.0, 0.0, 0.0);
	// Camera position relative to player centre.
	glm::vec3 camera_rel_position = glm::vec3(0.0, 0.75, 0.0);
	glm::vec3 player_position = glm::vec3(0.0, 0.0, 0.0);
	// Spindle collision detection
	glm::vec3 hitbox = glm::vec3(0.6, 1.8, 0.6); 
	// Each component represents the rotation in degrees around that axis
	glm::vec3 camera_rotation = glm::vec3(0.0, 0.0, 0.0);	

	spdlog::info("Creating World object");
	World map;	

	spdlog::info("Generating starter chunk");
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				spdlog::debug("{}",round(20.0f * perlin2d(x, z, 0.04, 5)) );
				if(y > round(20.0f * perlin2d(x, z, 0.04, 5))) {
					map.set_block(glm::vec3(x, y, z), grass);
				} else {
					map.set_block(glm::vec3(x, y, z), air);
				}
			}
		}
	}

	float frame_count = 0.0;

	glm::vec<2, double> mouse;
	glm::vec<2, double> mouse_before;
	glm::vec<2, double> mouse_delta;

	// Spawn pos
	player_position = glm::vec3(0.0, 40.0, 0.0);
	camera_position = player_position + camera_rel_position;
	glm::vec3 floor_height = player_position;
	glm::vec3 player_gravity = glm::vec3(0.0, -30.0, 0.0);
	glm::vec3 player_velocity = glm::vec3(0.0, 0.0, 0.0);
	bool grounded;

	spdlog::info("Starting render loop");
	while ( !glfwWindowShouldClose(window) )
	{
		// Update time, cursor position and transform cursor to useful coordinates
		time_after = glfwGetTime();
		time_delta = glfwGetTime() - time_before;
		time_before = time_after;

		glfwGetCursorPos(window, &mouse.x, &mouse.y);
		mouse_delta = mouse - mouse_before;
		mouse_before = mouse;

		glm::vec3 movement_direction = glm::vec3(0.0, 0.0, 0.0);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			movement_direction += glm::vec3(0.0, 0.0, 1.0);
		} 

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			movement_direction += glm::vec3(1.0, 0.0, 0.0);
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			movement_direction += glm::vec3(0.0, 0.0, -1.0);
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			movement_direction += glm::vec3(-1.0, 0.0, 0.0);
		}

		if (grounded && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			player_velocity.y = JUMP_POWER;
			grounded = false;
		}

		movement_direction = glm::normalize(movement_direction);
		if (isnan(movement_direction.x)) {
			movement_direction = glm::vec3(0.0f);
		}

		movement_direction = glm::rotateY(movement_direction, (float) M_PI - camera_rotation.y);

		// Transform mouse from pixels to NDC
		mouse = 2.0*mouse/DIMS;
		mouse.x = mouse.x - 1.0;
		mouse.y = 1.0 - mouse.y;

		processInputs(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Game logic
		
		// Camera rotation and translation stuff
		
		camera_rotation.x += 0.005f*mouse_delta.y;
		camera_rotation.y += 0.005f*mouse_delta.x;

		camera_rotation.x = clamp(camera_rotation.x, -M_PI_2, M_PI_2);
		
		program.use();

		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int y = 0; y < CHUNK_SIZE; y++) {
				for (int z = 0; z < CHUNK_SIZE; z++) {
					glm::vec3 pos = glm::vec3(x, y, z);
					glm::mat4 mat_model(1);	
					mat_model = glm::translate(mat_model, pos);
					
					unsigned int u_model = glGetUniformLocation(program.id(), "model");
					glUniformMatrix4fv(u_model, 1, false, (const float*) &mat_model[0]);

					glBindTexture(GL_TEXTURE_2D, texture);
					glBindVertexArray(VAO);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
		} 
		
		glm::vec3 temp_floor_height = glm::vec3(-100000.0f);
		glm::vec<3, int> rd_player_pos = floor(player_position - hitbox/2.0f + 0.5f);

		// TEMPORARY	
		glm::vec3 fpos = floor(player_position - hitbox/2.0f + 0.5f);
		glm::mat4 mat_model(1);	
		mat_model = glm::translate(mat_model, fpos);
		mat_model = glm::scale(mat_model, glm::vec3(1.1, 1.1, 1.1));
		
		unsigned int u_model = glGetUniformLocation(program.id(), "model");
		glUniformMatrix4fv(u_model, 1, false, (const float*) &mat_model[0]);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// TEMPORARY

		glm::vec3 movement_mask = glm::vec3(1, 1, 1);

		//spdlog::debug("Falling on type {} @ ({}, {}, {})", (int) map.get_block(rd_player_pos).type, rd_player_pos.x, rd_player_pos.y, rd_player_pos.z);
		glm::vec3 check = player_position;
		//spdlog::debug("@ ({}, {}, {})", check.x, check.y, check.z);
		if (map.get_block(fpos).type == grass) {
			player_position.y = rd_player_pos.y + hitbox.y/2.0f + 0.5f;
			player_velocity.y = 0.0f;
			grounded = true;
			movement_mask.y = 0;
		} else {
			grounded = false;
		}

		player_position += movement_direction * 8.0f * (float) time_delta;
		glm::vec3 player_position_before = player_position;
		player_velocity += player_gravity * (float) time_delta;
		player_position += player_velocity * movement_mask * (float) time_delta;
		
		camera_position = player_position + camera_rel_position;

		glm::mat4 mat_view(1);
		mat_view = glm::rotate(mat_view, camera_rotation.x, glm::vec3(1.0, 0.0, 0.0));
		mat_view = glm::rotate(mat_view, camera_rotation.y, glm::vec3(0.0, 1.0, 0.0));
		mat_view = glm::rotate(mat_view, camera_rotation.z, glm::vec3(0.0, 0.0, 1.0));
		mat_view = glm::translate(mat_view, -camera_position);

		unsigned int u_view = glGetUniformLocation(program.id(), "view");
		glUniformMatrix4fv(u_view, 1, false, (const float*) &mat_view[0]);

		// Equivalent of swapping buffers
		glfwSwapBuffers(window);
		glfwPollEvents();	

		if (1/time_delta < 50.0) {
			static int slow_count = 0;
			slow_count += 1.0;
			
			spdlog::debug("FPS: {}", 1/time_delta);
			spdlog::debug("{}% of frames are slower than 50fps", slow_count/frame_count*100.0);	
		}

		frame_count += 1.0;
		player_position_before = player_position;
	}

	glfwTerminate();
	return 0;
}


// Initialise GLFW window and set GL params

// Return: GLFW window object
GLFWwindow* initialise(void)
{
	std::cout << "Initialising GLFW" << std::endl;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT, "Car Simulation", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create window with GLFW" << std::endl;
		glfwTerminate();
		return window;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	std::cout << "Initialising GLAD" << std::endl;
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialise GLAD" << std::endl;
		window = NULL;
		return window;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	stbi_set_flip_vertically_on_load(true);  

	return window;
}


// clamps input between min and max values

// e.g. clamp(1.1f, 0.0f, 1.0f) == 1.0f;
float clamp(float input, float min, float max) {
	if (input >= max) {
		return max;
	}
	
	if (input <= min) {
		return min;
	}

	return input; 
}
