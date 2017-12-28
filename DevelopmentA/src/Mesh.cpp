#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::Mesh(ModelData info)
{
	processModelData(info);
	setupMesh();
}

void Mesh::processModelData(ModelData info) {
	std::vector<Vertex> v;
	std::vector<unsigned int> i;
	unsigned int currentIndex = 0;
	for each ( glm::vec3 index in info.indices) {
		Vertex newV;
		newV.position = info.positions[index.x];
		if (index.y == 5000) {
			newV.t_coords = glm::vec2(0.0f, 0.0f);
		}
		else {
			newV.t_coords = info.textureCoords[index.y];
		}
		newV.normal = info.normals[index.z];
		v.push_back(newV);
		i.push_back(currentIndex);
		currentIndex++;
	}
	vertices = v;
	indices = i;
	textures = info.texture;
}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//Vertex Pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	//Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	//Textures
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, t_coords));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}


void Mesh::draw(Shader shader) {
	unsigned int diffuseID = 0;
	unsigned int specularID = 0;
	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].type;
		if (name == "tDiffuse") {
			number = std::to_string(diffuseID++);
		}
		else if (name == "tSpecular") {
			number = std::to_string(specularID++);
		}
		else {
			std::cout << "No texture type by the name of " << name << std::endl;
		}

		shader.setI(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Mesh::drawInstanced(Shader shader, int count) {
	unsigned int diffuseID = 0;
	unsigned int specularID = 0;
	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].type;
		if (name == "tDiffuse") {
			number = std::to_string(diffuseID++);
		}
		else if (name == "tSpecular") {
			number = std::to_string(specularID++);
		}
		else {
			std::cout << "No texture type by the name of " << name << std::endl;
		}

		shader.setI(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, count);
	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


Mesh::~Mesh() {

}