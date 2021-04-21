// Header file for rigidbody class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#define RIGIDBODY_GRAVITY_IMPLIER_DEFAULT 15.0f
#define RIGIDBODY_DRAG_IMPLIER_DEFAULT 8.9f
#define RIGIDBODY_VELOCITY_MIN_CUT_TO_ZERO 5.0f

#include "../component.h"

#include "../math/vec2.h"
#include "../math/physics.h"
#include "../unique_types.h"

//Rigidbody has simple physics for "simulation"
class RigidBody : public Component {
private:
	Vec2 velocity; /***< The velocity of the rigidbody, is added to position every frame */
	Vec2 positionLastFrame; /***< Position last frame */

	void HandleMovement();
public:
	bool bSimulateGravity; /***< If true gravity will be simulated, set true in constructor*/
	bool bSimulateDrag; /***< If true drag will be simulated, set true in constructor*/

	float gravityAmount; /***< Amount of gravity */
	float dragAmount; /***< Amount of drag*/

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
	const Vec2 GetVelocity();

	virtual RigidBody* New() override { return new RigidBody(); }
	virtual RigidBody* Copy() const { return new RigidBody(*this); }

	virtual void OnComponentPropertiesEditor() override;
};

#endif // !RIGIDBODY_H