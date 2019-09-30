// Header file for collisionmanager class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H
#include <vector>

#include "components/collider.h"

class CollisionManager {
private:
	static CollisionManager* instance; /***< The CollisionManager instance*/
	std::vector<Collider*> colliders; /***< List of all registered colliders*/

	/**
	* Returns or creates a new instance
	*/
	static CollisionManager* GetInstance();
public:
	/**
	* Registers a collider to the colliders list
	*/
	static void RegisterCollider(Collider* collider);

	/**
	* Removes a collider from the colliders list
	*/
	static void RemoveCollider(Collider* collider);

	/**
	* Returns vector containing all colliders
	*/
	static std::vector<Collider*> GetColliders();
};

#endif // !COLLISIONMANAGER_H