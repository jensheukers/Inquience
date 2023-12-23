// Source file for collider class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2021
#include "collider.h"

#include "../entity.h"
#include "core.h"

void Collider::DrawDebugLines() {
	Debug::DrawLine(GetOwner()->GetGlobalPosition(), GetOwner()->GetGlobalPosition() + Vec2(outer.x, 0), debugDrawColor);
	Debug::DrawLine(GetOwner()->GetGlobalPosition() + Vec2(outer.x, 0), GetOwner()->GetGlobalPosition() + outer, debugDrawColor);
	Debug::DrawLine(GetOwner()->GetGlobalPosition() + outer, GetOwner()->GetGlobalPosition() + Vec2(0, outer.y), debugDrawColor);
	Debug::DrawLine(GetOwner()->GetGlobalPosition() + Vec2(0, outer.y), GetOwner()->GetGlobalPosition(), debugDrawColor);
}

Collider::Collider() {
	bDrawDebugLines = false;
	debugDrawColor = glm::vec3(0, 1, 0);

	this->outer = Vec2(32, 32);
	this->scaleToOwner = true;
	this->collisionActive = false;
	this->collisionEntered = false;
	this->collisionExited = false;

	AddProperty("DrawDebug", [=](std::vector<std::string> args) {
		bDrawDebugLines = (stoi(args[0]) == 1);
		}, [=]() -> StringVector {
			return { std::to_string(bDrawDebugLines) };
		});

	AddProperty("OnCollisionEnter", [=](std::vector<std::string> args) {
		this->onCollisionEnter.AddLambda([=]() {
			Core::GetLuaScript()->RunFunction(args[0], args[1]);
			});

		this->onCollisionEnterReturnVector = args;
		}, [=]() -> StringVector {
			return onCollisionEnterReturnVector;
		});

	AddProperty("OnCollisionActive", [=](std::vector<std::string> args) {
		this->onCollisionActive.AddLambda([=]() {
			Core::GetLuaScript()->RunFunction(args[0], args[1]);
			});

		this->onCollisionActiveReturnVector = args;
		}, [=]() -> StringVector {
			return onCollisionActiveReturnVector;
		});

	AddProperty("OnCollisionExit", [=](std::vector<std::string> args) {
		this->onCollisionExit.AddLambda([=]() {
			Core::GetLuaScript()->RunFunction(args[0], args[1]);
			});

		this->onCollisionExitReturnVector = args;
		}, [=]() -> StringVector {
			return onCollisionExitReturnVector;
		});

	AddProperty("Outer", [=](std::vector<std::string> args) {
		outer = Vec2(std::stof(args[0]), std::stof(args[1]));
		}, [=]() -> StringVector {
			return { std::to_string(outer.x), std::to_string(outer.y) };
		});

	AddProperty("ScaleToOwner", [=](std::vector<std::string> args) {
		scaleToOwner = std::stoi(args[0]);
		}, [=]() -> StringVector {
			return { std::to_string(scaleToOwner) };
		});
}



void Collider::Update() {
	if (bDrawDebugLines) {
		this->DrawDebugLines();
	}

	if (this->scaleToOwner) {
		this->outer = GetOwner()->GetGlobalScale();
	}
}

void Collider::CheckCollision(const std::vector<Collider*> colliders) {
	//Reset state booleans as they will be set again in this function
	this->collisionEntered = false;
	this->collisionActive = false;
	this->collisionExited = false;

	for (size_t i = 0; i < colliders.size(); i++) {
		if (colliders[i] == this) continue; // ensure
		bool hit = (IsColliding(colliders[i]) || colliders[i]->IsColliding(this));

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


bool Collider::IsColliding(Collider* other) {
	if (Collider* otherBoxCollider = dynamic_cast<Collider*>(other)) {
		if (Physics::InBounds(GetOwner()->GetGlobalPosition(), otherBoxCollider->GetOwner()->GetGlobalPosition(), otherBoxCollider->GetOwner()->GetGlobalPosition() + otherBoxCollider->outer)) return true; // Left Up
		if (Physics::InBounds(GetOwner()->GetGlobalPosition() + Vec2(outer.x, 0), otherBoxCollider->GetOwner()->GetGlobalPosition(), otherBoxCollider->GetOwner()->GetGlobalPosition() + otherBoxCollider->outer)) return true; // Right up
		if (Physics::InBounds(GetOwner()->GetGlobalPosition() + outer, otherBoxCollider->GetOwner()->GetGlobalPosition(), otherBoxCollider->GetOwner()->GetGlobalPosition() + otherBoxCollider->outer)) return true; // Right Down
		if (Physics::InBounds(GetOwner()->GetGlobalPosition() + Vec2(0, outer.y), otherBoxCollider->GetOwner()->GetGlobalPosition(), otherBoxCollider->GetOwner()->GetGlobalPosition() + otherBoxCollider->outer)) return true; // Left down
	}
	//Return false if not found
	return false;
}

bool Collider::IsCollidingWithPoint(Vec2 point) {
	return Physics::InBounds(point, GetOwner()->GetGlobalPosition(), GetOwner()->GetGlobalPosition() + this->outer);
}

float Collider::GetSize() {
	if (outer.x > outer.y) {
		return outer.x;
	}
	return outer.y;
}

void Collider::ShowComponentProperties() {
	Debug::DrawCube(this->GetOwner()->GetGlobalPosition(), this->GetOwner()->GetGlobalPosition() + this->outer, glm::vec3(0, 1, 0));

	ImGui::InputFloat("Outer X", &this->outer.x);
	ImGui::InputFloat("Outer Y", &this->outer.y);

	ImGui::Checkbox("Scale to Owner", &this->scaleToOwner);
	ImGui::Checkbox("Draw Debug Lines", &this->bDrawDebugLines);
}