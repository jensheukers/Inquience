// Header file for core class.
// Core class handles all main functionality of the engine.
// Core class is a singleton instance, meaning it can be accessed from anywhere in the program
//
// Version: 26/3/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, March 2019
#ifndef CORE_H
#define CORE_H
#include <string>
#include "renderer.h"

class Core {
private:
	static Core* instance; /***< The singleton instance*/
	
	//References
	Renderer* renderer; /***< The renderer instance*/

	//Local variables
	std::string executableDirectoryPath; /***< The path to the executable directory*/
	bool isActive; /***< Determines if the engine is active*/
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

	//Local methods

	/**
	* Calculates the time between this and last call to this method
	* @return float
	*/
	float CalculateDeltaTime();
};

#endif // !CORE_H