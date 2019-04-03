// Header file for entity class.
//
// Version: 3/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, March 2019
#ifndef ENTITY_H
#define ENTITY_H

//Include std::vector and Vec2
#include <vector>
#include "math/vec2.h"

//Include sprite
#include "sprite.h"

//Forward declare renderer
class Renderer;

class Entity {
private:
	Entity* parent; /***< The parent instance */
	std::vector<Entity*> children; /***< List of children*/

	//Transformations
	Vec2 position; /***< The global position (Parent transformations included)*/
	float rotation; /***< The global rotation (Parent transformations included)*/
	Vec2 scale; /***< The global scale (Parent transformations included)*/
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
	* Calls the renderer to render to the screen
	* @param Renderer*
	*/
	void Render(Renderer* renderer);

	/**
	* Update gets called every frame
	* @return void
	*/
	virtual void Update() {};
public:
	//Local members
	Sprite * sprite; /***< The sprite used by this entity*/

	Vec2 localPosition; /***< The local position of the Entity*/
	float localRotation; /***< The local rotation of the Entity*/
	Vec2 localScale; /***< The local scale of the Entity*/

	/**
	* Constructor
	*/
	Entity();

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
	* Returns a child where index matches
	* @param int
	* @return Entity*
	*/
	Entity* GetChild(int index);

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
	* Returns the position of the entity
	*/
	Vec2 GetPosition();
};

#endif // !ENTITY_H