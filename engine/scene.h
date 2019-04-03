// Header file for scene class.
//
// Version: 3/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, March 2019

//Include entity since scene is a entity
#include "entity.h"

class Scene : public Entity {
public:
	/**
	* Updates all the scene children
	*/
	void Update() override;

	/**
	* Calls the private render function
	*/
	void RenderScene(Renderer* renderer);
};