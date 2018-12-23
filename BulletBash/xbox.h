#pragma once

#include "const.h"

#define XBOX_A sf::Joystick::X
#define XBOX_X sf::Joystick::Z
#define XBOX_B sf::Joystick::Y
#define XBOX_Y sf::Joystick::R
#define XBOX_LB sf::Joystick::U
#define XBOX_RB sf::Joystick::V
#define XBOX_BACK sf::Joystick::PovX
#define XBOX_START sf::Joystick::PovY
#define XBOX_LDOWN 8
#define XBOX_RDOWN 9
#define XBOX_LX sf::Joystick::Axis::X
#define XBOX_LY sf::Joystick::Axis::Y
#define XBOX_RX sf::Joystick::Axis::U
#define XBOX_RY sf::Joystick::Axis::R
#define XBOX_TRIGGERS sf::Joystick::Axis::Z
#define XBOX_DX sf::Joystick::Axis::PovX
#define XBOX_DY sf::Joystick::Axis::PovY

#define XBOX_CONTROLLERS 4
#define XBOX_BUTTONS 10
#define XBOX_AXES 8

#define XBOX_DEADZONE 15.0f

class Xbox {
private:
	static std::vector<Xbox*> controllers;

	int id;
	bool connected;
	std::vector<bool> buttons;
	std::vector<float> axes;

	static bool bounds(int controller);
	static bool buttonBounds(int button);
	static bool axisBounds(int axis);

public:
	Xbox(int id);

	static void init();
	static void setButton(int controller, int button, bool state);
	static bool buttonDown(int controller, int button);
	static void setAxis(int controller, int axis, float pos);
	static float getAxis(int controller, int axis, bool deadzone=true);
	static void setConnected(int controller, bool state);
	static bool isConnected(int controller);

	static bool buttonDown(int button);
	static float getAxis(int axis);

	static void clearInputs(int controller);
	static void clearInputs();
};

