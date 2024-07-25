#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebufferSizeCallback( GLFWwindow* window, int width, int height ) {
    glViewport(0, 0, width, height);
}

void registerCallbacks( GLFWwindow* window ) {
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

