#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>

enum Face {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    FRONT,
    BACK
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

class Model {
    public:
        glm::vec3 position;
        
        void render();
        void buffer();

        void add_quad(enum Face face, glm::vec3 root_position);
        
        Model();
        //~Model();
        
        std::vector<Vertex> data;
    private:
        unsigned int VAO;
        unsigned int VBO;
};


#endif