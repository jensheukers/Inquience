// Header file for core class.
// Core class handles all main functionality of the engine.
// Core class is a singleton instance, meaning it can be accessed from anywhere in the program
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, November 2019
#ifndef CORE_H
#define CORE_H
#include <string>

//Include renderer.h
#include "renderer.h"

//Forward declare
class Scene;

class Core {
private:
	static Core* instance; /***< The singleton instance*/
	
	//References
	Renderer* renderer = nullptr; /***< The renderer instance pointer*/

	//Local variables
	std::string executableDirectoryPath; /***< The path to the executable directory*/

	//Time
	float deltaTime; /***< The amount of seconds between frames*/
	float fps; /***< The amount of frames per second*/
	float frame; /***< The Current frame*/
	float timeElapsed; /***< The time in seconds since the program started*/

	bool requestExit; /***< If true the applications requests to be exited*/

	std::vector<std::function<void()>> LateFrameFunctionList; /***< List of functions to be executed after the frame is done*/
public:
	/**
	* Initalizes the Core of the engine
	* @return void
	*/
	static void Initialize(int argc, char* argv[], Vec2 resolution = Vec2(1280, 720), std::string title = "INQUIENCE");

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
	static float GetTimeElapsed() { return (instance) ? instance->timeElapsed : 0; }

	/**
	* Returns the frames per second
	* @return float
	*/
	static float GetFramesPerSecond() { return instance->fps; };

	/**
	* Returns the current frame
	* @return float
	*/
	static float GetCurrentFrame() { return instance->frame; };

	/**
	* Returns the renderer instance
	* @return Renderer*
	*/
	static Renderer* GetRendererInstance();

	/**
	* Requests a exit
	*/
	static void RequestExit();

	/**
	* Adds a function that is to be executed once at the end of this frame
	*/
	static void ExecuteLateFrame(std::function<void()> func);

	//Local methods

	/**
	* Calculates the time between this and last call to this method
	* @return float
	*/
	float CalculateDeltaTime();
};

#endif // !CORE_H