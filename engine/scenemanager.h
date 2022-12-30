// Header file for SceneManager class.
// SceneManager is a singleton instance
//
// Version: 4/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, 2021
#include "scene.h"

class SceneManager {
private:
	//Local members
	Scene* activeScene; /***< The currently active scene */
public:
	/**
	* @brief Constructor
	*/
	SceneManager();

	/**
	* Sets the currently active scene
	* @param Scene*
	* @return Scene*
	*/
	Scene* SetActiveScene(Scene* scene);

	/**
	* Gets the currently active scene
	* @return Scene*
	*/
	Scene* GetActiveScene();

	/**
	* Safely swaps scenes, and removes current scene
	*/
	Scene* SwapScene(Scene* scene);

	/*
	* Reads a scene from file, and swaps the scene
	**/
	Scene* ReadFromFileAndSwap(std::string path);


	/**
	* Terminates the SceneManager
	* @return void
	*/
	void Terminate();
};