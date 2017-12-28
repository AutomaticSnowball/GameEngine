#pragma once

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Shader.h"

struct AnimatedVertex {
	glm::vec3 position;
	glm::vec2 t_coords;
	glm::vec3 normal;
	glm::ivec3 jointIDs;
	glm::vec3 vertexWeights;
};

struct ATexture {
	unsigned int id;
	std::string type; //Difuse/Specular etc.
};

struct AnimatedModelData {
	std::vector<AnimatedVertex> vertex;
	std::vector<unsigned int> indice;
	std::vector<ATexture> texture;
};

class AnimatedMesh
{
public:
	AnimatedMesh();
	AnimatedMesh(AnimatedModelData md);
	std::vector<AnimatedVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<ATexture> textures;
	void draw(Shader shader);
	~AnimatedMesh();
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();

};

