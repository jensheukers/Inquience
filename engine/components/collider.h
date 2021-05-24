// Header file for collider class.
//
// Inquience uses AABB Collision detection
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2021
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

	bool collisionEntered; /***< Boolean that is true whenever another collider entered our collider this frame*/
	bool collisionActive;/***< Boolean that is true whenever another collider is in our collider this frame, and was last frame*/
	bool collisionExited; /***< Boolean that is true whenever another collider exited our collider this frame*/
protected:
	/**
	* Draws debug lines
	*/
	virtual void DrawDebugLines();

	//Mem holders
	StringVector onCollisionEnterReturnVector;
	StringVector onCollisionActiveReturnVector;
	StringVector onCollisionExitReturnVector;
public:
	Vec2 outer; /***< The external bounds of the BoxCollider*/
	bool scaleToOwner; /***< If true the outer scale will be set equal to the entity scale every frame*/

	Delegate onCollisionEnter; /***< Should be triggered whenever another collider entered our collider this frame*/
	Delegate onCollisionActive; /***< Should be triggered whenever another collider entered our collider last frame, and is still colliding*/
	Delegate onCollisionExit; /***< Should be triggered whenever another collider was colliding last frame but not this frame*/

	bool bDrawDebugLines; /***< If true DrawDebugLines will be called*/
	glm::vec3 debugDrawColor; /***< The color to draw in, default is (0, 1, 0)*/

	/**
	* Constructor
	*/
	Collider();

	/**
	* Update
	*/
	void Update() override;

	/**
	* Is called every frame by collisionmanager
	*/
	void CheckCollision(const std::vector<Collider*> colliders);

	/**
	* The actual collision check, can be overwritten
	*/
	bool IsColliding(Collider* other);

	/**
	* Returns true if point is inside our collider
	*/
	bool IsCollidingWithPoint(Vec2 point);

	/**
	* Returns the size of the collider, should be overwritten
	*/
	virtual float GetSize();

	/**
	* Returns all the colliders currently colliding with this collider
	*/
	std::vector<Collider*> const GetHits();

	/**
	* Returns the value of collisionEntered as a constant
	*/
	bool CollisionEntered() const { return this->collisionEntered; }

	/**
	* Returns the value of collisionActive as a constant
	*/
	bool CollisionActive() const { return this->collisionActive; }

	/**
	* Returns the value of collisionExit as a constant
	*/
	bool CollisionExited() const { return this->collisionExited; }

	virtual Collider* New() override { return new Collider(); }
	virtual Collider* Copy() const { return new Collider(*this); }

	void OnComponentPropertiesEditor() override;
};

#endif // !COLLIDER_H