#ifndef GAME_H
#define GAME_H

#include <glm/glm.hpp>

#include "world.h"

struct Game {
	World world;
	
	Game();
	~Game();
};


#endif
