// Header file for Head up display class, this class will handle every HUD event (Pause Menu, buttons, building tab ect...)
//
// Version: 8/8/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, August 2019
#ifndef HUD_H
#define HUD_H

//Include ui.h
#include "../../engine/graphics/ui.h"

class Hud : public UIElement {
public:
	UIElement* wood_bg;
	UIElement* stones_bg;
	UIElement* materials_bg;

	/**
	* Constructor
	*/
	Hud();
};

#endif // !HUD_H