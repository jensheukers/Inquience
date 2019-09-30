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

RigidBody::RigidBody() {
	this->positionLastFrame = Vec2(0, 0);
	this->velocity = Vec2(0, 0);
}

void RigidBody::Update() {
	if (!GetOwner()->HasComponent<Collider>()) {
		Debug::Log("A RigidBody requires a Collider component, please add one to parent entity");
		return;
	}

	//Check for collision
	if (GetOwner()->GetComponent<Collider>()->collisionEntered) {
		this->velocity = Physics::Bounce(this->velocity, Physics::CalculateHitPlaneEventType(positionLastFrame, GetOwner()->GetComponent<Collider>()->hitLast));
	}

	//Add velocity on position
	GetOwner()->localPosition = GetOwner()->localPosition + (velocity * Core::GetDeltaTime());
	positionLastFrame = GetOwner()->localPosition;
}

void RigidBody::AddForce(Vec2 force) {
	velocity = velocity + force;
}

Vec2 RigidBody::GetVelocity() {
	return velocity;
}