// Source file for core class.
// Core class handles all main functionality of the engine
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#include "core.h"
#include "debug.h"
#include "scenemanager.h"
#include "input.h"
#include "soundmanager.h"
#include "luascript.h"
#include "editor.h"

#include "math/physics.h"

#include "component_register.h"

//Include steamworks api
#include <steam_api.h>
#include <steam_gameserver.h>

Core* Core::instance; // The singleton instance

void Core::Initialize(int argc, char* argv[], Vec2 resolution, std::string title) {
	Debug::Log("Initializing Engine");

	//Create singleton instance, if not already set
	if (!instance) {
		instance = new Core();
	}
	else return; // Else return as instance is already set, thus the engine is already initialized

	instance->requestExit = false;

	//Determine executable directory path
	std::string _exeDirArg = argv[0]; // Get the executable path directory from arguments
	std::size_t found = _exeDirArg.find_last_of("/\\"); // Get position of character
	instance->executableDirectoryPath = _exeDirArg.substr(0, found); // Cut off last part of path
	instance->executableDirectoryPath.append("\\"); // Append a slash to return the absolute directory

	//Create renderer
	instance->renderer = new Renderer(resolution, title.c_str());

	//Initialize variables
	instance->frame = 0;

	//Initialize Input
	Input::Init(instance->renderer->GetWindow());

	//Initialize SoundManager
	SoundManager::Init();

	//Initialize Steamworks API
	if (SteamAPI_Init()) {
		Debug::Log("Steam Initialized");
	}

	//Implement luascript native functions
	LuaScript::AddNativeFunction("Log", [](lua_State* state) -> int {
		Debug::Log(lua_tostring(state, -1));
		return 0;
	});

	//Runs a function from a native lua file from c++
	LuaScript::AddNativeFunction("RunFunction", [](lua_State* state) -> int {
		int top = -lua_gettop(state);

		std::string fileName = lua_tostring(state, top);
		std::string functionName = lua_tostring(state, top + 1);

		std::vector<std::string> params;
		for (size_t i = top + 2; i < 0; i++) {
			params.push_back(lua_tostring(state, i));
		}

		LuaScript::RunFunction(fileName, functionName, params);
		return 0;
	});

	LuaScript::AddNativeFunction("RequestExit", [](lua_State* state) -> int {
		instance->RequestExit();
		return 0;
	});

	Debug::Log("Engine Initialized");
}

void Core::Update() {
	//Static time instances
	static float frames = 0;
	static float lastTime = (float)glfwGetTime();

	//Handle Time
	instance->frame++;
	instance->deltaTime = instance->CalculateDeltaTime();
	instance->timeElapsed = (float)glfwGetTime();

	if (instance->timeElapsed >= lastTime + 1) {
		instance->fps = frames;
		frames = 0;
		lastTime = instance->timeElapsed;
	}

	//Update editor if active
	if (Editor::editorActive) {
		Editor::Update();

		Core::PauseGame();
	}
	else {
		Core::ContinueGame();
	}

	//Update Entities
	if (SceneManager::GetActiveScene()) {

		//Update Scene

		SceneManager::GetActiveScene()->UpdateScene(instance->gamePaused);

		if (SceneManager::GetActiveScene()->GetActiveCamera()) {
			instance->renderer->RenderScene(SceneManager::GetActiveScene(), SceneManager::GetActiveScene()->GetActiveCamera());

			//Render debug stuff
			for (size_t i = 0; i < Debug::GetLineDrawList().size(); i++) {
				Line line = Debug::GetLineDrawList()[i];
				instance->renderer->DrawLine(line.a, line.b, line.color, SceneManager::GetActiveScene()->GetActiveCamera());
			}

			for (size_t i = 0; i < Debug::GetTextDrawList().size(); i++) {
				DebugText text = Debug::GetTextDrawList()[i];
				instance->renderer->RenderText(text.font, text.text, text.position, text.size, text.color);
			}
		}
		else {
			Debug::Log("SceneManager has no active camera");
		}
	}
	else {
		Debug::Log("SceneManager has no active scene, cannot update");
	}

	if (Debug::consoleActive) {
		Debug::ConstructConsole();
	}

	instance->renderer->RenderImGui();
	
	//Check if we should enable/disable console
	if (Input::GetKeyDown(KEYCODE_GRAVE_ACCENT)) {
		if (!Debug::consoleActive)
			Debug::consoleActive = true;
		else
			Debug::consoleActive = false;
	}

	//Update soundmanager
	SoundManager::Update();

	//Update Input
	Input::HandleUpdates();

	//If window should not close, we poll events, swap buffers and clear, else we set active to false
	if (!glfwWindowShouldClose(instance->renderer->GetWindow()) && !instance->requestExit) {
		instance->renderer->PollEvents();
		instance->renderer->SwapBuffers();
		instance->renderer->Clear();

		//Clear debug
		Debug::Clear();

		//Execute late frame functions
		for (size_t i = 0; i < instance->LateFrameFunctionList.size(); i++) {
			instance->LateFrameFunctionList[i]();
			instance->LateFrameFunctionList.erase(instance->LateFrameFunctionList.begin() + i);
		}
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

	TextureLoader::Terminate();
	Input::Terminate();
	delete instance->renderer;

	SceneManager::Terminate();
	SoundManager::Destroy();

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

void Core::RequestExit() {
	instance->requestExit = true;
}

void Core::PauseGame() {
	instance->gamePaused = true;
}

void Core::ContinueGame() {
	instance->gamePaused = false;
}

void Core::ExecuteLateFrame(std::function<void()> func) {
	instance->LateFrameFunctionList.push_back(func);
}

float Core::CalculateDeltaTime() {
	static float lastTime = (float)glfwGetTime();
	float currentTime = (float)glfwGetTime();

	float _deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	return _deltaTime;
}