// Source file for core class.
// Core class handles all main functionality of the engine.
//
// Version: 3/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, March 2019
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
	instance->renderer->Initialize(800, 600, "Dustville");

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

	//Update Input
	Input::HandleUpdates();

	//Update Entities
	if (SceneManager::GetActiveScene()) {
		//Update Scene
		SceneManager::GetActiveScene()->Update();
		instance->renderer->RenderFrame();
	}
	
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

	delete instance; // Delete instance
}

std::string Core::GetExecutableDirectoryPath() {
	return instance->executableDirectoryPath;
}

Renderer* Core::GetRendererInstance() {
	return instance->renderer;
}

float Core::CalculateDeltaTime() {
	static float lastTime = (float)glfwGetTime();
	float currentTime = (float)glfwGetTime();

	float _deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	return _deltaTime;
}