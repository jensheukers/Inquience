// Header file for Physics class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#ifndef PHYSICS_H
#define PHYSICS_H
#include <vector>

//Include vec2.h
#include "vec2.h"

struct RaycastHit {
	class Collider* collider = nullptr; /***< The collider the raycast hit with*/
};

class Physics {
public:
	/**
	* Executes a raycast in given direction, does a check for layers given
	* @param Vec2 origin
	* @param Vec2 direction
	* @param RaycastHit& out
	* @param std::vector<Collider*> colliders
	* @param std::vector<Collider*> collidersToIgnore
	* @param bool success
	*/
	static bool Raycast(Vec2 origin, Vec2 direction, float length, RaycastHit& out, std::vector<class Collider*> colliders, std::vector<class Collider*> ignore = {});

	/**
	* Returns true if position is between a and b
	* @param Vec2
	* @param Vec2
	* @param Vec2
	* @return bool 
	*/
	static bool InBounds(Vec2 position, Vec2 a, Vec2 b);
};

#endif // !PHYSICS_H