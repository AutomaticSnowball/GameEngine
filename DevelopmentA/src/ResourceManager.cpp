#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
	Shader LightShader("src\\shaders\\vertex.vert", "src\\shaders\\light_frag.frag");
	shaders.push_back(LightShader);

	Shader BaseShader("src\\shaders\\e_vertex.vert", "src\\shaders\\e_fragment.frag");
	shaders.push_back(BaseShader);

	Shader AnimatedShader("src\\shaders\\animated.vert", "src\\shaders\\e_fragment.frag");
	shaders.push_back(AnimatedShader);

	AnimatedModel human("src\\models\\skeleton.dae");
	animated.push_back(human);
	animated[0].animate(0);

	glClearColor(0.1f, 0.6f, 0.6f, 1.0f);
}


ResourceManager::~ResourceManager()
{
}

World ResourceManager::getWorld(unsigned int ID)
{
	return worlds[ID];
}

Model ResourceManager::getModel(unsigned int ID)
{
	return models[ID];
}

void ResourceManager::add(World w) {
	worlds.push_back(w);
}
void ResourceManager::add(Model m) {
	models.push_back(m);
}
void ResourceManager::reset() {
	models.clear();
	worlds.clear();
}

void ResourceManager::setMatrices(glm::mat4 _view, glm::mat4 _proj) {
	w_view = _view;
	w_projection = _proj;
}

void ResourceManager::renderAnimated(Scene sc) {
	//Animated
	shaders[2].use();
	shaders[2].setM4FV("view", w_view);
	shaders[2].setM4FV("projection", w_projection);
	shaders[2].setV3F("l_directional.direction", -0.2f, -1.0f, -0.3f);
	shaders[2].setV3F("l_directional.ambient", 0.05f, 0.05f, 0.05f);
	shaders[2].setV3F("l_directional.diffuse", 0.4f, 0.4f, 0.4f);
	shaders[2].setV3F("l_directional.specular", 0.5f, 0.5f, 0.5f);
	for (int i = 0; i < 100; i++) {
		if (sc.getPointLight(i).isActive()) {
			shaders[2].setV3F("l_points[" + std::to_string(i) + "].position", sc.getPointLight(i).getPos().x, sc.getPointLight(i).getPos().y, sc.getPointLight(i).getPos().z);
			shaders[2].setV3F("l_points[" + std::to_string(i) + "].ambient", 0.05f, 0.05f, 0.05f);
			shaders[2].setV3F("l_points[" + std::to_string(i) + "].diffuse", sc.getPointLight(i).getDiffuse().x, sc.getPointLight(i).getDiffuse().y, sc.getPointLight(i).getDiffuse().z);
			shaders[2].setV3F("l_points[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
			shaders[2].setF("l_points[" + std::to_string(i) + "].constant", 1.0f);
			shaders[2].setF("l_points[" + std::to_string(i) + "].linear", 0.09f);
			shaders[2].setF("l_points[" + std::to_string(i) + "].quadratic", 0.032f);
		}
	}
	unsigned int index = 0;
	for each (AnimatedModel am in animated) {
		unsigned int pos = 0;
		am.update();
		glm::mat4 model = glm::mat4(1.0f);
		model = am.getTransform();
		shaders[2].setM4FV("model", model);
		for each (glm::mat4 m in am.getJointTransforms()) {
			shaders[2].setM4FV("jointTransforms[" + std::to_string(pos) + "]", m); //Stores all the joint transforms into the vertex shader.
			pos++;
		}
		am.getModel().draw(shaders[2]);
		animated[index] = am;
		index++;
	}

}

void ResourceManager::render(Scene s) {

	/* Render here */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //reset all of the depth planes.

	glEnable(GL_DEPTH_TEST);

	renderAnimated(s);

	//Objects
	shaders[1].use();
	shaders[1].setM4FV("view", w_view);
	shaders[1].setM4FV("projection", w_projection);
	shaders[1].setF("material.shininess", 64.0f);
	shaders[1].setV3F("l_directional.direction", -0.2f, -1.0f, -0.3f);
	shaders[1].setV3F("l_directional.ambient", 0.05f, 0.05f, 0.05f);
	shaders[1].setV3F("l_directional.diffuse", 0.4f, 0.4f, 0.4f);
	shaders[1].setV3F("l_directional.specular", 0.5f, 0.5f, 0.5f);
	for (int i = 0; i < 100; i++) {
		if (s.getPointLight(i).isActive()) {
			shaders[1].setV3F("l_points[" + std::to_string(i) + "].position", s.getPointLight(i).getPos().x, s.getPointLight(i).getPos().y, s.getPointLight(i).getPos().z);
			shaders[1].setV3F("l_points[" + std::to_string(i) + "].ambient", 0.05f, 0.05f, 0.05f);
			shaders[1].setV3F("l_points[" + std::to_string(i) + "].diffuse", s.getPointLight(i).getDiffuse().x, s.getPointLight(i).getDiffuse().y, s.getPointLight(i).getDiffuse().z);
			shaders[1].setV3F("l_points[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
			shaders[1].setF("l_points[" + std::to_string(i) + "].constant", 1.0f);
			shaders[1].setF("l_points[" + std::to_string(i) + "].linear", 0.09f);
			shaders[1].setF("l_points[" + std::to_string(i) + "].quadratic", 0.032f);
		}
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	glm::mat4 model;
	for each (Object o in s.getObjects()) {
		model = glm::mat4(1.0f);
		model = o.getTransform();
		shaders[1].setM4FV("model", model);
		o.draw(shaders[1]);
	}



	//Lights
	shaders[0].use();
	shaders[0].setM4FV("view", w_view);
	shaders[0].setM4FV("projection", w_projection);
	for (int i = 0; i < 100; i++) {
		if (s.getPointLight(i).isActive() == true) {
			shaders[0].setV3F("light_colour", s.getPointLight(i).getDiffuse().x, s.getPointLight(i).getDiffuse().y, s.getPointLight(i).getDiffuse().z);
			model = glm::mat4(1.0f);
			model = glm::translate(model, s.getPointLight(i).getPos());
			model = glm::scale(model, glm::vec3(0.2f));
			shaders[0].setM4FV("model", model);
			models[0].draw(shaders[0]);
		}
		else {
			break;
		}
	}

	glDisable(GL_CULL_FACE);
}

Chunk ResourceManager::getChunk(unsigned int ID, int x, int z)
{
	return worlds[ID].getChunk(x, z);
}

Chunk ResourceManager::getViewChunk(unsigned int ID, glm::vec2 p, glm::vec3 pos) {
	Chunk c;
	Chunk ca = getChunk(ID, p.x, p.y);
	for (unsigned int x = 0; x < 16; x++) {
		for (unsigned int z = 0; z < 16; z++) {
			int chunk_x = ca.getStart()[0];
			int chunk_z = ca.getStart()[1];
			int b_x = pos.x - 8 + x;
			if (b_x < 0) {
				b_x += 16;
				chunk_x = ca.getStart()[0] - 1;
			}
			else if (b_x > 15) {
				b_x -= 16;
				chunk_x = ca.getStart()[0] + 1;
			}
			int b_z = pos.z - 8 + z;
			if (b_z < 0) {
				b_z += 16;
				chunk_z = ca.getStart()[1] - 1;
			}
			else if (b_z > 15) {
				b_z -= 16;
				chunk_z = ca.getStart()[1] + 1;
			}
			c.SetBlock(x, 0, z, getChunk(ID, chunk_x, chunk_z).getBlock(b_x, 0, b_z));

		}
	}
	c.assignStart(0, 0);
	return c;
}

unsigned int ResourceManager::getBlock(unsigned int ID, int x, int y, int z) {

	glm::vec2 pos;
	pos.x = (x + 0.5);
	pos.y = (z + 0.5);
	pos.x = pos.x / 16;
	pos.y = pos.y / 16;
	if (pos.x == -0) { pos.x = -1; }
	if (pos.y == -0) { pos.y = -1; }

	pos.x = floor(pos.x);
	pos.y = floor(pos.y);

	glm::vec3 blockPos = glm::vec3(x,y,z);
	blockPos.x = round(blockPos.x);
	blockPos.y = round(blockPos.y) - 1;
	blockPos.z = round(blockPos.z);
	if (blockPos.x == -0) {
		blockPos.x = 0;
	}
	if (blockPos.z == -0) {
		blockPos.z = 0;
	}
	blockPos.x = round(blockPos.x) + (16 * -pos.x);
	blockPos.z = round(blockPos.z) + (16 * -pos.y);

	
	return getChunk(ID, pos.x, pos.y).getBlock(blockPos.x, 0, blockPos.z);
}

void ResourceManager::drawBlock(Shader s,unsigned int ID, glm::vec3 pos) {
	if (ID  != 0) {
		Model m = getModel(ID - 1);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, pos);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		s.setM4FV("model", model);
		m.draw(s);
	}
}

Camera ResourceManager::getCamera()
{
	return cam;
}

void ResourceManager::setCamera(Camera c)
{
	cam = c;
}
