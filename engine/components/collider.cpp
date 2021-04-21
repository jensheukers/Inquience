// Source file for collider class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#include "collider.h"

#include "../entity.h"

Collider::Collider() {
	bDrawDebugLines = false;
	debugDrawColor = glm::vec3(0, 1, 0);

	AddProperty("DrawDebug", [=](std::vector<std::string> args) {
		bDrawDebugLines = (stoi(args[0]) == 1);
	}, [=]() -> StringVector {
		return { std::to_string(bDrawDebugLines) };
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

void BoxCollider::DrawDebugLines() {
	Debug::DrawLine(GetOwner()->position, GetOwner()->position + Vec2(outer.x, 0), debugDrawColor);
	Debug::DrawLine(GetOwner()->position +  Vec2(outer.x, 0), GetOwner()->position + outer, debugDrawColor);
	Debug::DrawLine(GetOwner()->position + outer, GetOwner()->position + Vec2(0, outer.y), debugDrawColor);
	Debug::DrawLine(GetOwner()->position + Vec2(0, outer.y), GetOwner()->position, debugDrawColor);
}

BoxCollider::BoxCollider() {
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

void BoxCollider::Update() {
	if (bDrawDebugLines) {
		this->DrawDebugLines();
	}

	if (this->scaleToOwner) {
		this->outer = GetOwner()->scale;
	}
}

bool BoxCollider::IsColliding(Collider* other) {
	if (BoxCollider * otherBoxCollider = dynamic_cast<BoxCollider*>(other)) {
		if (Physics::InBounds(GetOwner()->position, otherBoxCollider->GetOwner()->position, otherBoxCollider->GetOwner()->position + otherBoxCollider->outer)) return true; // Left Up
		if (Physics::InBounds(GetOwner()->position + Vec2(outer.x, 0), otherBoxCollider->GetOwner()->position, otherBoxCollider->GetOwner()->position + otherBoxCollider->outer)) return true; // Right up
		if (Physics::InBounds(GetOwner()->position + outer, otherBoxCollider->GetOwner()->position, otherBoxCollider->GetOwner()->position + otherBoxCollider->outer)) return true; // Right Down
		if (Physics::InBounds(GetOwner()->position + Vec2(0, outer.y), otherBoxCollider->GetOwner()->position, otherBoxCollider->GetOwner()->position + otherBoxCollider->outer)) return true; // Left down
	}
	//Return false if not found
	return false;
}

bool BoxCollider::IsCollidingWithPoint(Vec2 point) {
	return Physics::InBounds(point, GetOwner()->position, GetOwner()->position + this->outer);
}

float BoxCollider::GetSize() {
	if (outer.x > outer.y) {
		return outer.x;
	}
	return outer.y;
}

void BoxCollider::OnComponentPropertiesEditor() {
	Debug::DrawCube(this->GetOwner()->position, this->GetOwner()->position + this->outer, glm::vec3(0, 1, 0));

	ImGui::InputFloat("Outer X", &this->outer.x);
	ImGui::InputFloat("Outer Y", &this->outer.y);
	
	ImGui::Checkbox("Scale to Owner", &this->scaleToOwner);
	ImGui::Checkbox("Draw Debug Lines", &this->bDrawDebugLines);
}