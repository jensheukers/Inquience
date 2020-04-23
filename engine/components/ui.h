// Header file for User Interface Elements.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, April 2020
#ifndef UI_H
#define UI_H

//Include entity.h
#include "../component.h"

//Include sprite
#include "../components/sprite.h"

//Include unique_types
#include "../unique_types.h"

//User Interface Element is a entity
class UIComponent : public Component {
private:
	bool entered; /***< True if mouse is inside the uiElement*/

	//Hold for returning to lua
	StringVector onEnterReturnVector;
	StringVector onStayReturnVector;
	StringVector onLeaveReturnVector;
	StringVector onRightClickReturnVector;
	StringVector onLeftClickReturnVector;
	StringVector onMiddleClickReturnVector;
public:
	Delegate OnEnterDelegate;
	Delegate OnLeaveDelegate;
	Delegate OnStayDelegate;
	Delegate OnRightClickDelegate;
	Delegate OnLeftClickDelegate;
	Delegate OnMiddleClickDelegate;

	/*
	* Constructor
	**/
	UIComponent();

	/**
	* Triggered whenever the mouse enters the element
	*/
	virtual void OnEnter() { OnEnterDelegate.Execute(); };

	/**
	* Triggered whenever the mouse leaves the element
	*/
	virtual void OnLeave() { OnLeaveDelegate.Execute(); };

	/**
	* Triggered whenever the mouse is on the element
	*/
	virtual void OnStay() { OnStayDelegate.Execute(); };

	/**
	* Triggered whenever the mouse is clicked on the element
	*/
	virtual void OnClick(int mouseIndex);

	/**
	* Overwritten update method
	*/
	void Update() override;

	/**
	* Overwrite
	*/
	virtual UIComponent* New() override { return new UIComponent(); }
	virtual UIComponent* Copy() const { return new UIComponent(*this); }

	/**
	* Destructor
	*/
	virtual ~UIComponent() {};
};

#endif // !UI_H
