// Header file for User Interface Elements.
//
// Version: 2/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#ifndef UI_H
#define UI_H

//Include entity.h
#include "../entity.h"

//Include sprite
#include "../components/sprite.h"

//User Interface Element is a entity
class UIElement : public Entity {
private:
	bool entered; /***< True if mouse is inside the uiElement*/
public:
	/**
	* Triggered whenever the mouse enters the element
	*/
	virtual void OnEnter() {};

	/**
	* Triggered whenever the mouse leaves the element
	*/
	virtual void OnLeave() {};

	/**
	* Triggered whenever the mouse is on the element
	*/
	virtual void OnStay() {};

	/**
	* Overwritten update method
	*/
	void Update() override;
};

#endif // !UI_H
