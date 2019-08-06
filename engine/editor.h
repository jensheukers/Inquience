// Header file for Editor class.
//
// Version: 6/8/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, August 2019
#ifndef EDITOR_H
#define EDITOR_H

//Include vector
#include <vector>

//Include entity.h & vec2.h
#include "../external/imgui/imgui.h"
#include "../engine/math/vec2.h"

//Forward declare
class Entity;
class Camera;
struct Grid;

class Editor {
private:
	//Bools
	bool loadMenuActive;
	bool saveMenuActive;

	bool levelEditorActive;

	//References
	Entity* selectionEntity; /***< The entity used as a reference, will be copied when placed*/
	Grid* grid; /***< Reference to the grid instance*/

	Camera* editorCamera; /***< The editor camera instance, will be set active whenever editor is active*/
public:
	/**
	* Constructor
	*/
	Editor();

	/**
	* Update gets called every frame
	*/
	void Update();
};

#endif // !EDITOR_H