// Header file for Physics class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#ifndef PHYSICS_H
#define PHYSICS_H

//Include vec2.h
#include "vec2.h"

//Side of hit
enum PhysicsHitPlaneResponseEventType {
	Horizontal,
	Vertical,
	Edge
};

class Physics {
public:
	/**
	* Returns true if position is between a and b
	* @param Vec2
	* @param Vec2
	* @param Vec2
	* @return bool 
	*/
	static bool InBounds(Vec2 position, Vec2 a, Vec2 b);

	/**
	* Calculates new velocity for given PhysicsHitPlaneEventType, returns new velocity
	* Basicly inverts x and y for velocity depending on PhysicsHitPlaneResponseEventType
	* @param Vec2
	* @param PhysicsHitPlaneEventType
	* @return Vec2
	*/
	static Vec2 Bounce(Vec2 velocity, PhysicsHitPlaneResponseEventType side = PhysicsHitPlaneResponseEventType::Edge);

	/**
	* Calculates and returns a PhysicsHitPlaneResponseEventType based on position vector and side of given collider.
	* @param Vec2
	* @param Collider*
	* @return PhysicsHitPlaneResponseEventType
	*/
	static PhysicsHitPlaneResponseEventType CalculateHitPlaneEventType(Vec2 position, class Collider* collider);
};

#endif // !PHYSICS_H