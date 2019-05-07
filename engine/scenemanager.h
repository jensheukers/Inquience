// Header file for SceneManager class.
// SceneManager is a singleton instance
//
// Version: 4/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#include "scene.h"

class SceneManager {
private:
	static SceneManager* instance; /***< The instance of the SceneManager*/

	//Local members
	Scene* activeScene; /***< The currently active scene */

	/**
	* Creates a instance if not yet created
	* @return SceneManager*
	*/
	static SceneManager* GetInstance();
public:
	/**
	* Sets the currently active scene
	* @param Scene*
	* @return Scene*
	*/
	static Scene* SetActiveScene(Scene* scene);

	/**
	* Gets the currently active scene
	* @return Scene*
	*/
	static Scene* GetActiveScene();

	/**
	* Safely Unloads active scene if there is one, then creates a new scene instance and loads scene from file
	* @param char*
	* @return Scene*
	*/
	static Scene* LoadScene(char* path);

	/**
	* Terminates the SceneManager
	* @return void
	*/
	static void Terminate();
};