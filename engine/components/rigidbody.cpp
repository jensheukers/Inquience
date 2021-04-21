// Source file for rigidbody class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#include "rigidbody.h"

#include "../core.h"
#include "../entity.h"
#include "../debug.h"

#include "collider.h"

void RigidBody::HandleMovement() {
	//Search through every entity from top in hierarchy if they have a collider component, then check for collision
	std::vector<Collider*> colliders;
	this->GetOwner()->GetHighestEntityInHierarchy()->GetAllComponentsOfTypeInChildren(colliders);

	positionLastFrame = GetOwner()->position;

	//Add Horizontal velocity
	GetOwner()->position = GetOwner()->position + (velocity * Core::GetDeltaTime());

	//Check for collision
	GetOwner()->GetComponent<Collider>()->CheckCollision(colliders);

	if (GetOwner()->GetComponent<Collider>()->CollisionEntered()) {
		GetOwner()->position = positionLastFrame;

		onBlockedDelegate.Execute();
	}
}

RigidBody::RigidBody() {
	this->velocity = Vec2(0.f, 0.f);
	this->positionLastFrame = Vec2(0, 0);

	this->bSimulateGravity = true;
	this->bSimulateDrag = true;
	this->gravityAmount = RIGIDBODY_GRAVITY_IMPLIER_DEFAULT;
	this->dragAmount = RIGIDBODY_DRAG_IMPLIER_DEFAULT;

	//Add Properties
	AddProperty("SimulateGravity", [=](StringVector args) {
		bSimulateGravity = std::stoi(args[0]);
	}, [=]() -> StringVector {
		return { std::to_string(bSimulateGravity) };
	});

	AddProperty("SimulateDrag", [=](StringVector args) {
		bSimulateDrag = std::stoi(args[0]);
	}, [=]() -> StringVector {
		return { std::to_string(bSimulateDrag) };
	});

	AddProperty("GravityAmount", [=](StringVector args) {
		gravityAmount = std::stof(args[0]);
	}, [=]() -> StringVector {
		return {std::to_string(gravityAmount) };
	});

	AddProperty("DragAmount", [=](StringVector args) {
		dragAmount = std::stof(args[0]);
	}, [=]() -> StringVector {
		return { std::to_string(dragAmount) };
	});
}

void RigidBody::Update() {
	if (!GetOwner()->HasComponent<Collider>()) {
		Debug::Log("A RigidBody requires a Collider component, please add one to parent entity");
		return;
	}

	if (this->bSimulateGravity) {
		velocity = velocity + Vec2(0, gravityAmount);
	}

	if (this->bSimulateDrag) {
		if (velocity.x > 0) {
			velocity = velocity - Vec2(dragAmount, 0);
		}

		if (velocity.x < 0) {
			velocity = velocity + Vec2(dragAmount, 0);
		}

		if (velocity.x > -RIGIDBODY_VELOCITY_MIN_CUT_TO_ZERO && velocity.x < RIGIDBODY_VELOCITY_MIN_CUT_TO_ZERO) {
			velocity = Vec2(0, velocity.y);
		}
	}

	HandleMovement();
}

void RigidBody::AddForce(Vec2 force) {
	velocity = velocity + force;
}

void RigidBody::SetVelocity(Vec2 velocity) {
	this->velocity = velocity;
}

const Vec2 RigidBody::GetVelocity() {
	return velocity;
}

void RigidBody::OnComponentPropertiesEditor() {
	ImGui::Checkbox("Simulate Gravity", &bSimulateGravity);
	ImGui::Checkbox("Simulate Drag", &bSimulateDrag);
	ImGui::Spacing();
	ImGui::InputFloat("Gravity Amount", &gravityAmount);
	ImGui::InputFloat("Drag Amount", &dragAmount);
}