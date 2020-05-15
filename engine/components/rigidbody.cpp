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

RigidBody::RigidBody() {
	this->velocity = Vec2(0, 0.f);
	this->positionLastFrame = Vec2(0, 0);

	this->bPreventSinkingBodies = true;
	this->bSimulateGravity = true;
	this->bSimulateDrag = true;
}

void RigidBody::Update() {
	if (!GetOwner()->HasComponent<Collider>()) {
		Debug::Log("A RigidBody requires a Collider component, please add one to parent entity");
		return;
	}

	//Search through every entity from top in hierarchy if they have a collider component, then check for collision
	std::vector<Collider*> colliders;
	this->GetOwner()->GetHighestEntityInHierarchy()->GetAllComponentsOfTypeInChildren(colliders);

	//Check for collision, only rigidbody objects should do collision checks
	GetOwner()->GetComponent<Collider>()->CheckCollision(colliders);

	if (GetOwner()->GetComponent<Collider>()->CollisionEntered()) {
		GetOwner()->localPosition = positionLastFrame;
		onBlockedDelegate.Execute();
		return;
	}

	positionLastFrame = GetOwner()->localPosition;

	if (this->bSimulateGravity) {
		velocity = velocity + Vec2(0, RIGIDBODY_GRAVITY_IMPLIER);
	}

	if (this->bSimulateDrag) {
		if (velocity.x > 0) {
			velocity = velocity - Vec2(RIGIDBODY_DRAG_IMPLIER, 0);
		}

		if (velocity.x < 0) {
			velocity = velocity + Vec2(RIGIDBODY_DRAG_IMPLIER, 0);
		}
	}

	//Raycast checks to prevent sinking bodies
	if (this->bPreventSinkingBodies) {
		Vec2 downRight = this->GetOwner()->GetPosition() + Vec2(this->GetOwner()->GetScale().x, this->GetOwner()->GetScale().y);
		Vec2 downLeft = this->GetOwner()->GetPosition() + Vec2(0, this->GetOwner()->GetScale().y);

		//Reset y velocity if true
		if (Physics::Raycast(downRight, Vec2(0, 1), 2, RaycastHit(), colliders, { GetOwner()->GetComponent<Collider>() }) ||
			Physics::Raycast(downLeft, Vec2(0, 1), 2, RaycastHit(), colliders, { GetOwner()->GetComponent<Collider>() })) {
			velocity = Vec2(velocity.x, 0);
		}
	}

	//Add velocity to position
	GetOwner()->localPosition = GetOwner()->localPosition + (velocity * Core::GetDeltaTime());
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