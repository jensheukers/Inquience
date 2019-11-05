// Source file for Input class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, November 2019

//Include input.h and glfw3.h

#include "input.h"
#include "GLFW/glfw3.h"

KeyEvent::KeyEvent(int keyCode, KeyEvent_Type type) {
	this->keyCode = keyCode;
	this->type = type;
}

bool KeyEvent::Check() {
	switch (this->type) {
	case KeyEvent_Type::Get:
		if (Input::GetKey(this->keyCode)) return true;
		break;
	case KeyEvent_Type::GetDown:
		if (Input::GetKeyDown(this->keyCode)) return true;
		break;
	case KeyEvent_Type::GetUp:
		if (Input::GetKeyUp(this->keyCode)) return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

KeyComboEvent::KeyComboEvent(KeyCombo combo) {
	this->combo = combo;
}

bool KeyComboEvent::Check() {
	for (size_t i = 0; i < this->combo.size(); i++) {
		if (!this->combo[i].Check()) return false;
	}

	this->onActivate.Execute();
	return true;
}

Axis::Axis(int positiveKey, int negativeKey) {
	this->positiveKey = positiveKey;
	this->negativeKey = negativeKey;
}

float Axis::GetValue() {
	float value = 0;
	if (Input::GetKey(positiveKey)) value += 1;
	if (Input::GetKey(negativeKey)) value -= 1;
	return value;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (action) {
	case GLFW_PRESS: // On Key press
		Input::SetKey(key, true); // Set to true
		break;
	case GLFW_RELEASE: // On Key Release
		Input::SetKey(key, false); // Set to false
		break;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	switch (action) {
	case GLFW_PRESS: // On Key press
		Input::SetButton(button, true); // Set to true
		break;
	case GLFW_RELEASE: // On Key Release
		Input::SetButton(button, false); // Set to false
		break;
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Input::SetMousePos(Vec2((float)xpos, (float)ypos));
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Input::SetScrollOffset(Vec2((float)xoffset, (float)yoffset));
}

Input* Input::_instance; // Declare static member

Input* Input::GetInstance() {
	if (!_instance) {
		_instance = new Input();
	}
	
	return _instance;
}

void Input::HandleUpdates() {
	// Set Key Last
	for (size_t i = 0; i < Input::GetInstance()->_keys.size(); i++) {
		GetInstance()->_keysLast[i] = GetInstance()->_keys[i];
	}

	for (size_t i = 0; i < Input::GetInstance()->_buttons.size(); i++) {
		GetInstance()->_buttonsLast[i] = GetInstance()->_buttons[i];
	}

	//Reset scrolloffset
	GetInstance()->_scrollOffset = Vec2(0);

	GetInstance()->lastKey = KEYCODE_EMPTY_KEY;
}

void Input::Init(GLFWwindow* window) {
	Input* instance = GetInstance();
	glfwSetKeyCallback(window, key_callback); // Set key callback
	glfwSetMouseButtonCallback(window, mouse_button_callback); // Set button callback
	glfwSetCursorPosCallback(window, cursor_position_callback); // Set mouse pos callback
	glfwSetScrollCallback(window, scroll_callback);
}

bool Input::GetKeyDown(int keyCode) {
	return GetInstance()->_keys[keyCode] && !GetInstance()->_keysLast[keyCode];
}

bool Input::GetKey(int keyCode) {
	return GetInstance()->_keys[keyCode] && GetInstance()->_keysLast[keyCode];
}

bool Input::GetKeyUp(int keyCode) {
	return !GetInstance()->_keys[keyCode] && GetInstance()->_keysLast[keyCode];
}

void Input::SetKey(int keyCode, bool state) {
	Input::GetInstance()->_keys[keyCode] = state;
	if (state == true) {
		Input::GetInstance()->lastKey = keyCode;
	}
}

bool Input::GetButtonDown(int buttonCode) {
	return GetInstance()->_buttons[buttonCode] && !GetInstance()->_buttonsLast[buttonCode];
}

bool Input::GetButton(int buttonCode) {
	return GetInstance()->_buttons[buttonCode] && GetInstance()->_buttonsLast[buttonCode];
}

bool Input::GetButtonUp(int buttonCode) {
	return !GetInstance()->_buttons[buttonCode] && GetInstance()->_buttonsLast[buttonCode];
}

void Input::SetButton(int buttonCode, bool state) {
	GetInstance()->_buttons[buttonCode] = state;
}

void Input::SetMousePos(Vec2 point) {
	GetInstance()->_mousePos = point;
}

Vec2 Input::GetMousePosition() {
	return GetInstance()->_mousePos;
}

void Input::SetScrollOffset(const Vec2 offset) {
	GetInstance()->_scrollOffset = offset;
}

const Vec2 Input::GetScrollOffset() {
	return GetInstance()->_scrollOffset;
}

int Input::GetLastKey() {
	return GetInstance()->lastKey;
}

bool Input::CheckCombo(KeyComboEvent comboEvent) {
	return comboEvent.Check();
}

void Input::AddAxis(std::string name, Axis* axis) {
	GetInstance()->_axises[name] = axis;
}

const float Input::GetAxis(std::string name) {
	return GetInstance()->_axises[name]->GetValue();
}

void Input::Terminate() {
	for (std::map<std::string, Axis*>::iterator it = GetInstance()->_axises.begin(); it != GetInstance()->_axises.end(); ++it) {
		delete it->second;
	}

	delete GetInstance();
}