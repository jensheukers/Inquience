// Source file for collider class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#include "collider.h"

#include "../collisionmanager.h"
#include "../entity.h"
#include "rigidbody.h"

void Collider::BeginPlay() {
	CollisionManager::RegisterCollider(this);
	debugDrawColor = glm::vec3(0, 1, 0);

	AddProperty("DrawDebug", [=](std::vector<std::string> args) {
		bDrawDebugLines = (stoi(args[0]) == 1) ? true : false;
	});

	AddProperty("Outer", [=](std::vector<std::string> args) {
		outer = Vec2(std::stof(args[0]), std::stof(args[1]));
	});
}

void Collider::Update() {
	if (bDrawDebugLines) DrawDebugLines();
}

void Collider::CheckCollision() {
	//Reset booleans
	collisionEntered = false;
	collisionActive = false;
	collisionExited = false;

	//We will check for every other collider if we are in fact colliding
	//Note that in order for any object to be passed into the collision check, that object must have a RigidBody component
	for (size_t i = 0; i < CollisionManager::GetColliders().size(); i++) {
		if (CollisionManager::GetColliders()[i] == this) continue;
		bool hit = IsColliding(CollisionManager::GetColliders()[i]);
		
		//Check if a rigidbody exists on the other collider's parent, if so we want to block its movement for this frame
		if (hit && CollisionManager::GetColliders()[i]->GetOwner()->HasComponent<RigidBody>()) {
			CollisionManager::GetColliders()[i]->GetOwner()->GetComponent<RigidBody>()->isBlockedThisFrame = true;
		}

		bool hitPreviousFrame = false;
		int lastHitIndex = 0;
		for (size_t ii = 0; ii < hits.size(); ii++) {
			if (hits[ii] == CollisionManager::GetColliders()[i]) {
				hitPreviousFrame = true;
				lastHitIndex = ii;
				break;
			}
		}

		if (hit && !hitPreviousFrame) {
			collisionEntered = true;
			onCollisionEnter.Execute(); 
			hits.push_back(CollisionManager::GetColliders()[i]); 
			hitLast = CollisionManager::GetColliders()[i];
		}

		if (hit && hitPreviousFrame) {
			collisionActive = true;
			onCollision.Execute();
		}

		if (!hit && hitPreviousFrame) {
			collisionExited = true;
			onCollisionExit.Execute();
			hits.erase(hits.begin() + lastHitIndex);
		}
	}
}

std::vector<Collider*> Collider::GetHits() {
	return this->hits;
}

Collider::~Collider() {
	CollisionManager::RemoveCollider(this);
}

void BoxCollider::DrawDebugLines() {
	Debug::DrawLine(GetOwner()->GetPosition(), GetOwner()->GetPosition() + Vec2(outer.x, 0), debugDrawColor);
	Debug::DrawLine(GetOwner()->GetPosition() +  Vec2(outer.x, 0), GetOwner()->GetPosition() + outer, debugDrawColor);
	Debug::DrawLine(GetOwner()->GetPosition() + outer, GetOwner()->GetPosition() + Vec2(0, outer.y), debugDrawColor);
	Debug::DrawLine(GetOwner()->GetPosition() + Vec2(0, outer.y), GetOwner()->GetPosition(), debugDrawColor);
}

bool BoxCollider::IsColliding(Collider* other) {
	Vec2 position = GetOwner()->GetPosition();
	Vec2 boundsPosition = position + outer;

	//Compare all
	if (Physics::InBounds(other->GetOwner()->GetPosition(), position, boundsPosition)) return true; // Left Up
	if (Physics::InBounds(other->GetOwner()->GetPosition() + Vec2(other->outer.x, 0), position, boundsPosition)) return true; // Right up
	if (Physics::InBounds(other->GetOwner()->GetPosition() + other->outer, position, boundsPosition)) return true; // Right Down
	if (Physics::InBounds(other->GetOwner()->GetPosition() + Vec2(0, other->outer.y), position, boundsPosition)) return true; // Left down

	//Return false if not found
	return false;
}