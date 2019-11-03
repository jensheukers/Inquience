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
}

void RigidBody::Update() {
	if (!GetOwner()->HasComponent<Collider>()) {
		Debug::Log("A RigidBody requires a Collider component, please add one to parent entity");
		return;
	}

	if (GetOwner()->GetComponent<Collider>()->CollisionEntered()) {
		GetOwner()->localPosition = positionLastFrame;
		onBlockedDelegate.Execute();
		return;
	}

	positionLastFrame = GetOwner()->localPosition;

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