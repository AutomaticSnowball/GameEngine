#pragma once
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Shader.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 t_coords;
};

struct Texture {
	unsigned int id;
	std::string type; //Difuse/Specular etc.
};

struct ModelData {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::vector<glm::vec3> indices;
	std::vector<Texture> texture;
};

class Mesh
{
public:
	Mesh();
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	void processModelData(ModelData info);
	Mesh(ModelData info);
	void draw(Shader shader);
	void drawInstanced(Shader shader, int count);
	~Mesh();
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

