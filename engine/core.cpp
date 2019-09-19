// Source file for core class.
// Core class handles all main functionality of the engine.
//
// Version: 19/9/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, September 2019
#include "core.h"
#include "debug.h"
#include "scenemanager.h"
#include "input.h"
#include "luascript.h"
#include "math/physics.h"

//Include steamworks api
#include <steam_api.h>
#include <steam_gameserver.h>


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
	instance->renderer = new Renderer(Vec2(1280, 720), Vec2(1280, 720), "Dustville");

	//Set sceneToBeLoaded to a nullptr value
	instance->sceneToBeLoaded = nullptr;

	//Initialize Input
	Input::Init(instance->renderer->GetWindow());

	//Initialize Steamworks API
	if (SteamAPI_Init()) {
		Debug::Log("Steam Initialized");
	}

	Debug::Log("Engine Initialized");


	//Implement luascript native functions
	LuaScript::AddNativeFunction("Log", [](lua_State* state) -> int {
			Debug::Log(lua_tostring(state, -1));
			return 0;
		});
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

	if (instance->sceneToBeLoaded != nullptr) {
		delete SceneManager::GetActiveScene();
		SceneManager::SetActiveScene(instance->sceneToBeLoaded);
		instance->sceneToBeLoaded = nullptr;
	}

	//Update Entities
	if (SceneManager::GetActiveScene()) {
		//Update Scene
		SceneManager::GetActiveScene()->Update();
		instance->renderer->RenderFrame();
	}
	else {
		Debug::Log("No active scene");
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
		Core::Destroy();
	}

	//Increment frames
	frames++;
}

bool Core::IsActive() {
	return (instance != nullptr) ? true : false;
}

void Core::Destroy() {
	if (!IsActive()) return; // Instance already destroyed

	//Terminate TextureLoader
	TextureLoader::Terminate();

	//Terminate renderer
	delete instance->renderer;

	//Delete scenemanager
	SceneManager::Terminate();

	// Delete instance and set instance to nullptr
	delete instance; 
	instance = nullptr;
}

std::string Core::GetExecutableDirectoryPath() {
	return instance->executableDirectoryPath;
}

Renderer* Core::GetRendererInstance() {
	return instance->renderer;
}

void Core::SwitchScene(Scene* scene) {
	instance->sceneToBeLoaded = scene;
}

float Core::CalculateDeltaTime() {
	static float lastTime = (float)glfwGetTime();
	float currentTime = (float)glfwGetTime();

	float _deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	return _deltaTime;
}