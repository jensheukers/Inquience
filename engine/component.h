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
#include <functional>

//INCLUDE IMGUI EDITOR ONLY
#include "../external/imgui/imgui.h"

typedef std::vector<std::string> StringVector;
typedef std::function<void(StringVector)> PropertySetCallback;
typedef std::function<StringVector()> PropertyGetCallback;

struct ComponentProperty {
	std::string key;
	PropertySetCallback setCallback;
	PropertyGetCallback getCallback;
};

class Component {
private:
	class Entity* owner; /***< The owner of this component*/
	std::vector<ComponentProperty*> properties; /***< Properties vector, contains all property callbacks*/

	/**
	* Returns the correct component property index for key
	*/
	size_t GetPropertyIndex(std::string key);
protected:
	/**
	* Adds a property function to the properties list, a component property is nothing more than a function that gets executed whenver SetProperty with right key is called
	* We can use this to set properties externally without having to recompile, as long as the property is registered in the properties map
	*/
	void AddProperty(std::string key, PropertySetCallback setCallback, PropertyGetCallback getCallback = []() -> StringVector { return StringVector(); });
public:
	/**
	* Returns the name of the component as a string
	*/
	std::string GetName() { return typeid(*this).name(); }
	/**
	* Calls key property function with std::vector<std::string> arguments
	*/
	void SetProperty(std::string key, StringVector args);

	/**
	* Returns component property values as a StringVector
 	*/
	StringVector GetProperty(std::string key);

	/**
	* Returns the component properties vector
	*/
	std::vector<ComponentProperty*> GetProperties() { return this->properties; }

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
	* Gets called after the component has been added through AddComponent
	*/
	virtual void BeginPlay() {};

	/**
	* Update function gets called by owner every frame
	* @return void
	*/
	virtual void Update() {};

	/**
	* Creates and returns a new instance, This method should be overwritten on derived components to always retrieve right type when using the component register
	*/
	virtual Component* New() { return new Component(); }

	/**
	* Copies the component settings and returns a new instance, should be overwritten
	*/
	virtual Component* Copy() const { return new Component(*this); }

// SHOULD ONLY BE WITH EDITOR (Fix/Create macro's)
	bool bShowComponentProperties = false;

	/**
	* Gets called when a component property window gets opened for this component
	*/
	virtual void OnComponentPropertiesEditor();
// -----------------------------------------
	/**
	* Destructor
	*/
	virtual ~Component() {};
};

#endif // !COMPONENT_H