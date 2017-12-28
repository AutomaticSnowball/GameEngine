#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "maths\Matrix4.h"
#include <iostream>
#include <fstream>

#include "ResourceManager.h"

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"

#include <cstdlib> //Random number generation

using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void drawInstanced(vector<Model> blockTypes, vector<unsigned int> type, vector<glm::vec3> pos, Shader s);

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 1080/2, lastY = 720/2; //For mouse position.

float width = 1080;
float height = 720;


vector<Model> block_models;
vector<unsigned int> block_types;
vector<glm::vec3> block_positions;

Camera mainCamera;
int main(void)
{
	cout << "OpenGL Development: Theodore Russell" << endl;
	cout << "Game Engine - V1.0" << endl;
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	//Multisampling
	//glfwWindowHint(GLFW_SAMPLES, 2);


	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window;
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glViewport(0, 0, width, height);

	if (glewInit() != GLEW_OK) {
		cout << "Error: not init" << endl;
		glfwTerminate();
		return -1;
	}

	//glEnable(GL_MULTISAMPLE);



	ResourceManager rm;


	lastX = 1080 / 2;
	lastY = 720 / 2;
	glfwSetCursorPos(window, lastX, lastY);


	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); /*Resizing the window. - Calls the function using the window context*/
	cout << "GPU: " << glGetString(GL_VERSION) << endl;

	glEnable(GL_DEPTH_TEST); //To prevent panes from overlapping.


	glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(mainCamera.getFOV()), (float)width / (float)height, 0.01f, 100.0f); //The model positions relative to 1 and -1.

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Hides cursor, records position.
	glfwSetCursorPosCallback(window, mouse_callback);

	//Loading the models.
	std::list<unsigned int> a;
	Model plane("src\\models\\plane2.obj");
	//Model cylinder("src\\models\\sphere.obj");
	Object sphereO(Model("src\\models\\cube.obj"));
	sphereO.move(glm::vec3(0.0f, 2.0f, 0.0f));
	sphereO.scale(glm::vec3(0.5f, 0.5f, 0.5f));
	
	mainCamera.setPos(0.0f, 0.0f, 3.0f);

	//Generating world.
	/*
	World world;
	world.setSize();
	world.generate();
	*/
	float timer = glfwGetTime();
	float updateTime = 0;
	int frames = 0;

	//setRenderData(baseChunk, baseShader);
	glm::vec2 preChunk(0,0);


	std::vector < std::string > block_texture;
	block_texture.clear();
	block_texture.push_back("b_grass_diffuse.png");
	block_texture.push_back("grass_specular.png");
	Model grass("src\\models\\block.obj", block_texture);
	rm.add(grass);
	block_texture.clear();
	block_texture.push_back("b_sand_diffuse.png");
	block_texture.push_back("grass_specular.png");
	Model sand("src\\models\\block.obj", block_texture);
	rm.add(sand);
	//rm.add(world);
	/*
	std::vector<std::string> dinoT;
	dinoT.push_back("dino_diffuse.png");
	Model dino("src\\models\\dinosaur.obj", dinoT);
	Object grassBlock(dino);
	grassBlock.scale(glm::vec3(0.2f,0.2f,0.2f));
	

	
	*/
	Scene s;

	//s.addObject(grassBlock);


	bool editMode = false;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		if (!editMode) {
			//Game mode
			float currentTime = glfwGetTime();
			deltaTime = currentTime - lastFrame;
			lastFrame = currentTime;
			updateTime += deltaTime / (1.0f / 60.0f);
			while (updateTime > 1.0f) {
				//Every 60th of a second.

				mainCamera.update();

				//####RENDERING
				rm.setMatrices(mainCamera.viewMatrix(), projection);
				rm.render(s);

				updateTime--;
			}
			frames++;
			if (glfwGetTime() - timer > 1.0f) {
				//Every 1 second.
				timer++;
				cout << "FPS:" << frames << endl;
				frames = 0;
			}
		}


		float radius = 1.0f;
		float camX = sin(glfwGetTime() * radius);
		float camZ = cos(glfwGetTime() * radius);

	

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	rm.end();

	glfwTerminate();
	return 0;
}

void drawInstanced(vector<Model> blockTypes, vector<unsigned int> type, vector<glm::vec3> pos, Shader s) {
	vector<unsigned int> quantity;
	vector<vector<glm::vec3>> block_Pos;
	for (int i = 0; i < blockTypes.size(); i++) {
		quantity.push_back(0);
	}
	
	//Generates the vectors to store all the positions of each model.
	vector<vector<glm::vec3>> positions;
	for each (Model m in blockTypes) {
		vector<glm::vec3> v;
		positions.push_back(v);
	}

	for each (vector<glm::vec3> vv in positions) {
		for (int i = 0; i < 256; i++) {
			vv.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		}
		block_Pos.push_back(vv);
	}

	//Sets the data of each position.
	unsigned int posPos = 0;
	for each (unsigned int x in type) {
		block_Pos[x][quantity[x]] = pos[posPos];
		posPos++;
		quantity[x]++;
		
	}

	unsigned int index = 0;
	for each (Model z in blockTypes) {
		unsigned int blockIndex = 0;
		for each (glm::vec3 y in block_Pos[index]) {
			stringstream ss;
			ss << blockIndex;
			s.setV3I(("positions[" + ss.str() + "]").c_str(), y.x, y.y, y.z);
			blockIndex++;
		}

		z.drawInstanced(s, quantity[index]);
		index++;
	}
}



void drawInstanced(vector<Model> blockTypes, vector<unsigned int> quantity, vector<vector<glm::vec3>> block_Pos, Shader s) {
	unsigned int index = 0;
	for each (Model z in blockTypes) {
		unsigned int blockIndex = 0;
		for each (glm::vec3 y in block_Pos[index]) {
			stringstream ss;
			ss << blockIndex;
			s.setV3I(("positions[" + ss.str() + "]").c_str(), y.x, y.y, y.z);
			blockIndex++;
		}

		z.drawInstanced(s, quantity[index]);
		index++;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		mainCamera.KEY_INPUT(400, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		mainCamera.KEY_INPUT(5, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		mainCamera.KEY_INPUT(6, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		mainCamera.KEY_INPUT(1, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		mainCamera.KEY_INPUT(2, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		mainCamera.KEY_INPUT(3, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		mainCamera.KEY_INPUT(4, deltaTime);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	float x_offset = xpos - lastX;
	float y_offset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	mainCamera.MOUSE_MOVE(x_offset, y_offset);

}