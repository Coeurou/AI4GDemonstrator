#pragma once

#include <nanogui/nanogui.h>
#include "SteeringMovementBehavior.h"

class UI
{
public:
	UI();
	~UI();

	void InitUI(GLFWwindow* window);
	void GenerateUI(SteeringMovementBehavior* behavior);
	void UpdateUI();

	nanogui::Screen* GetScreen() { return &screen; }

private:
	nanogui::Screen screen;
	nanogui::FormHelper gui;
	nanogui::ref<nanogui::Window> nanoguiWindow;
};
