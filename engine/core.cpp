// Source file for core class.
// Core class handles all main functionality of the engine.
//
// Version: 3/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#include "core.h"
#include "debug.h"
#include "scenemanager.h"
#include "input.h"

Core* Core::instance; // The singleton instance

void Core::Initialize(int argc, char* argv[]) {
	Debug::Log("Initializing Engine");

	//Create singleton instance, if not already set
	if (!instance) {
		instance = new Core();
	}
	else return; // Else return as instance is already set, thus the engine is already initialized

	//Determine executable directory path
	std::string _exeDirArg = argv[0]; // Get the executable path directory from arguments
	std::size_t found = _exeDirArg.find_last_of("/\\"); // Get position of character
	instance->executableDirectoryPath = _exeDirArg.substr(0, found); // Cut off last part of path
	instance->executableDirectoryPath.append("\\"); // Append a slash to return the absolute directory

	//Create renderer
	instance->renderer = new Renderer();
	instance->renderer->Initialize(1280, 720, "Dustville");

	//Create Editor
	instance->editor = new Editor();

	//Initialize Input
	Input::Init(instance->renderer->GetWindow());

	instance->isActive = true;

	Debug::Log("Engine Initialized");
}

void Core::Update() {
	//Static time instances
	static float frames = 0;
	static float lastTime = (float)glfwGetTime();

	//Handle Time
	instance->deltaTime = instance->CalculateDeltaTime();
	instance->timeElapsed = (float)glfwGetTime();

	if (instance->timeElapsed >= lastTime + 1) {
		instance->fps = frames;
		frames = 0;
		lastTime = instance->timeElapsed;
	}

	//Update Entities
	if (SceneManager::GetActiveScene()) {
		//Update editor if active
		if (instance->editorActive) {
			instance->editor->Update();
		}

		//Update Scene
		SceneManager::GetActiveScene()->Update();
		instance->renderer->RenderFrame();
	}
	else {
		Debug::Log("No active scene - Press 'Left Control + Left Shift + =' To create empty scene with camera!");

		//Create empty scene with camera, if keys are pressed
		if (Input::GetKey(KEYCODE_LEFT_CONTROL) && Input::GetKey(KEYCODE_LEFT_SHIFT) && Input::GetKey(KEYCODE_EQUAL)) {
			SceneManager::SetActiveScene(new Scene());
			SceneManager::GetActiveScene()->SetActiveCamera(new Camera());
		}
	}
	
	//Update Input
	Input::HandleUpdates();

	//If window should not close, we poll events, swap buffers and clear, else we set active to false
	if (!glfwWindowShouldClose(instance->renderer->GetWindow())) {
		instance->renderer->PollEvents();
		instance->renderer->SwapBuffers();
		instance->renderer->Clear();
	}
	else {
		instance->isActive = false;
	}

	//Increment frames
	frames++;
}

bool Core::IsActive() {
	return instance->isActive;
}

void Core::Destroy() {
	//Terminate TextureLoader
	TextureLoader::Terminate();

	//Terminate renderer
	delete instance->renderer;

	//Delete scenemanager
	SceneManager::Terminate();

	delete instance; // Delete instance
}

std::string Core::GetExecutableDirectoryPath() {
	return instance->executableDirectoryPath;
}

Renderer* Core::GetRendererInstance() {
	return instance->renderer;
}

void Core::EnableEditor(bool state) {
	instance->editorActive = state;

	if (state) {
		Debug::Log("Editor enabled");
	}
	else {
		Debug::Log("Editor disabled");
	}
}

float Core::CalculateDeltaTime() {
	static float lastTime = (float)glfwGetTime();
	float currentTime = (float)glfwGetTime();

	float _deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	return _deltaTime;
}