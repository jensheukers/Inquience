// Header file for component class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#ifndef COMPONENT_H
#define COMPONENT_H

#include <vector>
#include <string>

class Component {
private:
	class Entity* owner; /***< The owner of this component*/
public:
	/**
	* Sets the owner of this component
	* @param Entity*
	* @return Entity*
	*/
	Entity* SetOwner(Entity* entity);

	/**
	* Returns the owner of this component
	* @return Entity*
	*/
	Entity* GetOwner();

	//Virtual methods

	/**
	* Update function gets called by owner every frame
	* @return void
	*/
	virtual void Update() {};

	/**
	* Creates and returns a new instance, This method should be overwritten on derived components to always retrieve right type when using the component register
	*/
	virtual Component* CreateNewInstance() { return new Component(); }

	/**
	* Destructor
	*/
	virtual ~Component() {};
};

#endif // !COMPONENT_H