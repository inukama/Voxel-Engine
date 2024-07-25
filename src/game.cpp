#include <cmath>
//TODO: REMOVE
#include <iostream>

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

#include "game.h"


/*
	Constructors/Destructors
*/

Game::~Game() {
	delete(&world);
}


World::World() {
	spdlog::trace("World constructor");
	for (int i = 0; i < MAX_REGIONS; i++) {
		data[i] = nullptr;
	}


	for (int i = 0; i < MAX_REGIONS; i++) {
		if (data[i] == nullptr) {
			spdlog::trace("Allocating centre region");
			data[i] = new Region(glm::vec<3, int>(0, 0, 0)); 
			break;
		}
	}
}

World::~World() {
	spdlog::trace("World destructor");
	for (int region = 0; region < MAX_REGIONS; region++) {
		if (data[region] != nullptr) {
			delete(data[region]);	
		}	
	}
}

Region::Region(glm::vec<3, int> coordinate): coordinate(coordinate) {
	spdlog::trace("Region constructor");

	int i = 0;
	for (int x = 0; x < REGION_WIDTH; x++) {
		for (int y = 0; y < REGION_WIDTH; y++) {
			for (int z = 0; z < REGION_WIDTH; z++) {
				data[x][y][z] = nullptr;
				i++;
			}
		}
	}
	spdlog::trace("Chunk count: {}", i);
}

Region::~Region() {
	spdlog::trace("Region destructor");
	for (int x = 0; x < REGION_WIDTH; x++) {
		for (int y = 0; y < REGION_WIDTH; y++) {
			for (int z = 0; z < REGION_WIDTH; z++) {
				if (data[x][y][z] != nullptr) {
					delete(data[x][y][z]);
				}
			}
		}
	}
}

Chunk::Chunk() {
	spdlog::trace("Chunk constructor");
	for (int x = 0; x < CHUNK_WIDTH; x++) {
		for (int y = 0; y < CHUNK_WIDTH; y++) {
			for (int z = 0; z < CHUNK_WIDTH; z++) {
				data[x][y][z] = uninitialised;	
			}
		}
	}
}

Chunk::~Chunk() {	
	spdlog::trace("Chunk destructor");
}

/*
	Methods
*/

// Return pointer to region 
Region *World::get_region(glm::vec<3, int> coordinate) {
	spdlog::trace("World::get_region");

	// Iterate through all regions and check whether the coordinat matches
	for (int i = 0; i < MAX_REGIONS; i++) {
		if (data[i] != nullptr && data[i]->coordinate == coordinate) {
			// Return if a match is found
			return data[i];
		}
	}

	// Since a matching region wasn't found, find an empty slot and make one.
	for (int i = 0; i < MAX_REGIONS; i++) {
		if (data[i] == nullptr) {
			data[i] = new Region(coordinate);
			return data[i];
		}
	}


	// This will definitely cause an error!
	// We only reach this state if the region array is full
	spdlog::error("World::get_region::REGION_ARRAY_FULL");
	return nullptr;
}

Chunk *Region::get_chunk(glm::vec<3, int> coordinate) {
	spdlog::trace("Region::get_chunk");
	Chunk *chunk = data[coordinate.x][coordinate.y][coordinate.z];
	
	if (chunk == nullptr) {
		chunk = new Chunk();
		data[coordinate.x][coordinate.y][coordinate.z] = chunk;
	}

	return chunk;
}

Chunk *Region::set_chunk(glm::vec<3, int> coordinate, Chunk *chunk) {
	spdlog::trace("Region::set_chunk");
	data[coordinate.x][coordinate.y][coordinate.z] = chunk;	
	return chunk;	
}

Block Chunk::get_block(glm::vec<3, int> coordinate) {
	spdlog::trace("Chunk::get_block");
	return data[coordinate.x][coordinate.y][coordinate.z];
}

Block Chunk::set_block(glm::vec<3, int> coordinate, block_type type) {
	spdlog::trace("Chunk::set_block");
	data[coordinate.x][coordinate.y][coordinate.z].type = type;

	return data[coordinate.x][coordinate.y][coordinate.z];

}

Block World::set_block(glm::vec3 coordinate, block_type type) {
	spdlog::trace("World::set_block");
	// Get coordinates 
	glm::vec<3, int> region_coordinate = floor(coordinate / (float) REGION_WIDTH);
	
	glm::vec<3, int> chunk_index = floor(coordinate / (float) CHUNK_WIDTH);
	chunk_index =  (REGION_WIDTH + chunk_index % REGION_WIDTH) % REGION_WIDTH;
	
	glm::vec<3, int> block_index = floor(coordinate);
	block_index = (CHUNK_WIDTH + block_index % CHUNK_WIDTH) % CHUNK_WIDTH;

	// Traverse world
	Region *region = get_region(region_coordinate);
	Chunk *chunk = region->get_chunk(chunk_index);
	Block block = chunk->set_block(block_index, type);

	return block;
}

// Find the correct index for fetching a block in region, chunk, and then block
// coordinate resolution.
Block World::get_block(glm::vec3 coordinate) {
	spdlog::trace("World::get_block");
	glm::vec<3, int> region_coordinate = floor(coordinate / (float) REGION_WIDTH);

	
	glm::vec<3, int> chunk_index = floor(coordinate / (float) CHUNK_WIDTH);
	chunk_index =  (REGION_WIDTH + chunk_index % REGION_WIDTH) % REGION_WIDTH;
	
	glm::vec<3, int> block_index = floor(coordinate);
	block_index = (CHUNK_WIDTH + block_index % CHUNK_WIDTH) % CHUNK_WIDTH;
	
	glm::vec3 check = block_index;

	// Traverse world
	Region *region = get_region(region_coordinate);
	Chunk *chunk = region->get_chunk(chunk_index);
	Block block = chunk->get_block(block_index);

	return block;
}