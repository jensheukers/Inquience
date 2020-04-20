#include "gamestate.h"

#include <luascript.h>

GameState::GameState(std::vector<std::string> levels) {
	this->currentLevel = 0;
	this->levels = levels;
	this->currentScene = nullptr;
}

void GameState::Initialize() {
	this->NextLevel();
}

void GameState::NextLevel() {
	//State expects .scene file
	LuaScript::RunFunction(levels[this->currentLevel], IQ_SCENELOAD_FUNCTIONNAME);

	if (this->currentScene == SceneManager::GetActiveScene() || !(this->currentScene = SceneManager::GetActiveScene())) {
		Debug::Log("Failed to load level, active scene is nullptr OR scene did not change");
		return;
	}

	Debug::Log("Next Level (" + levels[this->currentLevel] + ") Loaded!");
}