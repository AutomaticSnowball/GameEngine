#include "Model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Model::Model()
{
	loadModel("src\\models\\block.obj");
}

Model::Model(char *path)
{
	loadModel(path);
}


Model::Model(char *path, std::vector<std::string> a)
{
	for each (std::string s in a) {
		curTextures.push_back(genTexture(s));
	}
	loadModel(path);
}

void Model::draw(Shader shader) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shader);

	}
}

void Model::drawInstanced(Shader shader, int count) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].drawInstanced(shader, count);

	}
}

void Model::loadModel(std::string path) {
	//Only 1 mesh!
	std::fstream obj;
	obj.open(path);

	std::string line;
	std::vector<glm::vec3> indices;
	std::vector<glm::vec3> normalStorage;
	std::vector<glm::vec2> textureStorage;
	std::vector<glm::vec3> posStorage;

	std::string meshMaterial = "";

	while (std::getline(obj, line)) {
		std::istringstream iss(line);
	
		if (line.find("v ") != std::string::npos) {
			std::string newLine;
			newLine = line.replace(0, 2, "") + " ";
			std::string token;
			unsigned int pos = 0;
			unsigned int vectorPos = 0;
			glm::vec3 vertex(0.0f, 0.0f, 0.0f);
			while ((pos = newLine.find(" ")) != std::string::npos) {
				token = newLine.substr(0, pos);
				newLine.erase(0, pos + 1);
				if (vectorPos == 0) {
					vertex.x = atof(token.c_str());
				}
				else if (vectorPos == 1) {
					vertex.y = atof(token.c_str());
				}
				else if (vectorPos == 2) {
					vertex.z = atof(token.c_str());
					posStorage.push_back(vertex);
				}
				vectorPos++;
			}
		}
		else if (line.find("vn ") != std::string::npos) {
			std::string newLine;
			newLine = line.replace(0, 3, "") + " ";
			std::string token;
			unsigned int pos = 0;
			unsigned int vectorNorm = 0;
			glm::vec3 normalPos;
			while ((pos = newLine.find(" ")) != std::string::npos) {
				token = newLine.substr(0, pos);
				newLine.erase(0, pos + 1);
				if (vectorNorm == 0) {
					normalPos.x = atof(token.c_str());
				}
				else if (vectorNorm == 1) {
					normalPos.y = atof(token.c_str());
				}
				else if (vectorNorm == 2) {
					normalPos.z = atof(token.c_str());
					normalStorage.push_back(normalPos);
				}
				vectorNorm++;
			}
		}
		else if (line.find("vt ") != std::string::npos) {
			std::string newLine;
			newLine = line.replace(0, 3, "") + " ";

			std::string token;
			unsigned int pos = 0;
			unsigned int textCoord = 0;
			glm::vec2 texCoord;
			while ((pos = newLine.find(" ")) != std::string::npos) {
				token = newLine.substr(0, pos);
				newLine.erase(0, pos + 1);
				float ref = atof(token.c_str());
				if (textCoord == 0) {
					texCoord.x = ref;
				}
				else if (textCoord == 1) {
					texCoord.y = ref;
				}
				textCoord += 1;
			}
			textureStorage.push_back(texCoord);
		}
		else if (line.find("f ") != std::string::npos) {
			std::string newLine = line.replace(0, 2, "") + " ";
			std::string token;
			unsigned int pos = 0;
			while ((pos = newLine.find(" ")) != std::string::npos) {
				token = newLine.substr(0, pos) + "/";
				newLine.erase(0, pos + 1);
				unsigned int eboPos = 0;
				unsigned int type = 0;
				std::string vertexPoint;
				glm::vec3 index;
				while ((eboPos = token.find("/")) != std::string::npos) {
					std::string token2;
					token2 = token.substr(0, eboPos);
					token.erase(0, eboPos + 1);
					vertexPoint = token2;
					unsigned int ref = atoi(vertexPoint.c_str()) - 1;
					if (type == 0) {
						//Index
						index.x = ref;
					}
					else if (type == 1) {
						//Texture
						if (ref + 1 == 0) { ref = 5000; }
						index.y = ref;
					}
					else if (type == 2) {
						//Normal
						index.z = ref;
						indices.push_back(index); 
					}
					type++;
				}
			}
		} 
		else if (line.find("mtllib ") != std::string::npos) {
			std::string materialLine = line.replace(0, 7, "");
			meshMaterial = materialLine;
		}
		else if (line.find("usemtl ") != std::string::npos) {
			std::string materialLine = line.replace(0, 7, "");
			if (materialLine != "None") {

			}
		}
		else {
			//Other data from the obj file.
		}
	}
	std::vector<Texture> t;


	if (curTextures.size() > 0) {
		for each (Texture b in curTextures) {
			t.push_back(b);
		}
	} 
	else {
		for each (std::string a in getTextures("src\\models\\" + meshMaterial)) {
			Texture tempText = genTexture(a);
			t.push_back(tempText);
		}
	}

	Texture tempText = genTexture("grass_diffuse.png");
	t.push_back(tempText);
	ModelData model;
	model.indices = indices;
	model.normals = normalStorage;
	model.positions = posStorage;
	model.textureCoords = textureStorage;
	model.texture = t;
	meshes.push_back(Mesh(model));

	obj.close();
}

std::vector<std::string> Model::getTextures(std::string path) {
	std::vector<std::string> textO;
	std::fstream mtl;
	mtl.open(path);
	std::string f_line;
	while (std::getline(mtl, f_line)) {
		std::istringstream iss(f_line);
		if (f_line.find("map_Kd ") != std::string::npos) {
			std::string texturePath = f_line.replace(0, 7, "");
			if (texturePath == ".") {
				textO.push_back("grass_diffuse.png"); //Default texture.
				textO.push_back("grass_specular.png"); //Default texture.
			} 
			else {
				textO.push_back(texturePath);
				if (f_line.find("_diffuse") != std::string::npos) {
					std::string n = texturePath.replace(texturePath.length() - 12, 12, "") + "_specular.png";
					std::ifstream hasSpecular("src\\textures\\" + n);
					if (hasSpecular.good()) {
						textO.push_back(n);
					}
 					hasSpecular.close();
				}
			}
		}
	}
	if (textO.size() == 0) {
		textO.push_back("grass_diffuse.png"); //Default texture.
		textO.push_back("grass_specular.png"); //Default texture.
	}
	mtl.close();
	return textO;
}


Texture Model::genTexture(std::string path) {
	unsigned int textureId;
	glGenTextures(1, &textureId); /*Want to create 1 texture and store it in the int.*/
	Texture result;
	result.id = textureId;
	if (path.find("_specular") != std::string::npos) {
		result.type = "tSpecular";
	}
	else {
		result.type = "tDiffuse";
	}

	glBindTexture(GL_TEXTURE_2D, textureId);
	stbi_set_flip_vertically_on_load(true); /*Images usually have y=0 at the top of the image, not bottom.*/
	int width, height, nrChannels;
	std::string file_path = "src\\textures\\" + path;
	const char* file_reference = file_path.c_str();
	unsigned char *data = stbi_load(file_reference, &width, &height, &nrChannels, 0);
	if (data) {
		if (nrChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		} 
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); /*Mipmaps are uses in down scaling to reduce memory and improve image quality.*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_image_free(data);
	}
	else {
		std::cout << "ERROR:TEXTURE:COULD_NOT_LOAD_FILE" << std::endl;
	}


	return result;
}

Model::~Model() {

}