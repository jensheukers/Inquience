// Main menu header file, derives from Scene
//
// Version: 2/7/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, July 2019
#ifndef MAIN_MENU_SCENE_H
#define MAIN_MENU_SCENE_H

//Include scenemanager.h & ui.h
#include "../../engine/scenemanager.h"
#include "../../engine/graphics/ui.h"

//Buttons 
class StartButton : public UIElement {
public:
	/**
	* Constructor
	*/
	StartButton();

	// Overrides
	void OnEnter() override;
	void OnLeave() override;
	void OnStay() override;
};

class OptionButton : public UIElement {
public:
	/**
	* Constructor
	*/
	OptionButton();

	// Overrides
	void OnEnter() override;
	void OnLeave() override;
	void OnStay() override;
};

class QuitButton : public UIElement {
public:
	/**
	* Constructor
	*/
	QuitButton();

	// Overrides
	void OnEnter() override;
	void OnLeave() override;
	void OnStay() override;
};

class MainMenu : public Scene {
public:
	/**
	* Constructor
	*/
	MainMenu();
};


#endif // !MAIN_MENU_SCENE_H
