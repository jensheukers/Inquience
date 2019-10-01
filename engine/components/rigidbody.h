// Header file for rigidbody class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../component.h"

#include "../math/vec2.h"
#include "../math/physics.h"

//Rigidbody has simple physics for "simulation"
class RigidBody : public Component {
private:
	Vec2 velocity; /***< The velocity of the rigidbody, is added to position every frame */
	Vec2 positionLastFrame; /***< Position last frame*/
public:
	Vec2 gravity; /***< Vector that holds the value for gravity, will be added everyframe as long as isGravityActive is true*/
	bool isGravityActive; /***< Determines if gravity is active or not*/
	bool isBlockedThisFrame; /***< If true, the rigidbody is not able to move anymore, note that this variable should not be modified outside of collider functions.
								   This makes sure that a collider cannot pass through whenever another collider has received a hit from our collider*/

	/**
	* Constructor
	*/
	RigidBody();

	/**
	* Update
	*/
	void Update() override;

	/**
	* Adds a force to velocity Vec2, NOTE: DeltaTime is automaticly included in rigidbody updates
	* @param Vec2
	*/
	void AddForce(Vec2 force);

	/**
	* Returns the velocity
	* @return Vec2
	*/
	Vec2 GetVelocity();
};

#endif // !RIGIDBODY_H