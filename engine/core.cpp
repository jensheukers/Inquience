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

	Debug::Log("Engine Initialized");

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

	//Creates new scene and sets it active, unloads scene if one is already loaded
	LuaScript::AddNativeFunction("NewScene", [](lua_State* state) -> int {
		if (SceneManager::GetActiveScene()) {
			delete SceneManager::GetActiveScene();
		}
		SceneManager::SetActiveScene(new Scene());
		if (lua_toboolean(state, -1)) {
			if (lua_toboolean(state, -1)) SceneManager::GetActiveScene()->SetActiveCamera(new Camera());
		}

		return 0;
	});

	//Sets camera position in scene
	LuaScript::AddNativeFunction("SetCameraPosition", [](lua_State* state) -> int {
		if (SceneManager::GetActiveScene() && SceneManager::GetActiveScene()->GetActiveCamera()) {
			SceneManager::GetActiveScene()->GetActiveCamera()->SetPosition(Vec2((float)lua_tonumber(state, -2), (float)lua_tonumber(state, -1)));
		}
		return 0;
	});

	//Implement Lua functions for UI Handling

	//The pointer to the current element being edited
	static Entity* _curEntity = nullptr;

	LuaScript::AddNativeFunction("BeginEntity", [](lua_State* state) -> int {
		if (!SceneManager::GetActiveScene())  return 0;

		Entity* entity = new Entity();
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
		return 0;
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

	LuaScript::AddNativeFunction("OnEnter", [](lua_State* state) ->int {
		int top = -lua_gettop(state);

		std::string filePath = lua_tostring(state, top);
		std::string funcName = lua_tostring(state, top + 1);

		std::vector<std::string> params;
		for (int i = top + 2; i < 0; i++) {
			params.push_back(lua_tostring(state, i));
		}

		_curEntity->GetComponent<UIComponent>()->OnEnterDelegate.AddLambda([=]() {
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

		_curEntity->GetComponent<UIComponent>()->OnStayDelegate.AddLambda([=]() {
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

		_curEntity->GetComponent<UIComponent>()->OnLeaveDelegate.AddLambda([=]() {
			LuaScript::RunFunction(filePath, funcName, params);
		});
		return 0;
	});

	//Components for entities
	static Component* _curComponent;
	LuaScript::AddNativeFunction("BeginComponent", [](lua_State* state) ->int {
		if (!_curEntity) {
			Debug::Log("No active entity, cannot add component");
			return 0;
		}

		if (Component* component = Component_Register::GetNewComponentInstance("class " + std::string(lua_tostring(state, -1)))) {
			_curEntity->AddExistingComponentInstance(component);
			_curComponent = component;
		}

		return 0;
	});

	LuaScript::AddNativeFunction("SetProperty", [](lua_State* state) ->int {
		if (!_curComponent) return 0;
		int top = -lua_gettop(state);
		std::string propertyName = lua_tostring(state, top);

		std::vector<std::string> params;
		for (int i = top + 1; i < 0; i++) {
			params.push_back(lua_tostring(state, i));
		}

		_curComponent->SetProperty(propertyName, params);
		return 0;
	});

	LuaScript::AddNativeFunction("EndComponent", [](lua_State* state) ->int {
		if (_curComponent) _curComponent = nullptr;
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


	//Editor can be enabled/disabled by calling this function
	LuaScript::AddNativeFunction("Editor", [](lua_State* state) -> int {
		Editor::editorActive = !Editor::editorActive;
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

	//Update editor if active
	if (Editor::editorActive) {
		Editor::Update();
	}

	//Update Entities
	if (SceneManager::GetActiveScene()) {

		//Update Scene
		SceneManager::GetActiveScene()->Update();


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
	if (!glfwWindowShouldClose(instance->renderer->GetWindow())) {
		instance->renderer->PollEvents();
		instance->renderer->SwapBuffers();
		instance->renderer->Clear();

		//Clear debug
		Debug::Clear();
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

float Core::CalculateDeltaTime() {
	static float lastTime = (float)glfwGetTime();
	float currentTime = (float)glfwGetTime();

	float _deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	return _deltaTime;
}