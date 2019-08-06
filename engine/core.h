// Header file for core class.
// Core class handles all main functionality of the engine.
// Core class is a singleton instance, meaning it can be accessed from anywhere in the program
//
// Version: 6/8/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, August 2019
#ifndef CORE_H
#define CORE_H
#include <string>

//Include renderer.h
#include "renderer.h"

//Include editor.h
#include "editor.h"

//Forward declare
class Scene;

//Define tile scale
#define DEFAULT_TILEMAP_SIZE 256
#define DEFAULT_TILE_SIZE 32
#define DEFAULT_GRID_SIZE 100 //Units (x:TILE_SIZE y:TILE_SIZE)

//Grid structure, there is always a grid initialized by default, using the grid we can check if tiles are occupied or not
struct GridTile {
	bool occupied; /***< True if a tile is occupied*/
	Vec2 position; /***< Position of the gridTile*/
	Entity* tileEntity; /***< The entity that is occupying the tile*/
};

struct Grid {
	Vec2 scale; /***< The scale of the grid*/
	Vec2 tileScale; /*** The scale of one tile*/
	std::vector<GridTile> gridTiles; /***< Vector containing all gridTiles*/

	/**
	* Constructs the grid, fills gridTiles vector
	*/
	void Construct();

	/**
	* Returns the Tile nearest to position
	* @param Vec2
	* @return GridTile
	*/
	GridTile* GetTilePosition(Vec2 position);

	/**
	* Clears the entire grid, sets all tiles to unoccupied, Note that this does not remove any objects
	*/
	void Clear();
};

class Core {
private:
	static Core* instance; /***< The singleton instance*/
	
	//References
	Renderer* renderer; /***< The renderer instance pointer*/
	Editor* editor; /***< The editor instance pointer*/
	Grid* grid; /***< The grid instance (Grid is used to find occupied tiles)*/

	//Local variables
	std::string executableDirectoryPath; /***< The path to the executable directory*/
	bool isActive; /***< Determines if the engine is active*/
	bool editorActive; /***< Determines if the editor is to be shown or not*/

	//Time
	float deltaTime; /***< The amount of seconds between frames*/
	float fps; /***< The amount of frames per second*/
	float timeElapsed; /***< The time in seconds since the program started*/

	//Core scene management
	Scene* sceneToBeLoaded; /***< Scene to be loaded at beginning of next frame, nullptr if there is no scene to be loaded*/

	//Global entity list
	std::vector<Entity*> globalEntityList; /***< The global entity list, every entity that gets created will automaticly be added, even if entity is not added to scene*/
public:
	/**
	* Initalizes the Core of the engine
	* @return void
	*/
	static void Initialize(int argc, char* argv[]);

	/**
	* Updates the engine, if engine is initialized
	* @return void
	*/
	static void Update();

	/**
	* Returns the value of isActive
	* @return bool
	*/
	static bool IsActive();

	/**
	* Frees all memory used by the engine
	* @return void
	*/
	static void Destroy();

	/**
	* Returns the path of the executable directory
	* @return std::string
	*/
	static std::string GetExecutableDirectoryPath();

	/**
	* Returns the deltaTime
	* @return float
	*/
	static float GetDeltaTime() { return instance->deltaTime; };

	/**
	* Returns the time elapsed since start of the program
	* @return float
	*/
	static float GetTimeElapsed() { return instance->timeElapsed; }

	/**
	* Returns the frames per second
	* @return float
	*/
	static float GetFramesPerSecond() { return instance->fps; };

	/**
	* Returns the renderer instance
	* @return Renderer*
	*/
	static Renderer* GetRendererInstance();

	/**
	* Enables or disables the editor
	* @param bool
	*/
	static void EnableEditor(bool state);

	/**
	* Safely finsishes frame then removes the active scene from memory and sets the new scene active
	*/
	static void SwitchScene(Scene* scene);

	/**
	* Returns the grid instance
	* @return Grid*
	*/
	static Grid* GetGrid();

	/**
	* Returns the global entity list
	* @return std::vector<Entity*>
	*/
	static std::vector<Entity*> GetGlobalEntityList();


	/**
	* Disables the core, settings isActive to false
	*/
	static void Terminate();

	//Local methods

	/**
	* Calculates the time between this and last call to this method
	* @return float
	*/
	float CalculateDeltaTime();
};

#endif // !CORE_H