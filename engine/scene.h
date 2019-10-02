// Header file for scene class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019

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
	* Destructor
	*/
	~Scene();
};
