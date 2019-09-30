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

//Rigidbody should know about collider, as a collider is required for a rigidbody to function properly
#include "collider.h"

//Rigidbody has simple physics for "simulation"
class RigidBody : public Component {
private:
	Vec2 velocity; /***< The velocity of the rigidbody, is added to position every frame */
	Vec2 positionLastFrame; /***< Position last frame*/
public:
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