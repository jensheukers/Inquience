#include "gamestate.h"

#include "creatures/player.h"

void GameState::BeginGameplay() {
	Debug::Log("Starting Gameplay");

	Player* player = new Player();
	Core::GetSceneManager()->GetActiveScene()->AddChild(player);

	//Try to retrieve x and y values from KeyValuePairs for spawn location
	float x = std::stof(Core::GetSceneManager()->GetActiveScene()->GetKeyValue("x"));
	float y = std::stof(Core::GetSceneManager()->GetActiveScene()->GetKeyValue("y"));

	player->position = Vec2(x, y); // Set spawn location

	Core::GetSceneManager()->GetActiveScene()->GetActiveCamera()->SetTarget(player);
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

	if (this->levels.size() <= this->currentLevel) {
		this->currentLevel = this->levels.size() - 1;
	}

	Debug::Log("Loading Scene: " + this->levels[this->currentLevel] );

	Core::GetSceneManager()->ReadFromFileAndSwap(this->levels[this->currentLevel]);

	if (this->currentScene == Core::GetSceneManager()->GetActiveScene() || !(this->currentScene = Core::GetSceneManager()->GetActiveScene())) {
		Debug::Log("Failed to load level, GetActiveScene() returned nullptr OR scene did not change");
		return;
	}

	Debug::Log("Next Level (" + levels[this->currentLevel] + ") Loaded!");

	//Check if gameplay should be started
	if (Core::GetSceneManager()->GetActiveScene()->GetKeyValue(IQ_SCENELOAD_GAMEPLAY_KEYNAME) == IQ_SCENELOAD_GAMEPLAY_POSITIVE_KEYVALUE) {
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