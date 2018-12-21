#include "xbox.h"

std::vector<Xbox*> Xbox::controllers(4);

Xbox::Xbox(int id) {
	this->id = id;
	buttons.resize(XBOX_BUTTONS);
	axes.resize(XBOX_AXES);
}

void Xbox::init() {
	for (int i = 0; i < 4; i++) {
		controllers[i] = new Xbox(i);
	}
}

bool Xbox::bounds(int controller) {
	return controller >= 0 && controller < XBOX_CONTROLLERS;
}

bool Xbox::buttonBounds(int button) {
	return button >= 0 && button < XBOX_BUTTONS;
}

bool Xbox::axisBounds(int axis) {
	return axis >= 0 && axis < XBOX_AXES;
}

void Xbox::setButton(int controller, int button, bool state) {
	if (bounds(controller) && buttonBounds(button)) {
		controllers[controller]->buttons[button] = state;
	}
}

void Xbox::setAxis(int controller, int axis, float pos) {
	if (bounds(controller) && axisBounds(axis)) {
		controllers[controller]->axes[axis] = pos;
	}
}

bool Xbox::buttonDown(int controller, int button) {
	if (bounds(controller) && buttonBounds(button)) {
		return controllers[controller]->buttons[button];
	}
	return false;
}

float Xbox::getAxis(int controller, int axis) {
	if (bounds(controller) && axisBounds(axis)) {
		float val = controllers[controller]->axes[axis];
		if (-XBOX_DEADZONE <= val && val <= XBOX_DEADZONE) {
			return 0.0f;
		} else {
			return val;
		}
	}
	return 0.0f;
}

void Xbox::setConnected(int controller, bool state) {
	if (bounds(controller)) {
		controllers[controller]->connected = state;
	}
}

bool Xbox::isConnected(int controller) {
	if (bounds(controller)) {
		return controllers[controller]->connected;
	}
	return false;
}

bool Xbox::buttonDown(int button) {
	for (int i = 0; i < controllers.size(); i++) {
		if (isConnected(i) && buttonDown(i, button)) {
			return true;
		}
	}
	return false;
}

float Xbox::getAxis(int axis) {
	for (int i = 0; i < controllers.size(); i++) {
		if (isConnected(i)) {
			return getAxis(i, axis);
		}
	}
	return 0.0f;
}

void Xbox::clearInputs() {
	for (int i = 0; i < controllers.size(); i++) {
		for (int j = 0; j < controllers[i]->buttons.size(); j++) {
			controllers[i]->buttons[j] = false;
		}
		for (int j = 0; j < controllers[i]->axes.size(); j++) {
			controllers[i]->axes[j] = 0;
		}
	}
}

void Xbox::clearInputs(int controller) {
	if (bounds(controller)) {
		for (int i = 0; i < controllers[controller]->buttons.size(); i++) {
			controllers[controller]->buttons[i] = false;
		}
		for (int i = 0; i < controllers[controller]->axes.size(); i++) {
			controllers[controller]->axes[i] = 0;
		}
	}
}