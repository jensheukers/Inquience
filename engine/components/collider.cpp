// Source file for collider class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#include "collider.h"

#include "../collisionmanager.h"
#include "../entity.h"

void Collider::BeginPlay() {
	CollisionManager::RegisterCollider(this);
	debugDrawColor = glm::vec3(0, 1, 0);

	AddProperty("DrawDebug", [=](std::vector<std::string> args) {
		bDrawDebugLines = (stoi(args[0]) == 1);
	}, [=]() -> StringVector {
		return { std::to_string(bDrawDebugLines) };
	});

	AddProperty("Outer", [=](std::vector<std::string> args) {
		outer = Vec2(std::stof(args[0]), std::stof(args[1]));
	}, [=]() -> StringVector {
		return { std::to_string(outer.x), std::to_string(outer.y) };
	});
}

void Collider::Update() {
	if (bDrawDebugLines) {
		this->DrawDebugLines();
	}
}

void Collider::CheckCollision(const std::vector<Collider*> colliders) {
	//Reset state booleans as they will be set again in this function
	this->collisionEntered = false;
	this->collisionActive = false;
	this->collisionExited = false;

	for (size_t i = 0; i < colliders.size(); i++) {
		if (colliders[i] == this) continue; // ensure
		bool hit = IsColliding(colliders[i]);

		//Check if contains, if so do some functionality
		bool contains = false;
		for (size_t ii = 0; ii < hits.size(); ii++) {
			if (hits[ii] == colliders[i]) {
				contains = true;
				if (!hit) {
					hits.erase(hits.begin() + ii);
					collisionExited = true;
				}
				else {
					collisionActive = true;
				}
			}
		}

		if (!contains && hit) {
			hits.push_back(colliders[i]);
			collisionEntered = true;
		}
	}

	if (CollisionEntered()) onCollisionEnter.Execute();
	if (CollisionActive()) onCollisionActive.Execute();
	if (CollisionExited()) onCollisionExit.Execute();
}

std::vector<Collider*> const Collider::GetHits() {
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

// We check if WE are colliding WITH other
bool BoxCollider::IsColliding(Collider* other) {
	if (Physics::InBounds(GetOwner()->GetPosition(), other->GetOwner()->GetPosition(), other->GetOwner()->GetPosition() + other->outer)) return true; // Left Up
	if (Physics::InBounds(GetOwner()->GetPosition() + Vec2(outer.x, 0), other->GetOwner()->GetPosition(), other->GetOwner()->GetPosition() + other->outer)) return true; // Right up
	if (Physics::InBounds(GetOwner()->GetPosition() + outer, other->GetOwner()->GetPosition(), other->GetOwner()->GetPosition() + other->outer)) return true; // Right Down
	if (Physics::InBounds(GetOwner()->GetPosition() + Vec2(0, outer.y), other->GetOwner()->GetPosition(), other->GetOwner()->GetPosition() + other->outer)) return true; // Left down

	//Return false if not found
	return false;
}