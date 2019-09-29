// Source file for core class.
// Core class handles all main functionality of the engine
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, September 2019
#include "core.h"
#include "debug.h"
#include "scenemanager.h"
#include "input.h"
#include "soundmanager.h"
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

	//Initialize variables
	instance->sceneToBeLoaded = nullptr;
	instance->frame = 0;

	//Initialize Input
	Input::Init(instance->renderer->GetWindow());

	//Initialize SoundManager
	SoundManager::Init();

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

	//Implement Lua functions for UI Handling

	//The pointer to the current element being edited
	static Entity* _curEntity = nullptr;

	LuaScript::AddNativeFunction("BeginEntity", [](lua_State* state) -> int {
		//Fetch params
		std::string path = lua_tostring(state, -lua_gettop(state));
			
		if (!SceneManager::GetActiveScene())  return 0;

		//Load Texture
		Texture* texture = TextureLoader::LoadTarga((char*)path.c_str());
		if (!texture) return 0;

		Entity* entity;
		if (std::string(lua_tostring(state, -lua_gettop(state) + 1)) == "UI") {
			entity = new UIElement();
		}
		else {
			entity = new Entity();
		}
			
		//When created from lua, a sprite is automaticly added
		entity->AddComponent<Sprite>()->SetTexture(texture);

		if (_curEntity) {
			_curEntity->AddChild(entity);
		}
		else {
			SceneManager::GetActiveScene()->AddChild(entity);
		}

		//Set Current element
		_curEntity = entity;

		return 0;
	});

	LuaScript::AddNativeFunction("BeginExistingEntityByTag", [](lua_State* state) -> int {
		if (_curEntity != nullptr) { Debug::Log("Lua: Cannot get existing entity if _curEnemy is not nullptr"); return 0; }

		std::string tag = std::string(lua_tostring(state, -1));
		Entity* entity = SceneManager::GetActiveScene()->GetChildByTag(tag);

		if(!entity) {
			Debug::Log("Lua: Cannot find entity with tag: " + tag);
		}
		else {
			_curEntity = entity;
		}
		return 0;
	});

	LuaScript::AddNativeFunction("EndEntity", [](lua_State* state) -> int {
		if (_curEntity) {
			if (_curEntity->GetParent() && !dynamic_cast<Scene*>(_curEntity->GetParent())) {
				_curEntity = _curEntity->GetParent();
			}
			else {
				_curEntity = nullptr;
			}
		}

		return 0;
	});

	LuaScript::AddNativeFunction("SetTag", [](lua_State* state) -> int {
		_curEntity->tag = std::string(lua_tostring(state, -1));
	});

	//Position is always local
	LuaScript::AddNativeFunction("SetPosition", [](lua_State* state) -> int {
		Vec2 position = Vec2((float)lua_tonumber(state, -2), (float)lua_tonumber(state, -1));
		_curEntity->localPosition = position;
		return 0;
	});

	LuaScript::AddNativeFunction("SetScale", [](lua_State* state) -> int {
		Vec2 scale = Vec2((float)lua_tonumber(state, -2), (float)lua_tonumber(state, -1));
		_curEntity->localScale = scale;
		return 0;
	});

	LuaScript::AddNativeFunction("Split", [](lua_State* state) -> int {
		_curEntity->GetComponent<Sprite>()->Split((int)lua_tonumber(state, -2), (int)lua_tonumber(state, -1));
		return 0;
	});

	LuaScript::AddNativeFunction("OnEnter", [](lua_State* state) ->int {
		int top = -lua_gettop(state);

		std::string filePath = lua_tostring(state, top);
		std::string funcName = lua_tostring(state, top + 1);

		std::vector<std::string> params;
		for (int i = top + 2; i < 0; i++) {
			params.push_back(lua_tostring(state, i));
		}

		dynamic_cast<UIElement*>(_curEntity)->OnEnterDelegate.AddLambda([=]() {
			LuaScript::RunFunction(filePath, funcName, params);
		});
		return 0;
	});

	LuaScript::AddNativeFunction("OnStay", [](lua_State* state) ->int {
		int top = -lua_gettop(state);

		std::string filePath = lua_tostring(state, top);
		std::string funcName = lua_tostring(state, top + 1);

		std::vector<std::string> params;
		for (int i = top + 2; i < 0; i++) {
			params.push_back(lua_tostring(state, i));
		}

		dynamic_cast<UIElement*>(_curEntity)->OnStayDelegate.AddLambda([=]() {
			LuaScript::RunFunction(filePath, funcName, params);
		});
		return 0;
	});

	LuaScript::AddNativeFunction("OnLeave", [](lua_State* state) ->int {
		int top = -lua_gettop(state);

		std::string filePath = lua_tostring(state, top);
		std::string funcName = lua_tostring(state, top + 1);

		std::vector<std::string> params;
		for (int i = top + 2; i < 0; i++) {
			params.push_back(lua_tostring(state, i));
		}

		dynamic_cast<UIElement*>(_curEntity)->OnLeaveDelegate.AddLambda([=]() {
			LuaScript::RunFunction(filePath, funcName, params);
		});
		return 0;
	});

	//Implement lua native sound functions
	LuaScript::AddNativeFunction("PlaySound", [](lua_State* state) -> int {
		Sound* sound = new Sound();

		sound->LoadAudioSource(lua_tostring(state, -3));

		if (!sound->GetAudioSource()) {
			delete sound;
			return 0;

		}
		sound->SetGain((float)lua_tonumber(state, -2));
		sound->SetPitch((float)lua_tonumber(state, -1));

		SoundManager::AddSound(sound);
		SoundManager::PlaySound(sound);

		return 0;
	});
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

	if (instance->sceneToBeLoaded != nullptr) {
		delete SceneManager::GetActiveScene();
		SceneManager::SetActiveScene(instance->sceneToBeLoaded);
		instance->sceneToBeLoaded = nullptr;
	}

	//Update Entities
	if (SceneManager::GetActiveScene() && SceneManager::GetActiveScene()->GetActiveCamera()) {
		//Update Scene
		SceneManager::GetActiveScene()->Update();
		instance->renderer->RenderScene(SceneManager::GetActiveScene(), SceneManager::GetActiveScene()->GetActiveCamera());
	}
	else {
		Debug::Log("No active scene or camera present");
	}
	
	//Update Input
	Input::HandleUpdates();

	//Update soundmanager
	SoundManager::Update();

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