// Header file for entity class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#ifndef ENTITY_H
#define ENTITY_H

//Include typeinfo
#include <typeinfo>

//Include debug.h
#include "debug.h"

//Include std::vector and Vec2
#include <vector>
#include "math/vec2.h"

//Include component
#include "component.h"

//Forward declare renderer
class Renderer;

class Entity {
private:
	//Local Members
	Entity* parent; /***< The parent instance */
	std::vector<Entity*> children; /***< List of children*/

	//Components
	std::vector<Component*> components; /***< List of components*/

	//Transformations
	Vec2 position; /***< The global position (Parent transformations included)*/
	Vec2 scale; /***< The global scale (Parent transformations included)*/

	bool active; /***< Determines wheter the entity is active or not, if entity is not active no updates will be performed*/

	/**
	* Generates a UniqueID and sets uniqueId
	*/
	void GenerateUniqueID();
protected:
	/**
	* Handles transformation with parent entity
	*/
	void HandleParentTransformations();

	/**
	* Updates all children's transformations, should be called by parent
	*/
	void UpdateChildren();

	/**
	* Updates all components
	*/
	void UpdateComponents();

	/**
	* Update gets called every frame
	* @return void
	*/
	virtual void Update() {};
public:
	Vec2 localPosition; /***< The local position of the Entity*/
	Vec2 localScale; /***< The local scale of the Entity*/
	std::string tag; /***< Tag of the entity, whenever GetChildByTag() gets called and tag matches it will be returned*/
	int uniqueId; /***< Unique id of the entity*/

	/**
	* Constructor
	*/
	Entity();

	/**
	* Copy constructor
	*/
	Entity(const Entity& entity);

	/**
	* Sets a entity active or not, also updates children automaticly
	* @param bool
	*/
	void SetActive(bool state);

	/**
	* Returns true if the entity is active
	*/
	bool Active();

	/**
	* Adds a child to the children vector
	* @param Entity*
	* @return Entity*
	*/
	Entity* AddChild(Entity* entity);

	/**
	* Removes a child from the children vector where index matches
	* @param int
	* @return Entity*
	*/
	Entity* RemoveChild(int index);

	/**
	* Removes a child from the children vector where pointer matches
	* @param Entity*
	* @return Entity*
	*/
	Entity* RemoveChild(Entity* entity);

	/**
	* Returns a child where index matches
	* @param int
	* @return Entity*
	*/
	Entity* GetChild(int index);

	/**
	* Returns a child where pointer matches
	* @param Entity*
	* @return Entity*
	*/
	Entity* GetChild(Entity* entity);

	/**
	* Returns true if entity has child
	* @param Entity*
	* @return bool
	*/
	bool HasChild(Entity* entity);

	/**
	* Returns the parent of this entity
	* @return Entity*
	*/
	Entity* GetParent();

	/**
	* Returns the children vector
	* @return std::vector<Entity*>
	*/
	std::vector<Entity*> GetChildren();

	/**
	* Sets the parent of this entity
	* @param Entity*
	* @return void
	*/
	void SetParent(Entity* entity);

	/**
	* Adds a component to the components vector
	* @return T* (template)
	*/
	template<class T>
	T* AddComponent() {	
		//First check if T is a derived component
		if (!std::is_base_of<Component, T>::value) {
			Debug::Log(std::string(typeid(T).name()) + " Is not a derived Component!");
			return nullptr; // Return nullptr
		}

		//Create a new instance of the component, and set the owner
		T* instance = new T();
		Component* component = dynamic_cast<Component*>(instance);
		components.push_back(component);
		component->SetOwner(this);
		component->BeginPlay();
		return instance;
	}

	/**
	* Adds a already existing component instance to the components vector
	*/
	Component* AddExistingComponentInstance(Component* component) {
		components.push_back(component);
		component->SetOwner(this);
		component->BeginPlay();
		return component;
	}

	/**
	* Gets a component from the components vector, if index is not provided, will return the first found instance
	* if none is found, will return nullptr.
	* @return T* (template)
	*/
	template<class T>
	T* GetComponent(int index = 0) {
		int steps = index;
		for (size_t i = 0; i < components.size(); i++) {
			if (dynamic_cast<T*>(components[i])) {
				if (steps != 0) { steps--; continue; }
				else return (T*)components[i]; // Assume components[i] == The given input template class
			}
		}

		//Return nullptr if not found
		return nullptr;
	}

	/**
	* Returns the component vector
	*/
	std::vector<Component*> GetComponents() { return this->components; }

	/**
	* Returns true if component is in the components list, else returns false
	* @return bool
	*/
	template<class T>
	bool HasComponent() {
		for (size_t i = 0; i < components.size(); i++) {
			if (dynamic_cast<T*>(components[i]))
				return true;
		}
		return false;
	}

	/**
	* Check if component is present with instance passed
	*/
	template <class T>
	bool HasComponent(T* instance) {
		for (size_t i = 0; i < components.size(); i++) {
			if (typeid(*instance) == typeid(*components[i]))
				return true;
		}
		return false;
	}

	/*
	* Removes component of type from components vector if exists
	* @return bool success
	*/
	template<class T>
	bool RemoveComponent(T* instance) {
		for (size_t i = 0; i < components.size(); i++) {
			if (instance == components[i]) {
				components.erase(components.begin() + i);
				delete instance;
				return true;
			}
		}
		return false;
	}

	/**
	* Returns the global position of the entity
	*/
	Vec2 GetPosition();

	/**
	* Retruns the global scale of the entity
	*/
	Vec2 GetScale();

	/**
	* Returns this if tag == this->tag, else calls this method on children.
	*/
	Entity* GetChildByTag(std::string tag);

	/**
	* Moves child up by 1 in children array
	*/
	void MoveChildUp(Entity* child);

	/**
	* Moves child down by 1 in children array
	*/
	void MoveChildDown(Entity* child);

	/**
	* Fills vector of Entity pointers that are x distance from given position
	* Note: function does not respect children arrays, all found entities will be added to the vector as a raw pointer
	* @param std::vector<Entity*>& vector reference
	* @param Vec2 position (In global space)
	* @param float distance
	*/
	void FindEntitiesNearPosition(std::vector<Entity*>& vectorRef, Vec2 position, float distance);

	/*
	* Returns the highest parent in the hierarchy (Mostly always scene), this is a more OOP way of getting the scene without requiring
	* Scenemanager
	*/
	Entity* GetHighestEntityInHierarchy();

	/**
	* Iterates through all children and if they have component of template type, it will be added to given parameter vector
	* @param std::vector<T*>& vector reference
	*/
	template<typename T>
	void GetAllComponentsOfTypeInChildren(std::vector<T*>& vectorRef) {
		if (T * component = GetComponent<T>()) {
			vectorRef.push_back(component);
		}

		for (size_t ii = 0; ii < children.size(); ii++) {
			children[ii]->GetAllComponentsOfTypeInChildren(vectorRef);
		}
	}

	/**
	* Destructor
	*/
	virtual ~Entity();
};

#endif // !ENTITY_H