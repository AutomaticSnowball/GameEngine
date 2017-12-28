#pragma once
#include "Shader.h"
#include "Mesh.h"

#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <stdlib.h>


class Model
{
public:
	Model(char *path);
	Model(char *path, std::vector<std::string> a);
	Model();
	void draw(Shader shader);
	void drawInstanced(Shader shader, int count);
	std::vector<std::string> getTextures(std::string path);
	~Model();
private:
	std::vector<Texture> curTextures;
	std::vector<Mesh> meshes;
	void loadModel(std::string path);
	Texture genTexture(std::string path);
};

