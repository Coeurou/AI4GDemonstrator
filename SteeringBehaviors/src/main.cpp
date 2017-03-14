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
#include "Grid.h"
#include "GLProgram.h"
#include "Model.h"
#include "TimeManager.h"
#include "Agent.h"
#include "SteeringAgentImpl.h"
#include "SpatialStructure.h"
#include "SteeringSeekBehavior.h"
#include "SteeringFleeBehavior.h"
#include "SteeringArriveBehavior.h"
#include "SteeringEvadeBehavior.h"
#include "SteeringPursueBehavior.h"
#include "SteeringWanderBehavior.h"

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

auto speed = 4.0f;
auto characterSpeed = 4.0f;
auto targetSpeed = 4.0f;
auto prediction = 1.0f;
auto wanderRadius = 1.0f;
auto wanderOffset = 2.0f;
auto wanderRate = glm::half_pi<float>();

auto FOV = -45.0f;

std::unique_ptr<nanogui::Screen> screen = std::make_unique<nanogui::Screen>();

glm::mat4 model = glm::mat4(1);
glm::mat4 view = glm::lookAt(glm::vec3(0, 100, -7), glm::vec3(0), glm::vec3(0, 1, 0));
glm::mat4 projection = glm::perspective(glm::radians(FOV), WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 1000.0f);

// Location of our AI agents
SpatialStructure character;
SpatialStructure target;

Agent characterAgent(nullptr);
Agent targetAgent(nullptr);

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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action > GLFW_RELEASE) {
		switch (key) {
		// Character behavior management
		case GLFW_KEY_Q:
			characterAgent.SetAgentImpl(new SteeringAgentImpl(new SteeringSeekBehavior(&character, &target, speed), characterSpeed));
			break;
		case GLFW_KEY_B:
			characterAgent.SetAgentImpl(new SteeringAgentImpl(new SteeringFleeBehavior(&character, &target, speed), characterSpeed));
			break;
		case GLFW_KEY_C:
			characterAgent.SetAgentImpl(new SteeringAgentImpl(new SteeringPursueBehavior(&character, &target, speed, prediction), characterSpeed));
			break;
		case GLFW_KEY_D:
			characterAgent.SetAgentImpl(new SteeringAgentImpl(new SteeringEvadeBehavior(&character, &target, speed, prediction), characterSpeed));
			break;
		case GLFW_KEY_E:
			characterAgent.SetAgentImpl(new SteeringAgentImpl(new SteeringWanderBehavior(&character, speed, wanderRadius, wanderOffset, wanderRate), characterSpeed));
			break;
		case GLFW_KEY_F:
			characterAgent.SetAgentImpl(new SteeringAgentImpl(new SteeringArriveBehavior(&character, &target), characterSpeed));
			break;
		// Target behavior management
		case GLFW_KEY_G:
			targetAgent.SetAgentImpl(new SteeringAgentImpl(new SteeringSeekBehavior(&target, &character, speed), targetSpeed));
			break;
		case GLFW_KEY_H:
			targetAgent.SetAgentImpl(new SteeringAgentImpl(new SteeringFleeBehavior(&target, &character, speed), targetSpeed));
			break;
		case GLFW_KEY_I:
			targetAgent.SetAgentImpl(new SteeringAgentImpl(new SteeringPursueBehavior(&target, &character, speed, prediction), targetSpeed));
			break;
		case GLFW_KEY_J:
			targetAgent.SetAgentImpl(new SteeringAgentImpl(new SteeringEvadeBehavior(&target, &character, speed, prediction), targetSpeed));
			break;
		case GLFW_KEY_K:
			targetAgent.SetAgentImpl(new SteeringAgentImpl(new SteeringWanderBehavior(&target, speed, wanderRadius, wanderOffset, wanderRate), targetSpeed));
			break;
		case GLFW_KEY_L:
			targetAgent.SetAgentImpl(new SteeringAgentImpl(new SteeringArriveBehavior(&target, &character), targetSpeed));
			break;
		default:
			break;
		}
	}	
	screen->keyCallbackEvent(key, scancode, action, mods);
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	projection = glm::perspective(glm::radians(FOV), WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
	screen->resizeCallbackEvent(width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	FOV += (float)(yoffset * 0.5f);
	projection = glm::perspective(glm::radians(FOV), WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
	screen->scrollCallbackEvent(xoffset, yoffset);
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
	if (!window)
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
	===================================================
	void InitGUI(GLFWWindow* window)

	Init NanoGUI capabilities
	==================================================
	*/
	screen->initialize(window, true);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	std::unique_ptr<nanogui::FormHelper> gui = std::make_unique<nanogui::FormHelper>(screen.get());
	nanogui::ref<nanogui::Window> nanoguiWindow = gui->addWindow(Eigen::Vector2i(10,10), "Steering Behaviors");

	gui->addGroup("Agent");
	gui->addVariable("Character Max Speed", characterSpeed)->setSpinnable(true);
	gui->addVariable("Target Max Speed", targetSpeed)->setSpinnable(true);


	gui->addGroup("GLOBAL");
	gui->addVariable("Speed", speed)->setSpinnable(true);

	gui->addGroup("Pursue/Evade");
	gui->addVariable("Prediction", prediction)->setSpinnable(true);

	gui->addGroup("Wander");
	gui->addVariable("Rate", wanderRate)->setSpinnable(true);
	gui->addVariable("Offset", wanderOffset)->setSpinnable(true);
	gui->addVariable("Radius", wanderRadius)->setSpinnable(true);

	screen->setVisible(true);
	screen->performLayout();

	/*
	=======================================================
	void InitGLFWCallback

	Assign glfw callback for window, keyboard, mouse events
	=======================================================
	*/
	glfwSetKeyCallback(window, key_callback);

	glfwSetMouseButtonCallback(window, 
		[](GLFWwindow *, int button, int action, int modifiers) {
			screen->mouseButtonCallbackEvent(button, action, modifiers);
		}
	);
	glfwSetCursorPosCallback(window,
		[](GLFWwindow *, double x, double y) {
			screen->cursorPosCallbackEvent(x, y);
		}
	);
	glfwSetCharCallback(window,
		[](GLFWwindow *, unsigned int codepoint) {
			screen->charCallbackEvent(codepoint);
		}
	);

	glfwSetDropCallback(window,
		[](GLFWwindow *, int count, const char **filenames) {
			screen->dropCallbackEvent(count, filenames);
		}
	);

	glfwSetScrollCallback(window,
		[](GLFWwindow *, double x, double y) {
			screen->scrollCallbackEvent(x, y);
		}
	);

	glfwSetFramebufferSizeCallback(window,
		[](GLFWwindow *, int width, int height) {
			screen->resizeCallbackEvent(width, height);
		}
	);

	glfwSetWindowSizeCallback(window, window_size_callback);

	glfwSetScrollCallback(window, scroll_callback);

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

	// Const variables
	const size_t GRID_WIDTH = 120;
	const size_t GRID_DEPTH = 80;

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
	gridProgram.LinkProgram();
	
	gridProgram.UseProgram();
	auto MVPLocation = glGetUniformLocation(gridProgram.GetProgramID(), "MVP");
	if (MVPLocation == GL_INVALID_VALUE || MVPLocation == GL_INVALID_OPERATION) {
		std::cerr << "Error cannot retrieve MVP uniform variable in OpenGL program" << std::endl;
	}

	// Pass MVP matrix to program use to draw the grid
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(projection*view*model));

	// Sphere program init
	GLProgram sphereProgram;
	assert(sphereProgram.AddShader(GL_VERTEX_SHADER, vsSphereSource));
	assert(sphereProgram.AddShader(GL_FRAGMENT_SHADER, fsSphereSource));
	sphereProgram.LinkProgram();

	sphereProgram.UseProgram();
	MVPLocation = glGetUniformLocation(sphereProgram.GetProgramID(), "MVP");
	if (MVPLocation == GL_INVALID_VALUE || MVPLocation == GL_INVALID_OPERATION) {
		std::cerr << "Error cannot retrieve MVP uniform variable in OpenGL program" << std::endl;
	}

	auto colorLocation = glGetUniformLocation(sphereProgram.GetProgramID(), "sphereColor");

	// The red sphere
	Model characterModel;
	assert(characterModel.LoadModel("../Resources/agent.obj"));

	// The green sphere
	Model targetModel = characterModel;

	// Const variables used for AI init & update
	const float HALF_GRID_WIDTH = GRID_WIDTH * 0.5f;
	const float HALF_GRID_DEPTH = GRID_DEPTH * 0.5f;

	
	target.position = glm::vec2(rand() % GRID_WIDTH - HALF_GRID_WIDTH, rand() % GRID_DEPTH - HALF_GRID_DEPTH);

	// Agents & behaviors allocation
	characterAgent.SetAgentImpl(new SteeringAgentImpl(new SteeringPursueBehavior(&character, &target, speed, 1.0f),
								speed));
	targetAgent.SetAgentImpl(new SteeringAgentImpl(new SteeringWanderBehavior(&target, speed, 1.0f, 2.0f, glm::half_pi<float>()),
							 speed));

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
		glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(projection*view*model));
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
		glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(projection*view*model));
		glUniform3f(colorLocation, 1, 0, 0);
		characterModel.Render();
		
		// Pass apply updated target position to rendering
		model = glm::rotate(glm::translate(glm::mat4(1), glm::vec3(target.position.x, 0, target.position.y)),
							target.orientation, glm::vec3(0,1,0));
		glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(projection*view*model));
		glUniform3f(colorLocation, 0.04f, 0.43f, 0.0f);
		targetModel.Render();

		// Draw nanogui
		screen->drawContents();
		screen->drawWidgets();
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glUseProgram(0);

	glfwTerminate();

	return 0;
}