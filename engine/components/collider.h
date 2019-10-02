// Header file for collider class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#ifndef COLLIDER_H
#define COLLIDER_H

//Include base
#include "../component.h"

#include "../unique_types.h"
#include "../math/physics.h"
#include "glm/glm.hpp"

#include <vector>

//Collider class provides simple collision calculations.
class Collider : public Component {
private:
	std::vector<Collider*> hits;
public:
	bool isTrigger; /*** Determine's if collider is a trigger or not, this is just a fictional property and has no use in the component itself,
						 it is just here if it is to be used, RigidBody does use isTrigger*/
	Vec2 outer; /***< The external bounds of the collider*/
 
	Delegate onCollisionEnter; /***< Should be triggered whenever another collider entered our collider this frame*/
	Delegate onCollision; /***< Should be triggered whenever another collider entered our collider last frame, and is still colliding*/
	Delegate onCollisionExit; /***< Should be triggered whenever another collider was colliding last frame but not this frame*/

	bool collisionEntered; /***< Boolean that is true whenever another collider entered our collider this frame*/
	bool collisionActive;/***< Boolean that is true whenever another collider is in our collider this frame, and was last frame*/
	bool collisionExited; /***< Boolean that is true whenever another collider exited our collider this frame*/

	Collider* hitLast; /***< The last collider to enter this collider*/

	bool bDrawDebugLines; /***< If true DrawDebugLines will be called*/
	glm::vec3 debugDrawColor; /***< The color to draw in, default is (0, 1, 0)*/

	/**
	* BeginPlay
	*/
	virtual void BeginPlay() override;

	/**
	* Update
	*/
	virtual void Update() override;

	/**
	* Is called every frame by collisionmanager
	*/
	void CheckCollision();

	/**
	* The actual collision check, can be overwritten
	*/
	virtual bool IsColliding(Collider* other) { return false; };

	/**
	* Returns all the colliders currently colliding with this collider
	*/
	std::vector<Collider*> GetHits();

	virtual Component* CreateNewInstance() override { return new Collider(); }

	/**
	* Draws debug lines
	*/
	virtual void DrawDebugLines() {};

	/**
	* Destructor
	*/
	~Collider();
};

class BoxCollider : public Collider {
private:
	/**
	* Override DrawDebugLines
	*/
	void DrawDebugLines() override;

public:
	virtual Component* CreateNewInstance() override { return new BoxCollider(); }

	/**
	* Override IsColliding to do collision check
	*/
	virtual bool IsColliding(Collider* other) override;
};

#endif // !COLLIDER_H