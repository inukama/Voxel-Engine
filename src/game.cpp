#include <cmath>
//TODO: REMOVE
#include <iostream>

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

#include "game.h"


Game::~Game() {
	delete(&world);
}
