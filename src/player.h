#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>

struct Entity {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 rotation;

    Entity();
};

struct Player {
    Entity body;
    Entity camera;
	glm::vec3 hitbox; 

    //Player();
};

#endif