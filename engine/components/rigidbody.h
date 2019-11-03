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
#include "../unique_types.h"

//Rigidbody has simple physics for "simulation"
class RigidBody : public Component {
private:
	Vec2 velocity; /***< The velocity of the rigidbody, is added to position every frame */
	Vec2 positionLastFrame; /***< Position last frame */
public:
	Delegate onBlockedDelegate; /***< Delegate for whenever isBlockedThisFrame is true*/

	/**
	* Constructor
	*/
	RigidBody();

	/**
	* Update
	*/
	void Update() override;

	/**
	* Adds a force to velocity Vec2
	* @param Vec2
	*/
	void AddForce(Vec2 force);

	/**
	* Sets the velocity
	*/
	void SetVelocity(Vec2 velocity);

	/**
	* Returns the velocity
	* @return Vec2
	*/
	Vec2 GetVelocity();

	virtual RigidBody* New() override { return new RigidBody(); }
	virtual RigidBody* Copy() const { return new RigidBody(*this); }
};

#endif // !RIGIDBODY_H