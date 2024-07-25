#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

class Shader {
    public:
        Shader(const char* vertPath, const char* fragPath);
        void use(void);
        unsigned int id(void);

    private:
        unsigned int ID;
};

#endif