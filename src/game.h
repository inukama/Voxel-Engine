#ifndef GAME_H
#define GAME_H

#include <glm/glm.hpp>

#define MAX_REGIONS 1000
#define REGION_WIDTH 32
#define CHUNK_WIDTH 16

enum block_type {
	air,
	grass,
	uninitialised = -1,
};


struct Block {
	public:
		block_type type;
		
		Block(block_type type): type(type) {}
		Block(): type(uninitialised) {}

	private:
		//

};


struct Chunk {
	public:
		Block get_block(glm::vec<3, int> coordinate);
		Block set_block(glm::vec<3, int> coordinate, block_type type);
	
		Chunk();
		~Chunk();

	private:
		Block data[CHUNK_WIDTH][CHUNK_WIDTH][CHUNK_WIDTH];
	
};


struct Region {
	public:
		// Coordinate is given in region space (i.e. regions around spawn have
		// coordinates (1,0,0), (0,1,0), (1,1,0), (0,0,0)).
		glm::vec<3, int> coordinate;
			
		Chunk *get_chunk(glm::vec<3, int> coordinate);
		Chunk *set_chunk(glm::vec<3, int> coordinate, Chunk *chunk);
		
		Region(glm::vec<3, int> coordinate);
		~Region();

	private:
		Chunk *data[REGION_WIDTH][REGION_WIDTH][REGION_WIDTH];

};


// We don't really want to work with chunks and regions from the outside.
// Instead we want to directly access blocks.
struct World {
	public:
		Block get_block(glm::vec3 coordinate);
		Block set_block(glm::vec3 coordinate, block_type type);	

		World();
		~World();
	
	private:	
		Region *data[MAX_REGIONS];	
		Region *get_region(glm::vec<3, int> coordinate);
};


struct Game {
	World world;
	
	Game();
	~Game();
};


#endif
