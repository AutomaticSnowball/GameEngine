#include "World.h"



World::World()
{
	Chunk curChunk;
	for (int x = -5; x < 6; x++) {
		for (int z = -5; z < 6; z++) {
			curChunk.assignStart(x, z);
			chunks.push_back(curChunk);
		}
	}
}

void World::setSize() {
	int minX = 0;
	int minZ = 0;
	int maxX = 0;
	int maxZ = 0;
	for each (Chunk c in chunks) {
		if (c.getStart()[0] < minX) {
			minX = c.getStart()[0];
		}
		else if (c.getStart()[0] > maxX) {
			maxX = c.getStart()[0];
		}
		if (c.getStart()[1] < minZ) {
			minZ = c.getStart()[1];
		}
		else if (c.getStart()[1] > maxZ) {
			maxZ = c.getStart()[1];
		}
	}
	worldSize.push_back(minX);
	worldSize.push_back(maxX);
	worldSize.push_back(minZ);
	worldSize.push_back(maxZ);
}


void World::generate() {

	std::vector<Chunk> newChunks;
	for each (Chunk c in chunks) {
		newChunks.push_back(genChunk(c.getStart()[0], c.getStart()[1]));
	}
	chunks = newChunks;
}

Chunk World::genChunk(int x, int z) {
	Chunk a;
	a.assignStart(x, z);
	for (int y = 0; y < 1; y++) {
		for (int x = 0; x < 16; x++) {
			for (int z = 0; z < 16; z++) {
				a.SetBlock(x, y, z, 1);
				if (rand() % 6 == 0) {
					a.SetBlock(x, y, z, 2);
				}
				else if (rand() % 6 == 0) {
					a.SetBlock(x, y, z, 2);
				}
			}
		}
	}
	return a;
}

Chunk World::getChunk(int x, int z) {
	for each (Chunk a in chunks ) {
		if (a.getStart()[0] == x) {
			if (a.getStart()[1] == z) {
				return a;
			}
		}
	}
	Chunk defaultChunk = genChunk(x, z);
	chunks.push_back(defaultChunk);
	return defaultChunk;
}

unsigned int World::getBlock(int x, int y, int z) {
	return getChunk(x / 16, z / 16).getBlock(x / 16, y, z / 16);
}

World::~World()
{
}
