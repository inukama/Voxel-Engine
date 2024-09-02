#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

#include "noise.h"

#include "world.h"
#include "model.h"

glm::vec<3, int> block_space(glm::vec3 coordinate);
glm::vec<3, int> chunk_space(glm::vec3 coordinate);
glm::vec<3, int> region_space(glm::vec3 coordinate);

void generate_chunk(Chunk *chunk, glm::vec3 chunk_pos);
block_type terrain_generator(glm::vec3 coordinate);

#define DIST 1.0

/*
	Constructors/Destructors
*/

World::World() {
	spdlog::trace("World constructor");
	for (int i = 0; i < MAX_REGIONS; i++) {
		data[i] = nullptr;
	}


	for (int i = 0; i < MAX_REGIONS; i++) {
		if (data[i] == nullptr) {
			spdlog::trace("Allocating centre region");
			data[i] = new Region(glm::vec3(0, 0, 0)); 
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

Region::Region(glm::vec3 coordinate): coordinate(region_space(coordinate)) {
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

Chunk::Chunk(glm::vec3 coordinate) {
    glm::vec<3, int> chunk_pos = chunk_space(coordinate);

	spdlog::debug("Chunk constructor");
	
    glm::vec3 chunk_root = chunk_pos * 16;
	for (int x = 0; x < CHUNK_WIDTH; x++) {
		for (int y = 0; y < CHUNK_WIDTH; y++) {
			for (int z = 0; z < CHUNK_WIDTH; z++) {
				glm::vec3 pos = floor(chunk_root + glm::vec3(x, (CHUNK_WIDTH - 1) - y, z));
				set_block(pos, terrain_generator(pos));
				
				block_type this_block = get_block(pos).type;
				if (this_block == air) {
					continue;
				}

				block_type sample;


				//pos.y++;
				
				//glm::vec3 sample_c = pos + glm::vec3(0.0, -DIST, 0.0);
				//spdlog::debug("Coord: <{}, {}, {}>", sample_c.x, sample_c.y, sample_c.z);
				//spdlog::debug("{}", (int) terrain_generator(sample_c));
				
				// We use 0.9 because if we use 1.0, sometimes floating point errors cause sample to go 2 blocks deep (e.g. 0.99999+ 1.0000001 goes to >2.0)
				sample = terrain_generator(pos + glm::vec3(0.0, DIST, 0.0));
				if (sample == air) {	
					mesh.add_quad(TOP, pos);
				}
				
				sample = terrain_generator(pos + glm::vec3(0.0, -DIST, 0.0));
				if (sample == air) {	
					mesh.add_quad(BOTTOM, pos);
				}
				
				sample = terrain_generator(pos + glm::vec3(0.0, 0.0, DIST));
				if (sample == air) {	
					mesh.add_quad(FRONT, pos);
				}
				
				sample = terrain_generator(pos + glm::vec3(0.0, 0.0, -DIST));
				if (sample == air) {	
					mesh.add_quad(BACK, pos);
				}
				
				sample = terrain_generator(pos + glm::vec3(DIST, 0.0, 0.0));
				if (sample == air) {	
					mesh.add_quad(RIGHT, pos);
				}
				
				sample = terrain_generator(pos + glm::vec3(-DIST, 0.0, 0.0));
				if (sample == air) {	
					mesh.add_quad(LEFT, pos);
				}
				
			}
		}
	}

	mesh.buffer();
}

void Chunk::generate_mesh(glm::vec3 coordinate) {
	// TODO: Implement
}



/*
    START: For chunk generation
*/

block_type terrain_generator(glm::vec3 coordinate) {

	//coordinate.x += 1000.0f;
	//coordinate.z += 1000.0f;

/*
	if (floor(coordinate) == glm::vec3(10.0, 10.0, 10.0)) {
		spdlog::critical("Grass debug");
		return grass;
	} else {
		return air;
	}
*/

	int index = floor(50.0f * perlin2d(floor(coordinate.x), floor(coordinate.z), 0.04, 5));

	if (floor(coordinate.y) < index) {
		return grass;
	} else {
		return air;
	}
}

/*
    END: For chunk generation
*/

void Chunk::mesher() {

}

void Chunk::render() {
	mesh.render();
}

Chunk::~Chunk() {	
	spdlog::trace("Chunk destructor");
}

/*
	Methods
*/

// Return pointer to region 
Region *World::get_region(glm::vec3 coordinate) {
	spdlog::trace("World::get_region");

	glm::vec<3, int> region_pos = region_space(coordinate);

	int first_empty = -1;

	// Iterate through all regions and check whether the coordinat matches
	for (int i = 0; i < MAX_REGIONS; i++) {
		if (data[i] != nullptr && data[i]->coordinate == region_pos) {
			// Return if a match is found
			return data[i];
		} else if (first_empty < 0 && data[i] == nullptr) {
			first_empty = i;
		}
	}

	if (first_empty >= 0) {
		data[first_empty] = new Region(coordinate);
		return data[first_empty];
	}

	// Since a matching region wasn't found, find an empty slot and make one.
	/*
	for (int i = 0; i < MAX_REGIONS; i++) {
		if (data[i] == nullptr) {
			data[i] = new Region(coordinate);
			return data[i];
		}
	}
	*/


	// This will definitely cause an error!
	// We only reach this state if the region array is full
	spdlog::critical("World::get_region::REGION_ARRAY_FULL");
	return nullptr;
}


Chunk *Region::get_chunk(glm::vec3 coordinate) {
    glm::vec<3, int> chunk_pos = chunk_space(coordinate);

	spdlog::trace("Region::get_chunk");
	Chunk *chunk = data[chunk_pos.x][chunk_pos.y][chunk_pos.z];
	
	if (chunk == nullptr) {
		chunk = new Chunk(coordinate);
		data[chunk_pos.x][chunk_pos.y][chunk_pos.z] = chunk;
	}

	return chunk;
}


Chunk *Region::set_chunk(glm::vec3 coordinate, Chunk *chunk) {
    glm::vec<3, int> chunk_pos = chunk_space(coordinate);

	spdlog::trace("Region::set_chunk");
	data[chunk_pos.x][chunk_pos.y][chunk_pos.z] = chunk;	
	return chunk;	
}

Block Chunk::get_block(glm::vec3 coordinate) {
	spdlog::trace("Chunk::get_block");   
	glm::vec<3, int> block_pos = block_space(coordinate);
	
	return data[block_pos.x][block_pos.y][block_pos.z];
}

Block Chunk::set_block(glm::vec3 coordinate, block_type type) {
	spdlog::trace("Chunk::set_block");
	glm::vec<3, int> block_pos = block_space(coordinate);
	data[block_pos.x][block_pos.y][block_pos.z].type = type;

	return data[block_pos.x][block_pos.y][block_pos.z].type;
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

	Region *region = get_region(coordinate);
	Chunk *chunk = region->get_chunk(coordinate);
	Block block = chunk->get_block(coordinate);

	return block;
}

Chunk *World::get_chunk(glm::vec3 coordinate) {
	spdlog::trace("World::get_block");

	Region *region = get_region(coordinate);
	Chunk *chunk = region->get_chunk(coordinate);

	return chunk;
}

glm::vec<3, int> region_space(glm::vec3 coordinate) {
    return floor(coordinate / (float) REGION_WIDTH);
}

glm::vec<3, int> chunk_space(glm::vec3 coordinate) {
	glm::vec<3, int> chunk_index = floor(coordinate / (float) CHUNK_WIDTH);
	chunk_index = (REGION_WIDTH + chunk_index % REGION_WIDTH) % REGION_WIDTH;
    return chunk_index;
}

glm::vec<3, int> block_space(glm::vec3 coordinate) {
	glm::vec<3, int> block_index = floor(coordinate);
	block_index = (CHUNK_WIDTH + block_index % CHUNK_WIDTH) % CHUNK_WIDTH;
    return block_index;
}

// coordinate in 
void World::render(glm::vec3 coordinate) {
	get_chunk(coordinate)->render();
}