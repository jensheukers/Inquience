#include "gamestate.h"

#include <luascript.h>

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