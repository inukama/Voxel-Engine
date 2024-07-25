#include <glad/glad.h>
#include <GLFW/glfw3.h>


// Function to process inputs which are relevant to many applications (application agnostic)
void processInputs(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}