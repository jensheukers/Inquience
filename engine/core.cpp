// Source file for core class.
// Core class handles all main functionality of the engine.
//
// Version: 6/8/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, August 2019
#include "core.h"
#include "debug.h"
#include "scenemanager.h"
#include "input.h"
#include "math/physics.h"

//Include steamworks api
#include <steam_api.h>
#include <steam_gameserver.h>

void Grid::Construct() {
	//Empty the gridtiles list, if already filled
	for (int i = 0; i < (int)gridTiles.size(); i++) {
		gridTiles.erase(gridTiles.begin(), gridTiles.begin() + i);
	}

	for (int x = 0; x <= scale.x * tileScale.x; x += (int)tileScale.x) {
		for (int y = 0; y <= scale.y * tileScale.y; y += (int)tileScale.y) {
			GridTile tile;
			tile.position = Vec2((float)x, (float)y);
			tile.occupied = false;
			gridTiles.push_back(tile);
		}
	}
}

GridTile* Grid::GetTilePosition(Vec2 position) {
	for (int i = 0; i < (int)gridTiles.size(); i++) {
		if (Physics::InBounds(position, gridTiles[i].position, gridTiles[i].position + tileScale)) {
			return &gridTiles[i];
		}
	}

	return nullptr;
}

void Grid::Clear() {
	for (size_t i = 0; i < gridTiles.size(); i++) {
		gridTiles[i].occupied = false;
		gridTiles[i].tileEntity = nullptr;
	}
}

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

	//Create Grid
	instance->grid = new Grid();

	//Create Editor
	instance->editor = new Editor();

	//Set sceneToBeLoaded to a nullptr value
	instance->sceneToBeLoaded = nullptr;

	//Initialize Input
	Input::Init(instance->renderer->GetWindow());

	instance->isActive = true;

	//Set default values
	instance->grid->scale = Vec2(DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE);
	instance->grid->tileScale = Vec2(DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE);
	instance->grid->Construct();

	//Initialize Steamworks API
	if (SteamAPI_Init()) {
		Debug::Log("Steam Initialized");
	}

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

	if (instance->sceneToBeLoaded != nullptr) {
		delete SceneManager::GetActiveScene();
		SceneManager::SetActiveScene(instance->sceneToBeLoaded);
		instance->sceneToBeLoaded = nullptr;
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

void Core::SwitchScene(Scene* scene) {
	instance->sceneToBeLoaded = scene;
}

Grid* Core::GetGrid() {
	return instance->grid;
}

std::vector<Entity*> Core::GetGlobalEntityList()
{
	return instance->globalEntityList;
}

void Core::Terminate() {
	instance->isActive = false;
}

float Core::CalculateDeltaTime() {
	static float lastTime = (float)glfwGetTime();
	float currentTime = (float)glfwGetTime();

	float _deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	return _deltaTime;
}