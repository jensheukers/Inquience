//Base Game State class
//Logic for game state

#ifndef IQ_GAMESTATE_H
#define IQ_GAMESTATE_H

//The function inside lua script to load scenes
#define IQ_SCENELOAD_FUNCTIONNAME "Initialize"

//Definitions to load gameplay
#define IQ_SCENELOAD_GAMEPLAY_KEYNAME "BeginGameplay"
#define IQ_SCENELOAD_GAMEPLAY_POSITIVE_KEYVALUE "True"
#define IQ_SCENELOAD_GAMEPLAY_NEGATIVE_KEYVALUE "False"

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

protected:
	//Triggered when specific KVP is set in scene file
	virtual void BeginGameplay();
public:
	GameState(std::vector<std::string> levels);

	//Initializes the game state and loads given scene path
	void Initialize();

	//Loads the next level
	void NextLevel();

	//Loads the level with given index
	void LoadLevel(int index);

	//Loads the level with given path
	void LoadLevel(std::string path);
};

#endif // !IQ_GAMESTATE_H