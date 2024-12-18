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

#include "callbacks.h"
#include "inputs.h"
#include "shader.h"
#include "game.h"
#include "player.h"

#define WIDTH 1200
#define HEIGHT 600

#define CHUNK_SIZE 16

#define CHUNK_RENDER_DISTANCE 10

#define ASPECT ((float) WIDTH)/((float) HEIGHT)
#define DIMS glm::vec<2, double>(WIDTH, HEIGHT);


// Function declarations
float clamp(float input, float min, float max);
GLFWwindow* initialise(void);


int main(void) {
	// Logger setup
	spdlog::set_level(spdlog::level::info);
	//spdlog::set_level(spdlog::level::trace);

	// Initialise window and create program
	GLFWwindow* window = initialise();
	if (window == nullptr) return -1;

	Shader program = Shader("resources/shader.vs", "resources/shader.fs");
	program.use();

	// TODO: create timekeeping class
	const double time_0 = glfwGetTime();
	double time_delta = 0.0;
	double time_before = time_0;
	double time_after;

	// Set up transformation matrices and other things
	glm::mat4 mat_projection = glm::perspective(glm::radians(45.0f), ASPECT, 0.1f, 10000.0f);
	unsigned int u_projection = glGetUniformLocation(program.id(), "projection");
	glUniformMatrix4fv(u_projection, 1, false, (const float*) &mat_projection[0]);


	// Begin generating world data
	spdlog::info("Creating World object");
	World map;	

	spdlog::info("Generating starter chunk");

	float frame_count = 0.0;

	glm::vec<2, double> mouse;
	glm::vec<2, double> mouse_before;
	glm::vec<2, double> mouse_delta;

	// Spawn pos
	Player player;
	player.body.position = glm::vec3(0.0, 0.0, 0.0);
	player.body.velocity = glm::vec3(0.0, 0.0, 0.0);

	spdlog::info("Starting render loop");
	while ( !glfwWindowShouldClose(window) )
	{

		// Transform mouse from pixels to NDC
		mouse = 2.0*mouse/DIMS;
		mouse.x = mouse.x - 1.0;
		mouse.y = 1.0 - mouse.y;
		
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

		movement_direction = glm::normalize(movement_direction);
		if (isnan(movement_direction.x)) {
			movement_direction = glm::vec3(0.0f);
		}
		
		glm::mat4 mat_view(1);
		mat_view = glm::rotate(mat_view, player.camera.rotation.x, glm::vec3(1.0, 0.0, 0.0));
		mat_view = glm::rotate(mat_view, player.camera.rotation.y, glm::vec3(0.0, 1.0, 0.0));
		mat_view = glm::rotate(mat_view, player.camera.rotation.z, glm::vec3(0.0, 0.0, 1.0));

		movement_direction = glm::rotateX(movement_direction, player.camera.rotation.x);
		movement_direction = glm::rotateY(movement_direction, (float) M_PI - player.camera.rotation.y);
		
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			movement_direction += glm::vec3(0.0, 1.0, 0.0);
		}
		
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			movement_direction += glm::vec3(0.0, -1.0, 0.0);
		}

		player.body.position += movement_direction * 8.0f * (float) time_delta;
		
		player.camera.rotation.x += 0.005f*mouse_delta.y;
		player.camera.rotation.y += 0.005f*mouse_delta.x;
		player.camera.rotation.x = clamp(player.camera.rotation.x, -M_PI_2, M_PI_2);
		
		mat_view = glm::translate(mat_view, -(player.body.position + player.camera.position));
		unsigned int u_view = glGetUniformLocation(program.id(), "view");
		glUniformMatrix4fv(u_view, 1, false, (const float*) &mat_view[0]);


		processInputs(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Game logic
		
		// Camera rotation and translation stuff
		
		program.use();

		for (int x = -CHUNK_RENDER_DISTANCE; x < CHUNK_RENDER_DISTANCE; x++) {
			for (int y = -CHUNK_RENDER_DISTANCE; y < CHUNK_RENDER_DISTANCE; y++) {
				for (int z = -CHUNK_RENDER_DISTANCE; z < CHUNK_RENDER_DISTANCE; z++) {
					glm::vec3 pos = ((float) CHUNK_SIZE ) * glm::vec3(x, y, z);
					pos +=  player.body.position;
					
					map.render(pos);
				}
			}
		}
	
		

		glfwSwapBuffers(window);
		glfwPollEvents();	

		if (1/time_delta < 50.0) {
			static int slow_count = 0;
			slow_count += 1.0;
			
			spdlog::debug("FPS: {}", 1/time_delta);
			spdlog::debug("{}% of frames are slower than 50fps", slow_count/frame_count*100.0);	
		}
		
		//spdlog::info("FPS: {}", 1/time_delta);
		//spdlog::debug("Coordinate: <{}, {}, {}>", player.body.position.x, player.body.position.y, player.body.position.z);

		frame_count += 1.0;
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

	GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT, "Voxel Game", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create window with GLFW" << std::endl;
		glfwTerminate();
		return window;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
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
