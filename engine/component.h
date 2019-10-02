// Header file for component class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#ifndef COMPONENT_H
#define COMPONENT_H

#include <map>
#include <vector>
#include <string>
#include <functional>

class Component {
private:
	class Entity* owner; /***< The owner of this component*/
	std::map <std::string, std::function<void(std::vector<std::string>)>> properties; /***< Properties map containing function pointers to properties*/
protected:
	/**
	* Adds a property function to the properties list, a component property is nothing more than a function that gets executed whenver SetProperty with right key is called
	* We can use this to set properties externally without having to recompile, as long as the property is registered in the properties map
	*/
	void AddProperty(std::string key, std::function<void(std::vector<std::string>)> value);
public:
	/**
	* Calls key property function with std::vector<std::string> arguments
	*/
	void SetProperty(std::string key, std::vector<std::string> args);

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