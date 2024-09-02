#ifndef WORLD_H
#define WORLD_H

#include <glm/glm.hpp>

#include "model.h"

#define MAX_REGIONS 100000
#define REGION_WIDTH 32
#define CHUNK_WIDTH 16

enum block_type {
	air,
	grass,
	uninitialised = -1,
};


class Block {
	public:
		block_type type;
		
		Block(block_type type): type(type) {}
		Block(): type(uninitialised) {}

	private:
		//

};


class Chunk {
	public:	
		Block get_block(glm::vec3 coordinate);
		Block set_block(glm::vec3 coordinate, block_type type);

		void generate_mesh(glm::vec3 coordinate);

		void render();
		void mesher();
	
		Chunk(glm::vec3 coordinate);
		~Chunk();
		
	private:
		Block data[CHUNK_WIDTH][CHUNK_WIDTH][CHUNK_WIDTH];
		Model mesh;
	
};


class Region {
	public:
		// Coordinate is given in region space (i.e. regions around spawn have
		// coordinates (1,0,0), (0,1,0), (1,1,0), (0,0,0)).
		glm::vec<3, int> coordinate;
			
		Chunk *get_chunk(glm::vec3 coordinate);
		Chunk *set_chunk(glm::vec3 coordinate, Chunk *chunk);
		
		Region(glm::vec3 coordinate);
		~Region();

	private:
		Chunk *data[REGION_WIDTH][REGION_WIDTH][REGION_WIDTH];

};


// We don't really want to work with chunks and regions from the outside.
// Instead we want to directly access blocks.
class World {
	public:
		Block get_block(glm::vec3 coordinate);
		Block set_block(glm::vec3 coordinate, block_type type);	
		
		Chunk *get_chunk(glm::vec3 coordinate);
		
		Region *get_region(glm::vec3 coordinate);

		void render(glm::vec3 coordinate);

		World();
		~World();
	
	private:	
		Region *data[MAX_REGIONS];	
};



#endif