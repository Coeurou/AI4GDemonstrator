// main.cpp : définit le point d'entrée pour l'application console.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <nanogui/nanogui.h>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include "Agent.h"
#include "BehaviorsHeader.h"
#include "Constants.h"
#include "GLProgram.h"
#include "Grid.h"
#include "Model.h"
#include "RenderablePath.h"
#include "SteeringAgentImpl.h"
#include "SpatialStructure.h"
#include "TimeManager.h"
#include "UI.h"

struct Wall
{
	Model* wallModelPtr;
	SpatialStructure wallEntity;

	Wall(Model* model, int posX, int posZ, float orientation)
		: wallModelPtr(model)
	{
		wallEntity.position = glm::vec2(posX, posZ);
		wallEntity.orientation = orientation;
		wallEntity.size = glm::vec2(model->GetSize().x, model->GetSize().z);
	}
};

// Const variables used for AI init & update
const size_t GRID_WIDTH = 120;
const size_t GRID_DEPTH = 80;
const float HALF_GRID_WIDTH = GRID_WIDTH * 0.5f;
const float HALF_GRID_DEPTH = GRID_DEPTH * 0.5f;

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

auto characterSpeed = 4.0f;
auto targetSpeed = 4.0f;

auto FOV = -45.0f;

glm::mat4 model = glm::mat4(1);
glm::mat4 view = glm::lookAt(glm::vec3(0, 100, -7), glm::vec3(0), glm::vec3(0, 1, 0));
glm::mat4 projection = glm::perspective(glm::radians(FOV), WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 1000.0f);

// Location of our AI agents
SpatialStructure character;
SpatialStructure target;

Agent characterAgent(nullptr);
Agent targetAgent(nullptr);

// Random number generators
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> randomXPos(static_cast<int>(-HALF_GRID_WIDTH), static_cast<int>(HALF_GRID_WIDTH));
std::uniform_int_distribution<> randomZPos(static_cast<int>(-HALF_GRID_DEPTH), static_cast<int>(HALF_GRID_DEPTH));
std::uniform_real_distribution<float> randomOrientation(-glm::pi<float>(), glm::pi<float>());

// Behaviors
const auto speed = 4.0f;
const auto prediction = 1.0f;
const auto wanderRadius = 1.0f;
const auto wanderOffset = 2.0f;
const auto wanderRate = glm::half_pi<float>();

SteeringMovementBehavior* currentBehavior_charac = nullptr;

SteeringSeekBehavior seekBehavior_charac(&character, &target, speed);
SteeringLWYGBehavior lookBehavior_charac(&character);
SteeringFleeBehavior fleeBehavior_charac(&character, &target, speed);
SteeringPursueBehavior pursueBehavior_charac(&character, &target, speed, prediction);
SteeringEvadeBehavior evadeBehavior_charac(&character, &target, speed, prediction);
SteeringWanderBehavior wanderBehavior_charac(&character, speed, wanderRadius, wanderOffset, wanderRate);
SteeringArriveBehavior arriveBehavior_charac(&character, &target);
SteeringAlignBehavior alignBehavior_charac(&character, &target);
SteeringFaceBehavior faceBehavior_charac(&character, &target);

SteeringMovementBehavior* currentBehavior_target = nullptr;

SteeringSeekBehavior seekBehavior_target(&target, &character, speed);
SteeringLWYGBehavior lookBehavior_target(&target);
SteeringFleeBehavior fleeBehavior_target(&target, &character, speed);
SteeringPursueBehavior pursueBehavior_target(&target, &character, speed, prediction);
SteeringEvadeBehavior evadeBehavior_target(&target, &character, speed, prediction);
SteeringWanderBehavior wanderBehavior_target(&target, speed, wanderRadius, wanderOffset, wanderRate);
SteeringArriveBehavior arriveBehavior_target(&target, &character);
SteeringAlignBehavior alignBehavior_target(&target, &character);
SteeringFaceBehavior faceBehavior_target(&target, &character);
UI gui;

// Utility functions
std::vector<AABB> GetAllWallBBoxes(const std::vector<Wall>& walls)
{
	std::vector<AABB> boxes;
	for (auto& wall : walls) {
		AABB box;
		box.min = (wall.wallEntity.position - wall.wallEntity.size * 0.5f);
		box.max = (wall.wallEntity.position + wall.wallEntity.size * 0.5f);

		boxes.emplace_back(box);
	}
	return boxes;
}

std::vector<SpatialStructure*> GetAllAgentsPtr()
{
	std::vector<SpatialStructure*> agents;
	return agents;
}

std::vector<SpatialStructure*> GetAllObstaclesPtr()
{
	std::vector<SpatialStructure*> obstacles;
	return obstacles;
}

template<class T>
void TrimWorld(const T halfWorldSize, T* value)
{
	if (*value > halfWorldSize) {
		*value = -halfWorldSize;
	}
	else if (*value < -halfWorldSize) {
		*value = halfWorldSize;
	}
}

// GLFW callback implementation
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto screen = gui.GetScreen();
	screen->keyCallbackEvent(key, scancode, action, mods);
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	projection = glm::perspective(glm::radians(FOV), WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
	auto screen = gui.GetScreen();
	screen->resizeCallbackEvent(width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	FOV += (float)(yoffset * 0.5f);
	projection = glm::perspective(glm::radians(FOV), WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
	auto screen = gui.GetScreen();
	screen->scrollCallbackEvent(xoffset, yoffset);
}

void SetCharacterLinearBehavior(int behaviorIndex)
{
	auto behavior = dynamic_cast<BlendedBehavior*>(currentBehavior_charac);
	if (behavior == nullptr) {
		return;
	}

	SteeringMovementBehavior* newBehavior = nullptr;

	switch (behaviorIndex) {
	case 0:
		newBehavior = &seekBehavior_charac;
		break;
	case 1:
		newBehavior = &fleeBehavior_charac;
		break;
	case 2:
		newBehavior = &arriveBehavior_charac;
		break;
	case 3:
		newBehavior = &pursueBehavior_charac;
		break;
	case 4:
		newBehavior = &evadeBehavior_charac;
		break;
	case 5:
		newBehavior = &wanderBehavior_charac;
		break;
	default:
		break;
	}
	
	behavior->AddBehaviorAndWeight(0, newBehavior, 1.0f);
	gui.GenerateUI(currentBehavior_charac);
}

void SetCharacterAngularBehavior(int behaviorIndex)
{
	auto behavior = dynamic_cast<BlendedBehavior*>(currentBehavior_charac);
	if (behavior == nullptr) {
		return;
	}
	SteeringMovementBehavior* newBehavior = nullptr;

	switch (behaviorIndex) {
	case 0:
		newBehavior = &lookBehavior_charac;
		break;
	case 1:
		newBehavior = &alignBehavior_charac;
		break;
	case 2:
		newBehavior = &faceBehavior_charac;
		break;
	default:
		break;
	}

	behavior->AddBehaviorAndWeight(1, newBehavior, 1.0f);
	gui.GenerateUI(currentBehavior_charac);
}

int main()
{
	/*
	=======================================================
	bool InitGLFW

	Init GLFW library, quit program if failed (assertion ?)
	=======================================================
	*/
	if (!glfwInit())
		return -1;

	/*
	=====================================================================================================
	GLFWWindow* CreateWindow

	Instantiate a new GLFW window and the OpenGL context associated, quit program if failed (assertion ?)
	=====================================================================================================
	*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Artificial Intelligence Sandbox",
										  nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		return -1;
	}

	/*
	===================================================
	void AssignCurrentOpenGLContext(GLFWWindow* window)

	Make a GLFW window the current OpenGL context
	==================================================
	*/
	glfwMakeContextCurrent(window);

	/*
	===============================================================================
	bool InitGLEW

	Init OpenGL capabilities via GLEW library, quit program if failed (assertion ?)
	===============================================================================
	*/
	GLenum glewInitStatus = glewInit();
	if (glewInitStatus != GLEW_OK) {
		std::cerr << "Failed to init GLEW: " << glewGetErrorString(glewInitStatus) << std::endl;
		return -1;
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	/*
	===================================================
	void InitGUI(GLFWWindow* window)

	Init NanoGUI capabilities
	==================================================
	*/
	
	gui.InitUI(window);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	nanogui::Window nanoguiWindow(gui.GetScreen(), "Steering Behaviors");
	nanoguiWindow.setPosition(Eigen::Vector2i(10, WINDOW_HEIGHT - 200));	
	nanoguiWindow.setLayout(new nanogui::GroupLayout());

	nanogui::Label lAngularBehavior(&nanoguiWindow, "Set angular behavior (Red entity) :", "sans-bold");
	nanogui::ComboBox coboAngularBehavior(&nanoguiWindow, { "Look Where U Go", "Align", "Face" });
	coboAngularBehavior.setFontSize(16);
	coboAngularBehavior.setFixedSize(Eigen::Vector2i(100, 20));
	coboAngularBehavior.setCallback(SetCharacterAngularBehavior);

	nanogui::Label lLinearBehavior(&nanoguiWindow, "Set linear behavior (Red entity) :", "sans-bold");
	nanogui::ComboBox coboLinearBehavior(&nanoguiWindow, { "Seek", "Flee", "Arrive", "Pursue", "Evade", "Wander" });
	coboLinearBehavior.setFontSize(16);
	coboLinearBehavior.setFixedSize(Eigen::Vector2i(100, 20));
	coboLinearBehavior.setCallback(SetCharacterLinearBehavior);

	// The wall
	Model wallModel;
	assert(wallModel.LoadModel("../Resources/wall.obj"));
	std::vector<Wall> walls;

	/*
	=======================================================
	void InitGLFWCallback

	Assign glfw callback for window, keyboard, mouse events
	=======================================================
	*/
	glfwSetKeyCallback(window, key_callback);

	auto screen = gui.GetScreen();
	glfwSetMouseButtonCallback(window, 
		[](GLFWwindow*, int button, int action, int modifiers) {
			gui.GetScreen()->mouseButtonCallbackEvent(button, action, modifiers);
		}
	);
	glfwSetCursorPosCallback(window,
		[](GLFWwindow*, double x, double y) {
			gui.GetScreen()->cursorPosCallbackEvent(x, y);
		}
	);
	glfwSetCharCallback(window,
		[](GLFWwindow*, unsigned int codepoint) {
			gui.GetScreen()->charCallbackEvent(codepoint);
		}
	);

	glfwSetDropCallback(window,
		[](GLFWwindow*, int count, const char **filenames) {
			gui.GetScreen()->dropCallbackEvent(count, filenames);
		}
	);

	glfwSetScrollCallback(window,
		[](GLFWwindow*, double x, double y) {
			gui.GetScreen()->scrollCallbackEvent(x, y);
		}
	);

	glfwSetFramebufferSizeCallback(window,
		[](GLFWwindow*, int width, int height) {
			gui.GetScreen()->resizeCallbackEvent(width, height);
		}
	);

	glfwSetWindowSizeCallback(window, window_size_callback);

	glfwSetScrollCallback(window, scroll_callback);	

	const char* vsSource =	"#version 330 core\n"
							"layout(location = 0)\nin vec3 position;\n"
							"layout(location = 1)\nin vec3 color;\n"
							"out vec3 vsColor;"
							"uniform mat4 MVP;\n"
							"void main()\n{\ngl_Position = MVP * vec4(position, 1.0);\nvsColor = color;}";
	const char* fsSource = "#version 330 core\nin vec3 vsColor;\nout vec4 fsColor;\n"
						   "void main()\n{\nfsColor = vec4(vsColor,1.0);\n}";

	const char* vsSphereSource = "#version 330 core\n"
								 "layout(location = 0)\nin vec3 position;\n"
								 "uniform mat4 MVP;\n"
								 "void main()\n{\ngl_Position = MVP * vec4(position, 1.0);}";
	const char* fsSphereSource = "#version 330 core\nout vec4 fsColor;\nuniform vec3 sphereColor;\n"
								 "void main()\n{\nfsColor = vec4(sphereColor,1.0);\n}";

	// The grid
	Grid grid(GRID_WIDTH, GRID_DEPTH);
	grid.InitGeometry();
	
	// Grid program init
	GLProgram gridProgram;
	assert(gridProgram.AddShader(GL_VERTEX_SHADER, vsSource));
	assert(gridProgram.AddShader(GL_FRAGMENT_SHADER, fsSource));
	assert(gridProgram.LinkProgram());
	
	gridProgram.UseProgram();
	auto MVPLocationGP = glGetUniformLocation(gridProgram.GetProgramID(), "MVP");
	if (MVPLocationGP == GL_INVALID_VALUE || MVPLocationGP == GL_INVALID_OPERATION) {
		std::cerr << "Error cannot retrieve MVP uniform variable in OpenGL program" << std::endl;
	}

	// Sphere program init
	GLProgram sphereProgram;
	assert(sphereProgram.AddShader(GL_VERTEX_SHADER, vsSphereSource));
	assert(sphereProgram.AddShader(GL_FRAGMENT_SHADER, fsSphereSource));
	assert(sphereProgram.LinkProgram());

	sphereProgram.UseProgram();
	auto MVPLocationSP = glGetUniformLocation(sphereProgram.GetProgramID(), "MVP");
	if (MVPLocationSP == GL_INVALID_VALUE || MVPLocationSP == GL_INVALID_OPERATION) {
		std::cerr << "Error cannot retrieve MVP uniform variable in OpenGL program" << std::endl;
	}

	auto colorLocation = glGetUniformLocation(sphereProgram.GetProgramID(), "sphereColor");

	// Wall program init
	GLProgram wallProgram;
	const std::string wallVSFile("../Resources/mesh.vert");
	const std::string wallFSFile("../Resources/mesh.frag");
	assert(wallProgram.AddShader(GL_VERTEX_SHADER, wallVSFile));
	assert(wallProgram.AddShader(GL_FRAGMENT_SHADER, wallFSFile));
	assert(wallProgram.LinkProgram());

	wallProgram.UseProgram();
	auto MVPLocationWP = glGetUniformLocation(wallProgram.GetProgramID(), "MVP");
	if (MVPLocationWP == GL_INVALID_VALUE || MVPLocationWP == GL_INVALID_OPERATION) {
		std::cerr << "Error cannot retrieve MVP uniform variable in OpenGL program" << std::endl;
	}

	// The red sphere
	Model characterModel;
	assert(characterModel.LoadModel("../Resources/agent.obj"));

	// The green sphere
	Model targetModel = characterModel;	
		
	target.position = glm::vec2(rand() % GRID_WIDTH - HALF_GRID_WIDTH, rand() % GRID_DEPTH - HALF_GRID_DEPTH);

	BlendedBehavior behavior({ { &seekBehavior_charac, 1.0f },{ &lookBehavior_charac, 1.0f } }, speed + 1.0f, glm::half_pi<float>());
	currentBehavior_charac = &behavior;
	characterAgent.SetAgentImpl(std::make_unique<SteeringAgentImpl>(currentBehavior_charac, characterSpeed));

	std::vector<BlendedBehavior*> targetGroups = {};
	BlendedBehavior targetBehavior({ { &wanderBehavior_target, 1.0f },{ &lookBehavior_target, 1.0f } }, speed + 1.0f, glm::half_pi<float>());
	currentBehavior_target = &targetBehavior;
	targetAgent.SetAgentImpl(std::make_unique<SteeringAgentImpl>(&targetBehavior, targetSpeed));

	// Bind behavior to UI
	gui.GenerateUI(currentBehavior_charac);

	// Light blue background color
	glClearColor(0.815f, 0.92f, 1.0f, 1.0f);

	// Handle elapsed time between frames
	TimeManager timeManager(glfwGetTime());

	// Application main loop
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gridProgram.UseProgram();
		// Pass MVP matrix to program use to draw the grid
		model = glm::mat4(1);
		glUniformMatrix4fv(MVPLocationGP, 1, GL_FALSE, glm::value_ptr(projection*view*model));
		grid.Render();

		sphereProgram.UseProgram();
		// Update agents behavior
		timeManager.UpdateTime(glfwGetTime());
		characterAgent.UpdateAgent((float)timeManager.GetDeltaTime());
		targetAgent.UpdateAgent((float)timeManager.GetDeltaTime());

		// Trim character position inside our grid
		TrimWorld(HALF_GRID_WIDTH, &character.position.x);
		TrimWorld(HALF_GRID_DEPTH, &character.position.y);

		TrimWorld(HALF_GRID_WIDTH, &target.position.x);
		TrimWorld(HALF_GRID_DEPTH, &target.position.y);

		// Pass apply updated character position to rendering
		model = glm::rotate(glm::translate(glm::mat4(1), glm::vec3(character.position.x, 0, character.position.y)),
							character.orientation, glm::vec3(0,1,0));
		glUniformMatrix4fv(MVPLocationSP, 1, GL_FALSE, glm::value_ptr(projection*view*model));
		glUniform3f(colorLocation, 1, 0, 0);
		characterModel.Render();
				
		// Pass apply updated target position to rendering
		model = glm::rotate(glm::translate(glm::mat4(1), glm::vec3(target.position.x, 0, target.position.y)),
							target.orientation, glm::vec3(0,1,0));
		glUniformMatrix4fv(MVPLocationSP, 1, GL_FALSE, glm::value_ptr(projection*view*model));
		glUniform3f(colorLocation, 0.04f, 0.43f, 0.0f);
		targetModel.Render();

		// Draw walls
		wallProgram.UseProgram();
		for (auto& w : walls) {
			model = glm::rotate(glm::translate(glm::mat4(1), glm::vec3(w.wallEntity.position.x, 0, w.wallEntity.position.y)),
								w.wallEntity.orientation, glm::vec3(0, 1, 0));
			glUniformMatrix4fv(MVPLocationWP, 1, GL_FALSE, glm::value_ptr(projection*view*model));
			w.wallModelPtr->Render();
		}

		// Draw nanogui
		gui.UpdateUI();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glUseProgram(0);

	glfwTerminate();

	return 0;
}