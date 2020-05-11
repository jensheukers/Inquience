#include "gamestate.h"

#include <luascript.h>
#include "creatures/player.h"

void GameState::BeginGameplay() {
	Debug::Log("Starting Gameplay");

	Player* player = new Player();
	SceneManager::GetActiveScene()->AddChild(player);
}

GameState::GameState(std::vector<std::string> levels) {
	this->currentLevel = -1;
	this->levels = levels;
	this->currentScene = nullptr;
}

void GameState::Initialize() {
	this->NextLevel();
}

void GameState::NextLevel() {
	this->currentLevel++;

	//State expects .scene file
	LuaScript::RunFunction(levels[this->currentLevel], IQ_SCENELOAD_FUNCTIONNAME);

	if (this->currentScene == SceneManager::GetActiveScene() || !(this->currentScene = SceneManager::GetActiveScene())) {
		Debug::Log("Failed to load level, GetActiveScene() returned nullptr OR scene did not change");
		return;
	}

	Debug::Log("Next Level (" + levels[this->currentLevel] + ") Loaded!");

	//Check if gameplay should be started
	if (SceneManager::GetActiveScene()->GetKeyValue(IQ_SCENELOAD_GAMEPLAY_KEYNAME) == IQ_SCENELOAD_GAMEPLAY_POSITIVE_KEYVALUE) {
		this->BeginGameplay();
	}
}

void GameState::LoadLevel(int index) {
	this->currentLevel = index - 1; // Set index of currentlevel to given index - 1 so we can load next level

	Debug::Log("Level index  set to (" + std::to_string(this->currentLevel) + ") loading next level (" + std::to_string(index) + ")");
	this->NextLevel();
}

void GameState::LoadLevel(std::string path) {
	for (size_t i = 0; i < levels.size(); i++) {
		if (levels[i] == path) {
			this->LoadLevel(i);
		}
	}
}