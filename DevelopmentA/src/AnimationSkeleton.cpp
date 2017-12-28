#include "AnimationSkeleton.h"



AnimationSkeleton::AnimationSkeleton()
{

}

AnimationSkeleton::AnimationSkeleton(std::string path)
{
	loadDae(path);
}

void AnimationSkeleton::loadDae(std::string filePath) {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> t_coords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> jointIDs;
	std::vector<glm::vec3> vertexWeights;
	std::vector<glm::ivec3> indices; //Stores what each vertex needs to contain from the other data.

	std::vector<float> weights;
	std::vector<unsigned int> joints;
	std::vector<glm::ivec2> jointEffects;

	//Joint data
	std::vector<std::string> jointNames;
	std::vector<glm::mat4> jointStartPoses;
	std::vector<float> timeStamps;
	std::map<unsigned int, std::vector<glm::mat4>> jointAnimations;

	std::fstream obj;
	obj.open(filePath);
	std::string mainLine;
	unsigned int mode = 0;
	unsigned int currentJoint = 0;

	std::map<std::string, unsigned int> namesAndTiers;
	unsigned int jointHier_type = 0;
	unsigned int jointHier_count = 0;
	unsigned int jointHier_total = 0;
	unsigned int preJointHier = 1;

	while (std::getline(obj, mainLine)) {
		if (mainLine.find("<library_geometries>") != std::string::npos) {
			mode = 1;
		}
		else if (mainLine.find("<polylist material=") != std::string::npos) {
			mode = 2;
		}
		else if (mainLine.find("<library_controllers>") != std::string::npos) {
			mode = 3;
		}
		else if (mainLine.find("<library_animations>") != std::string::npos) {
			mode = 4;
		}
		else if (mainLine.find("<library_visual_scenes>") != std::string::npos) {
			mode = 5;
		}
		else if (mainLine.find("</library_geometries>") != std::string::npos || mainLine.find("</library_visual_scenes>") != std::string::npos || mainLine.find("</library_animations>") != std::string::npos || mainLine.find("</polylist>") != std::string::npos || mainLine.find("</library_controllers>") != std::string::npos) {
			mode = 0;
		}
		if (mode == 1 && mainLine.find("mesh-positions\">") != std::string::npos) {
			std::string dataLine;
			std::getline(obj, dataLine);
			dataLine = getDataFromIdentifier(dataLine);
			positions = getVec3FromString(dataLine, ' ', false);
		} else if (mode == 1 && mainLine.find("mesh-normals\">") != std::string::npos) {
			std::string dataLine;
			std::getline(obj, dataLine);
			dataLine = getDataFromIdentifier(dataLine);
			normals = getVec3FromString(dataLine, ' ', true);
		} else if (mode == 1 && mainLine.find("mesh-map-0\">") != std::string::npos) {
			std::string dataLine;
			std::getline(obj, dataLine);
			dataLine = getDataFromIdentifier(dataLine);
			t_coords = getVec2FromString(dataLine, ' ');
		}
		if (mode == 2 && mainLine.find("<p>") != std::string::npos) {
			std::string dataLine = mainLine;
			dataLine = getDataFromIdentifier(dataLine);
			indices = getIndicesFromString(dataLine, ' ');
			//Position // Normals // Textures //
		}
		if (mode == 3 && mainLine.find("weights\">") != std::string::npos) {
			std::string dataLine;
			std::getline(obj, dataLine);
			dataLine = getDataFromIdentifier(dataLine);
			weights = getFloatFromString(dataLine, ' ');
		} else if (mode == 3 && mainLine.find("<vcount>") != std::string::npos) {
			std::string dataLine = mainLine;
			dataLine = getDataFromIdentifier(dataLine);
			joints = getIntFromString(dataLine, ' ');
		} else if (mode == 3 && mainLine.find("<v>") != std::string::npos) {
			std::string dataLine = mainLine;
			dataLine = getDataFromIdentifier(dataLine);
			jointEffects = getIntVec2FromString(dataLine, ' ');
		}
		else if (mode == 3 && mainLine.find("joints\">") != std::string::npos) {
			std::string dataLine = mainLine;
			std::getline(obj, dataLine);
			dataLine = getDataFromIdentifier(dataLine);
			jointNames = getStringFromString(dataLine, ' ');
		}
		else if (mode == 3 && mainLine.find("bind_poses\">") != std::string::npos) {
			std::string dataLine = mainLine;
			std::getline(obj, dataLine);
			dataLine = getDataFromIdentifier(dataLine);
			jointStartPoses = getMat4FromString(dataLine, ' ');
		}
		else if (mode == 4 && mainLine.find("input-array\"") != std::string::npos && mainLine.find("<float_array") != std::string::npos) {
			std::string dataLine = mainLine;
			dataLine = getDataFromIdentifier(dataLine);
			timeStamps = getFloatFromString(dataLine, ' ');
		}
		else if (mode == 4 && mainLine.find("output-array\"") != std::string::npos && mainLine.find("<float_array") != std::string::npos) {
			std::string dataLine = mainLine;
			dataLine = getDataFromIdentifier(dataLine);
			std::vector<glm::mat4> i = getMat4FromString(dataLine, ' ');
			jointAnimations.insert(std::pair<unsigned int, std::vector<glm::mat4>>(currentJoint, i)); //Each joint // Each keyframe.
			currentJoint++;
		}
		else if (mode == 5 && mainLine.find("type=\"NODE\">") != std::string::npos && mainLine.find("<node ") != std::string::npos) {
			jointHier_type = 1;
		}
		else if (mode == 5 && mainLine.find("type=\"JOINT\">") != std::string::npos && mainLine.find("<node ") != std::string::npos) {
			std::string dataLine = mainLine;
			std::string name = getNameFromIdentifier(dataLine);
			std::getline(obj, dataLine);
			dataLine = getDataFromIdentifier(dataLine);
			glm::mat4 transform = getMat4FromString(dataLine, ' ')[0]; //Base transform (not inverse).
			jointHier_count++;
			Joint currentJoint = Joint(jointHier_total, name, 1, jointStartPoses[jointHier_total]);
			//Joint currentJoint = Joint(jointHier_total, name, 0, transform); //Creates the joint.
			if (jointHier_count == 1) {
				//rootJoint = Joint(jointHier_total, name, 0, transform);
				rootJoint = Joint(jointHier_total, name, 1, jointStartPoses[0]);
			}
			else {
				rootJoint.setChild(getClosestParent(namesAndTiers, jointHier_count), currentJoint);
			}
			jointHier_total++;
			jointHier_type = 0;
			namesAndTiers.insert(std::pair<std::string, unsigned int>(name, jointHier_count));
			preJointHier = jointHier_count;
		}
		else if (mode == 5 && jointHier_type == 0 && mainLine.find("</node>") != std::string::npos) {
			jointHier_count--;
		}
	}

	//Get how each vertex is affected by the joints. //Stores in the vertice array.
	unsigned int jointEffectPos = 0;
	for each (unsigned int j in joints) {
		glm::ivec3 v;
		glm::vec3 w;
		//Each vertex
		double sum = 0;
		for (unsigned int i = 1; i < j + 1; i++) {
			if (i <= 3) {
				sum += weights[jointEffects[jointEffectPos + i - 1].y];
			}
		}
		for (unsigned int i = 1; i < 4; i++) {
			//Each joint that affects the vertex.
			if (i <= j) {
				if (i == 1) {
					v.x = jointEffects[jointEffectPos + i - 1].x; //The joint affecting the vertex.
					w.x = weights[jointEffects[jointEffectPos + i - 1].y] / sum; //The weight of that join on this vertex.
				}
				else if (i == 2) {
					v.y = jointEffects[jointEffectPos + i - 1].x;
					w.y = weights[jointEffects[jointEffectPos + i - 1].y] / sum;
				}
				else if (i == 3) {
					v.z = jointEffects[jointEffectPos + i - 1].x;
					w.z = weights[jointEffects[jointEffectPos + i - 1].y] / sum;
				}
			}
			else {
				if (i == 2) {
					v.y = 0;
					w.y = 0;
				}
				else if (i == 3) {
					v.z = 0;
					w.z = 0;
				}
			}
		}
		jointEffectPos += j;
		jointIDs.push_back(v);
		vertexWeights.push_back(w);
	}
	setModelData(indices, positions, normals, t_coords, jointIDs, vertexWeights); //Generates VBO.
	setKeyFrames(jointAnimations, jointNames, timeStamps); //Generates KeyFrames.
	animations.push_back(Animation(timeStamps[timeStamps.size() - 1], keyFrames));
	mesh = AnimatedMesh(modelData);
	
}

void AnimationSkeleton::setKeyFrames(std::map<unsigned int, std::vector<glm::mat4>> _keyframes, std::vector<std::string> jointNames, std::vector<float> times) {
	keyFrames.clear();
	for (unsigned int i = 0; i < times.size(); i++) { //Each new key frame.
		std::map<std::string, JointTransform> jt;
		for (unsigned int jointPos = 0; jointPos < _keyframes.size(); jointPos++) { //Each joint transformation for the current time.
			jt.insert(std::pair<std::string, JointTransform>(jointNames[jointPos], JointTransform(_keyframes[jointPos][i])));
		}
		keyFrames.push_back(KeyFrame(times[i], jt));
	}
}

void AnimationSkeleton::setModelData(std::vector<glm::ivec3> i, std::vector<glm::vec3> p, std::vector<glm::vec3> n, std::vector<glm::vec2> t, std::vector<glm::vec3> j, std::vector<glm::vec3> w) {
	modelData.vertex.clear();
	unsigned int position = 0;
	for each (glm::ivec3 indice in i) {
		AnimatedVertex v;
		v.position = p[indice.x];
		v.normal = n[indice.y];
		v.t_coords = t[indice.z];
		v.jointIDs = j[indice.x]; //Gets joint and weights for equivilent vertex.
		v.vertexWeights = w[indice.x];
		modelData.vertex.push_back(v);
		modelData.indice.push_back(position);
		position++;
	}
	ATexture tex = genTexture("grass_diffuse.png");
	modelData.texture.push_back(tex);
}

ATexture AnimationSkeleton::genTexture(std::string path) {
	unsigned int textureId;
	glGenTextures(1, &textureId); /*Want to create 1 texture and store it in the int.*/
	ATexture result;
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

Animation AnimationSkeleton::getAnimation(unsigned int ID) {
	return animations[ID];
}

std::vector<Animation> AnimationSkeleton::getAllAnimations() {
	return animations;
}

std::string AnimationSkeleton::getClosestParent(std::map<std::string, unsigned int> d,unsigned int pos) {
	std::string r = "Torso";
	std::map<std::string, unsigned int> data;
	for each (std::pair<std::string, unsigned int> s in d) {
		if (s.second == pos - 1) {
			data.insert(std::pair<std::string, unsigned int>(s.first, s.second));
		}
	}
	unsigned int ind = 0;
	for each (std::pair<std::string, unsigned int> s in data) {
		if (rootJoint.getChild(s.first).index > ind) {
			ind = rootJoint.getChild(s.first).index;
		}
	}
	r = rootJoint.getChild(ind).name;
	return r;
}

std::string AnimationSkeleton::getNameFromIdentifier(std::string input) {
	std::string meshLine = input;
	unsigned int pos = meshLine.find("id=\"");
	if (pos != std::string::npos) {
		meshLine.erase(0, pos + 4);
		unsigned int attribPoint = meshLine.find("name=\"");
		meshLine.erase(attribPoint - 2, meshLine.length());
	}
	return meshLine;
}

std::string AnimationSkeleton::getDataFromIdentifier(std::string input) {
	std::string meshLine = input;
	std::vector<unsigned int> arrowPos;
	unsigned int pos = -1;
	unsigned int change = 0;
	bool start = false;
	for each (char c in input) {
		pos++;
		if (c == '<' || c == '>') {
			arrowPos.push_back(pos);
			start = true;
		}
		else if (c == ' ' && start == false) {
			meshLine.erase(pos - change, 1);
			change += 1;
		}

		if (arrowPos.size() == 2) {
			meshLine.erase(arrowPos[0] - change, arrowPos[1] - arrowPos[0] + 1);
			change += arrowPos[1] - arrowPos[0] + 1;
			arrowPos.clear();
		}
	}
	return meshLine;
}

std::vector<glm::ivec3> AnimationSkeleton::getIndicesFromString(std::string input, char seperator) {
	std::string item;
	std::stringstream ss(input);
	std::vector<glm::ivec3> r;
	unsigned int pos = 0;
	glm::ivec3 p;
	while (std::getline(ss, item, seperator)) {
		if (pos == 0) {
			p.x = ::atof(item.c_str());
		}
		else if (pos == 1) {
			p.y = ::atof(item.c_str());
		}
		else if (pos == 2) {
			p.z = ::atof(item.c_str());
		}
		else if (pos == 3) {
			//Don't need the colour value.
		}
		pos++;
		if (pos == 4) {
			r.push_back(p);
			pos = 0;
		}
	}
	return r;
}

std::vector<glm::mat4> AnimationSkeleton::getMat4FromString(std::string input, char seperator) {
	std::string item;
	std::stringstream ss(input);
	std::vector<glm::mat4> r;
	unsigned int pos = 0;
	unsigned int row = 0;
	unsigned int col = 0;
	glm::mat4 p;
	while (std::getline(ss, item, seperator)) {
		if (pos < 16) {
			p[row][col] = ::atof(item.c_str());
		}
		col++;
		if (col == 4) {
			row++;
			col = 0;
		}
		pos++;
		if (pos == 16) {
			r.push_back(p);
			pos = 0;
			row = 0;
			col = 0;
		}
	}
	return r;
}

std::vector<std::string> AnimationSkeleton::getStringFromString(std::string input, char seperator) {
	std::string item;
	std::stringstream ss(input);
	std::vector<std::string> r;
	while (std::getline(ss, item, seperator)) {
		r.push_back(item);
	}
	return r;
}

std::vector<glm::ivec2> AnimationSkeleton::getIntVec2FromString(std::string input, char seperator) {
	std::string item;
	std::stringstream ss(input);
	std::vector<glm::ivec2> r;
	unsigned int pos = 0;
	glm::ivec2 p;
	while (std::getline(ss, item, seperator)) {
		if (pos == 0) {
			p.x = ::atof(item.c_str());
		}
		else if (pos == 1) {
			p.y = ::atof(item.c_str());
		}
		pos++;
		if (pos == 2) {
			r.push_back(p);
			pos = 0;
		}
	}
	return r;
}

std::vector<float> AnimationSkeleton::getFloatFromString(std::string input, char seperator) {
	std::string item;
	std::stringstream ss(input);
	std::vector<float> r;
	while (std::getline(ss, item, seperator)) {
		r.push_back(::atof(item.c_str()));
	}
	return r;
}

std::vector<unsigned int> AnimationSkeleton::getIntFromString(std::string input, char seperator) {
	std::string item;
	std::stringstream ss(input);
	std::vector<unsigned int> r;
	while (std::getline(ss, item, seperator)) {
		r.push_back(::atof(item.c_str()));
	}
	return r;
}

std::vector<glm::vec3> AnimationSkeleton::getVec3FromString(std::string input, char seperator, bool xyz) {
	std::string item;
	std::stringstream ss(input);
	std::vector<glm::vec3> r;
	unsigned int pos = 0;
	glm::vec3 p;
	while (std::getline(ss, item, seperator)) {
		if (pos == 0) {
			p.x = ::atof(item.c_str());
		} else if (pos == 1 && xyz == true) {
			p.y = ::atof(item.c_str());
		}
		else if (pos == 2 && xyz == true) {
			p.z = ::atof(item.c_str());
		}
		else if (pos == 1 && xyz == false) {
			p.z = ::atof(item.c_str());
		}
		else if (pos == 2 && xyz == false) {
			p.y = ::atof(item.c_str());
		}
		pos++;
		if (pos == 3) {
			r.push_back(p);
			pos = 0;
		}
	}
	return r;
}

std::vector<glm::vec2> AnimationSkeleton::getVec2FromString(std::string input, char seperator) {
	std::string item;
	std::stringstream ss(input);
	std::vector<glm::vec2> r;
	unsigned int pos = 0;
	glm::vec2 p;
	while (std::getline(ss, item, seperator)) {
		if (pos == 0) {
			p.x = ::atof(item.c_str());
		}
		else if (pos == 1) {
			p.y = ::atof(item.c_str());
		}
		pos++;
		if (pos == 2) {
			r.push_back(p);
			pos = 0;
		}
	}
	return r;
}



AnimatedMesh AnimationSkeleton::getMesh() {
	return mesh;
}

std::vector<ATexture> AnimationSkeleton::getTexture() {
	return modelData.texture;
}

AnimationSkeleton::~AnimationSkeleton()
{
}

Joint AnimationSkeleton::getRootJoint()
{
	return rootJoint;
}
