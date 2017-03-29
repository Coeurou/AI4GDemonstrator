#include "UI.h"

UI::UI() : gui(nanogui::FormHelper(&screen)), nanoguiWindow(nullptr)
{
}

UI::~UI()
{
}

void UI::InitUI(GLFWwindow* window)
{
	screen.initialize(window, true);
	gui = nanogui::FormHelper(&screen);

	screen.setVisible(true);
}

void UI::GenerateUI(SteeringMovementBehavior* behavior)
{
	auto tempWindow = gui.addWindow(Eigen::Vector2i(10, 10), behavior->ToString());

	if (nanoguiWindow) {
		screen.removeChild(nanoguiWindow.get());
	}
	nanoguiWindow = tempWindow;

	auto& parameters = behavior->GetParametersByRef();
	for (auto& param : parameters) {
		switch (param.second.tag) {
		case Parameter::FLOAT:
			gui.addVariable(param.first, param.second.floatValue)->setSpinnable(true);
			break;
		case Parameter::INT:
			gui.addVariable(param.first, param.second.intValue);
			break;
		default:
			break;
		}
	}
	screen.performLayout();
}

void UI::UpdateUI()
{
	screen.drawContents();
	screen.drawWidgets();
}
