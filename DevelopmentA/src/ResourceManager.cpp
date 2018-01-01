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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


ResourceManager::~ResourceManager()
{
}

Model ResourceManager::getModel(unsigned int ID)
{
	return models[ID];
}

void ResourceManager::add(Model m) {
	models.push_back(m);
}
void ResourceManager::reset() {
	models.clear();
}

void ResourceManager::setMatrices(glm::mat4 _view, glm::mat4 _proj) {
	w_view = _view;
	w_projection = _proj;
}

void ResourceManager::end() {
	for each (Shader s in shaders) {
		s.deleteProgram();
	}
}

void ResourceManager::renderAnimated(Scene sc) {
	//Animated
	shaders[2].use();
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

	//renderAnimated(s);

	//Objects
	for (unsigned int i = 1; i < 3; i++) {
		shaders[i].use();
		shaders[i].setM4FV("view", w_view);
		shaders[i].setM4FV("projection", w_projection);
		shaders[i].setF("material.shininess", 64.0f);
		shaders[i].setV3F("l_directional.direction", -0.2f, -1.0f, -0.3f);
		shaders[i].setV3F("l_directional.ambient", 0.05f, 0.05f, 0.05f);
		shaders[i].setV3F("l_directional.diffuse", 0.4f, 0.4f, 0.4f);
		shaders[i].setV3F("l_directional.specular", 0.5f, 0.5f, 0.5f);
		unsigned int index = 0;
		for each (PointLight pl in s.getPointLights()) {
			if (pl.isActive()) {
				shaders[i].setV3F("l_points[" + std::to_string(index) + "].position", pl.getPos().x, pl.getPos().y, pl.getPos().z);
				shaders[i].setV3F("l_points[" + std::to_string(index) + "].ambient", 0.05f, 0.05f, 0.05f);
				shaders[i].setV3F("l_points[" + std::to_string(index) + "].diffuse", pl.getDiffuse().x, pl.getDiffuse().y, pl.getDiffuse().z);
				shaders[i].setV3F("l_points[" + std::to_string(index) + "].specular", 1.0f, 1.0f, 1.0f);
				shaders[i].setF("l_points[" + std::to_string(index) + "].constant", 1.0f);
				shaders[i].setF("l_points[" + std::to_string(index) + "].linear", 0.09f);
				shaders[i].setF("l_points[" + std::to_string(index) + "].quadratic", 0.032f);
				index++;
			}
		}
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	shaders[1].use();
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
	for each (PointLight pl in s.getPointLights()) {
		if (pl.isActive() == true) {
			shaders[0].setV3F("light_colour", pl.getDiffuse().x, pl.getDiffuse().y, pl.getDiffuse().z);
			model = glm::mat4(1.0f);
			model = glm::translate(model, pl.getPos());
			model = glm::scale(model, glm::vec3(0.2f));
			shaders[0].setM4FV("model", model);
			models[0].draw(shaders[0]);
		}
	}

	glDisable(GL_CULL_FACE);
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
