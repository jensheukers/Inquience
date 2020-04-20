//Base Game State class
//Logic for game state

#ifndef IQ_GAMESTATE_H
#define IQ_GAMESTATE_H

//The function inside lua script to load scenes
#define IQ_SCENELOAD_FUNCTIONNAME "Initialize"

#include <scenemanager.h>
#include <vector>
#include <string>

class GameState {
private:
	//Current level index
	int currentLevel;
	std::vector<std::string> levels; // All the levels in numeric order ( 0 = main menu)

	//Holders
	Scene* currentScene;
public:
	GameState(std::vector<std::string> levels);

	//Initializes the game state and loads given scene path
	void Initialize();

	//Loads the next level
	void NextLevel();
};

#endif // !IQ_GAMESTATE_H