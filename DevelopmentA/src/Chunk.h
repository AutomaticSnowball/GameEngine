#pragma once
#include <vector>

class Chunk
{
public:
	Chunk();
	~Chunk();
	void SetBlock(int x, int y, int z, int type);
	unsigned int getBlock(int x, int y, int z);
	void assignStart(int x, int y);
	std::vector<int> getStart();
private:
	unsigned int blocks[16][2][16];
	int start_x = 0;
	int start_z = 0;
};

