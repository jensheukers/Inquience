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
#include "../component.h"

//Include sprite
#include "../components/sprite.h"

//Include unique_types
#include "../unique_types.h"

//User Interface Element is a entity
class UIElement : public Component {
private:
	bool entered; /***< True if mouse is inside the uiElement*/
public:
	Delegate OnEnterDelegate;
	Delegate OnLeaveDelegate;
	Delegate OnStayDelegate;

	/**
	* Triggered whenever the mouse enters the element
	*/
	void OnEnter() { OnEnterDelegate.Execute(); };

	/**
	* Triggered whenever the mouse leaves the element
	*/
	void OnLeave() { OnLeaveDelegate.Execute(); };

	/**
	* Triggered whenever the mouse is on the element
	*/
	void OnStay() { OnStayDelegate.Execute(); };

	/**
	* Overwritten update method
	*/
	void Update() override;

	/**
	* Destructor
	*/
	virtual ~UIElement() {};
};

#endif // !UI_H
