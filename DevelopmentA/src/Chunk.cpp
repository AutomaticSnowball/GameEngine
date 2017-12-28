#include "Chunk.h"


Chunk::Chunk()
{
	for (int y = 0; y < 2; y++) {
		for (int x = 0; x < 16; x++) {
			for (int z = 0; z < 16; z++) {
				blocks[x][y][z] = 0;
			}
		}
	}
	start_x = 0;
	start_z = 0;
}

void Chunk::SetBlock(int x, int y, int z, int type) {
	blocks[x][y][z] = type;
}

unsigned int Chunk::getBlock(int x, int y, int z) {
	try {
		return blocks[x][y][z];
	}
	catch (...) {
		return 0;
	}
}

void Chunk::assignStart(int x, int z) {
	start_x = x;
	start_z = z;
}

std::vector<int> Chunk::getStart() {
	std::vector<int> output;
	output.push_back(start_x);
	output.push_back(start_z);
	return output;
}


Chunk::~Chunk()
{
}
