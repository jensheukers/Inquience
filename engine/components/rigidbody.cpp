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
	this->gravity = Vec2(0, 0.9f);
	this->maxVelocity = Vec2(500.f, 500.f);
	this->isGravityActive = true;
}

void RigidBody::Update() {
	if (!GetOwner()->HasComponent<Collider>()) {
		Debug::Log("A RigidBody requires a Collider component, please add one to parent entity");
		return;
	}

	if (isBlockedThisFrame) {
		GetOwner()->localPosition = positionLastFrame;
		isBlockedThisFrame = false;
		onBlockedDelegate.Execute();
		return;
	}

	positionLastFrame = GetOwner()->localPosition;

	//Add gravity
	if (isGravityActive) {
		velocity = velocity + gravity;
	}

	//Check if velocity exeeds maxVelocity
	if (velocity.x > maxVelocity.x) {
		velocity.x = maxVelocity.x;
	}

	if (velocity.y > maxVelocity.y) {
		velocity.y = maxVelocity.y;
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

Vec2 RigidBody::GetVelocity() {
	return velocity;
}