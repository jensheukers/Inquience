// Header file for client class, client handles / supervises all gameplay
// Client is a singleton instance, and should never be destroyed during gameplay
//
// Version: 6/8/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, August 2019
#ifndef CLIENT_H
#define CLIENT_H

//Defines
#define UNIT_SELECT_RANGE 32

//Enum structuretypes

enum StructureType {
	STRUCTURE_WOODCUTTER_HUT = 0,
	STRUCTURE_STONE_MINE_HUT,
	STRUCTURE_MATERIAL_MINE_HUT
};

//Include vector
#include <vector>

//Include vec2.h
#include "../engine/math/vec2.h"

class Scene; // Forward declare
class Unit; // Forward declare
class UIElement; // Forward declare
class Text; // Forward declare
class Structure; // Forward declare

// Struct containing game settings, instance of gameSettings should be parsed whenever a game is started
// Game settings can also be loaded from an external source.
struct GameSettings {
	//World settings
	std::string worldSceneFile; /***< File of the scene to be loaded as world*/

	//Resources
	int start_wood; /***< The amount of wood the client starts with*/
	int start_stones; /***< The amount of stones the client starts with*/
	int start_materials; /***< The amount of materials the client starts with*/

	/**
	* Constructor
	* @param std::string worldSceneFile
	* @param int start_wood
	* @param int start_stones
	* @param int start_materials
	*/
	GameSettings(std::string worldSceneFile, int start_wood, int start_stones, int start_materials);
};

//Singleton instance
class Client {
private:
	static Client* instance; /***< The singleton client instance */

	//Scenes
	Scene* mainMenu; /***< The main menu scene pointer*/
	Scene* scene; /*** Reference to the current scene*/

	//Bools
	bool inSession; /***< Should be true if the player is in a session, the player will now be able to select units and move them.*/

	//Vectors
	std::vector<Unit*> units; /***< All our units in the scene*/
	std::vector<Unit*> selectedUnits; /***< List containing all selected units*/

	std::vector<Structure*> structures; /***< All our structures in the scene*/

	//Resources
	int wood; /***< Amount of wood the client has */
	int stones; /***< Amount of stones the client has*/
	int materials; /***< Amount of materials the player has*/

	//Hud elements
	//Backgrounds
	UIElement* wood_bg;
	UIElement* stones_bg;
	UIElement* materials_bg;
public:
	/***
	* Constructor, if instance pointer is already set then we destroy the newly created instance 
	*/
	Client();

	/**
	* Start is called right after the instance is declared, it should only be called once
	*/
	static void Start();

	/**
	* Update gets called by main every frame
	*/
	static void Update();

	/**
	* Starts the game using a GameSettings instance as reference, loads the world and sets default settings then plays the game.
	* @param GameSettings
	*/
	static void StartGame(GameSettings settings);

	/**
	* Builds a structure
	* @param StructureType*
	* @param Vec2 position
	*/
	static void BuildStructure(StructureType type, Vec2 position);
};


#endif // !CLIENT_H