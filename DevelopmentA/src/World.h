#pragma once

#include "Chunk.h"
#include <cstdlib>

class World
{
public:
	World();
	Chunk getChunk(int x, int z);
	unsigned int getBlock(int x, int y, int z);
	void generate();
	Chunk genChunk(int x, int z);
	void setSize();
	~World();
private:
	std::vector<Chunk> chunks;
	std::vector<int> worldSize;
};

