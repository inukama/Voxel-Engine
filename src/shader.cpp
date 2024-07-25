
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem> // TODO: Remove redundant dependency.
#include <sstream>

#include "shader.h"

Shader::Shader(const char* vertPath, const char* fragPath)
: ID(0)
{   
    std::string c_path = std::filesystem::current_path().string();
    std::string root = c_path.substr(0,c_path.rfind("/build") + 1);

    // vsc = vertex source
    std::ifstream vsc(root + std::string(vertPath), std::ifstream::in);
    std::stringstream vsc_data;
    vsc_data << vsc.rdbuf();
    std::string vsc_string = vsc_data.str();
    const char* vsc_str = vsc_string.c_str();

    // fsc = fragment source
    std::ifstream fsc(root + std::string(fragPath), std::ifstream::in);
    std::stringstream fsc_data;
    fsc_data << fsc.rdbuf();
    std::string fsc_string = fsc_data.str();
    const char* fsc_str = fsc_string.c_str();
    

    int success;
    char infoLog[512];

    // Compile the vertex and fragment shaders, then check for errors
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource Specifies where the source code is stored
    glShaderSource(vertexShader, 1, &vsc_str, NULL); 
    glCompileShader(vertexShader);

    // Error checking
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Do the same for the fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fsc_str, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // The shader program is what we use to activate the shaders
    ID = glCreateProgram();

    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINKAGE_FAILED\n" << infoLog << std::endl;   
    }

    // Cleanup
    glUseProgram(ID);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() {
    glUseProgram(ID);
}

unsigned int Shader::id() {
    return ID;
}
