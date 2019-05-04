// Header file for scene class.
//
// Version: 23/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, March 2019

//Include entity since scene is a entity
#include "entity.h"
#include "camera.h"

class Scene : public Entity {
private:
	Camera * activeCamera; /***< The currently active Camera*/
public:
	/**
	* Constructor
	*/
	Scene();

	/**
	* Updates all the scene children
	*/
	void Update() override;

	/**
	* Sets the active camera
	* @param Camera*
	*/
	void SetActiveCamera(Camera* camera);

	/**
	* Returns the active camera
	* @return Camera*
	*/
	Camera* GetActiveCamera();

	/**
	* Load scene data from a external source
	* @param path
	* @return void
	*/
	void Load(char* filePath);

	/**
	* Save scene data to a external source
	* @param path
	* @return void
	*/
	void Save(char* filePath);
};
