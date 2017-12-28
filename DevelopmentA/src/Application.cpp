#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
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
void setRenderData(Chunk c_chunk, Shader s);
void drawChunk(Chunk c,Shader s, glm::vec3 offset);

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



	Shader lightShader("src\\shaders\\vertex.vert", "src\\shaders\\light_frag.frag");
	Shader baseShader("src\\shaders\\e_vertex.vert", "src\\shaders\\e_fragment.frag");

	glEnable(GL_DEPTH_TEST); //To prevent panes from overlapping.


	glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(mainCamera.getFOV()), (float)width / (float)height, 0.01f, 100.0f); //The model positions relative to 1 and -1.
	glm::mat4 model = glm::mat4(1.0f);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Hides cursor, records position.
	glfwSetCursorPosCallback(window, mouse_callback);

	//Loading the models.
	std::list<unsigned int> a;
	Model plane("src\\models\\plane2.obj");
	//Model cylinder("src\\models\\sphere.obj");
	Object sphereO(Model("src\\models\\cube.obj"));
	sphereO.move(glm::vec3(0.0f, 2.0f, 0.0f));
	sphereO.scale(glm::vec3(0.5f, 0.5f, 0.5f));
	
	mainCamera.setPos(0.0f, 5.0f, 0.0f);

	//Generating world.
	World world;
	world.setSize();
	world.generate();
	Chunk baseChunk = world.getChunk(0, 0);
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
	rm.add(world);
	/*
	std::vector<std::string> dinoT;
	dinoT.push_back("dino_diffuse.png");
	Model dino("src\\models\\dinosaur.obj", dinoT);
	Object grassBlock(dino);
	grassBlock.scale(glm::vec3(0.2f,0.2f,0.2f));
	

	
	*/
	Scene s;

	//s.addObject(grassBlock);

	Chunk viewChunk;
	glm::vec3 prePos(10.0f, 0.0f, 10.0f);

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

				baseChunk = world.getChunk(mainCamera.getChunk().x, mainCamera.getChunk().y);
				glm::vec2 nowChunk(mainCamera.getChunk().x, mainCamera.getChunk().y);
				if (nowChunk != preChunk) {
					//setRenderData(baseChunk, baseShader);
					preChunk = nowChunk;
				}
				mainCamera.update(baseChunk);


				if (prePos.x != mainCamera.getPos().x || prePos.z != mainCamera.getPos().z) {
					//Detects movement
					//viewChunk = rm.getViewChunk(0, mainCamera.getChunk(), mainCamera.getBlock());
					prePos = mainCamera.getPos();
				}

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
		glm::mat4 view = mainCamera.viewMatrix();

		s.setPointLightAttrib(0,"diffuse", glm::vec3(1.0f, (float)camX, (float)camZ));

		rm.setMatrices(view, projection);
		rm.render(s);

		

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));


		/*
		for each (Chunk c_chunk in world.getAllChunks()) {
				unsigned int block_index = 0;
				block_models.push_back(grass);
				block_models.push_back(sand);
				block_models.push_back(sphere);
				for (unsigned int x = 0; x < 16; x++) {
					for (unsigned int z = 0; z < 16; z++) {
						if (c_chunk.getBlock(x, 1, z) == 0 || c_chunk.getBlock(x + 1, 0, z) == 0 || c_chunk.getBlock(x - 1, 0, z) == 0 || c_chunk.getBlock(x, 0, z + 1) == 0 || c_chunk.getBlock(x, 0, z - 1) == 0) {
							//Checks if there is air surrounding blocks.
							if (c_chunk.getBlock(x, 0, z) != 0) {
								block_types.push_back(c_chunk.getBlock(x, 0, z) - 1);
								block_positions.push_back(glm::vec3(x * 2, 0, z * 2));
								block_index++;
							}
						}
					}

				}
				if (block_index > 0) {
					drawInstanced(block_models, block_types, block_positions, baseShader);
					block_models.clear();
					block_types.clear();
					block_positions.clear();
					block_index = 0;
				}
		}
		*/

		for (int x = 0; x < 16; x++) {
			for (int z = 0; z < 16; z++) {

				//rm.drawBlock(baseShader, rm.getBlock(0, x + mainCamera.getPos().x, 0, z + mainCamera.getPos().z), glm::vec3(x + mainCamera.getPos().x, 5.0f, z + mainCamera.getPos().z));
			}
		}
		
		//rm.drawBlock(baseShader, 1, glm::vec3(1.0f, 5.0f, 2.0f));
		//drawChunk(rm, viewChunk, baseShader, mainCamera.getPos());
		//drawChunk(rm, rm.getChunk(0, mainCamera.getChunk().x, mainCamera.getChunk().y), baseShader, glm::vec3(0.0f,0.0f,0.0f));
		//drawInstanced(block_models, block_types, block_positions, baseShader);




		//glDisable(GL_CULL_FACE);


		

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}



	baseShader.deleteProgram();
	lightShader.deleteProgram();

	glfwTerminate();
	return 0;
}

void setRenderData(Chunk c_chunk, Shader s) {
	//Resources r

	block_models.clear();
	block_types.clear();
	block_positions.clear();

	std::vector < std::string > crateT;
	crateT.push_back("b_grass_diffuse.png");
	crateT.push_back("grass_specular.png");
	Model grass("src\\models\\t_Crate.obj", crateT);
	std::vector < std::string > sandT;
	sandT.push_back("b_sand_diffuse.png");
	sandT.push_back("grass_specular.png");
	Model sand("src\\models\\t_Crate.obj", sandT);

		block_models.push_back(grass);
		block_models.push_back(sand);
		for (unsigned int x = 0; x < 16; x++) {
			for (unsigned int z = 0; z < 16; z++) {
				if (c_chunk.getBlock(x, 1, z) == 0 || c_chunk.getBlock(x + 1, 0, z) == 0 || c_chunk.getBlock(x - 1, 0, z) == 0 || c_chunk.getBlock(x, 0, z + 1) == 0 || c_chunk.getBlock(x, 0, z - 1) == 0) {
					//Checks if there is air surrounding blocks.
					if (c_chunk.getBlock(x, 0, z) != 0) {
						block_types.push_back(c_chunk.getBlock(x, 0, z) - 1);
						block_positions.push_back(glm::vec3((16 * c_chunk.getStart()[0]) + x, 0, (16 * c_chunk.getStart()[1]) + z));
						//+ (16 * c_chunk.getStart()[0])
						//+ (16 * c_chunk.getStart()[1])
					}
				}
			}

		}
}

void drawChunk(ResourceManager rm, Chunk c, Shader s, glm::vec3 offset) {
	for (int x = 0; x < 16; x++) {
		for (int z = 0; z < 16; z++) {
			if (c.getBlock(x, 0, z) != 0) {
				Model m = rm.getModel(c.getBlock(x, 0, z) - 1);
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(((16 * c.getStart()[0]) + x - 8) + offset.x, 0.0f, offset.z + ((16 * c.getStart()[1]) + z - 8)));
				model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				s.setM4FV("model", model);
				m.draw(s);
			}
		}
	}
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