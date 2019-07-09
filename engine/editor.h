// Header file for Editor class.
//
// Version: 3/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#ifndef EDITOR_H
#define EDITOR_H

//Include imgui.h & entity.h
#include "../external/imgui/imgui.h"

class Editor {
private:
	//Bools
	bool loadMenuActive;
	bool saveMenuActive;

	bool levelEditorActive;
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