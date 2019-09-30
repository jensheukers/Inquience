// Header file for collisionmanager class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#include "collisionmanager.h"

CollisionManager* CollisionManager::instance;

CollisionManager* CollisionManager::GetInstance() {
	if (!instance) {
		instance = new CollisionManager();
	}
	return instance;
}

void CollisionManager::RegisterCollider(Collider* collider) {
	GetInstance()->colliders.push_back(collider);
}

void CollisionManager::RemoveCollider(Collider* collider) {
	for (size_t i = 0; i < GetInstance()->colliders.size(); i++) {
		if (GetInstance()->colliders[i] == collider) {
			GetInstance()->colliders.erase(GetInstance()->colliders.begin() + i);
		}
	}
}

std::vector<Collider*> CollisionManager::GetColliders() {
	return GetInstance()->colliders;
}