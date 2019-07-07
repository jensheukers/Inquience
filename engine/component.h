// Header file for component class.
//
// Version: 7/7/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, July 2019
#ifndef COMPONENT_H
#define COMPONENT_H

//Forward declare entity
class Entity;

class Component {
private:
	Entity* owner; /***< The owner of this component*/
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

	/**
	* Update function gets called by owner every frame
	* @return void
	*/
	virtual void Update() {};

	/**
	* Destructor
	*/
	virtual ~Component() {};
};

#endif // !COMPONENT_H