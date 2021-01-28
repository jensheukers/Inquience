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
	this->velocity = Vec2(0.f, 0.f);
	this->positionLastFrame = Vec2(0, 0);

	this->bPreventSinkingBodies = true;
	this->bSimulateGravity = true;
	this->bSimulateDrag = true;
	this->gravityAmount = RIGIDBODY_GRAVITY_IMPLIER_DEFAULT;
	this->dragAmount = RIGIDBODY_DRAG_IMPLIER_DEFAULT;

	//Add Properties
	AddProperty("PreventSinkingBodies", [=](StringVector args) {
		bPreventSinkingBodies = std::stoi(args[0]);
	}, [=]() -> StringVector {
		return { std::to_string(bPreventSinkingBodies) };
	});

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

	//Reset booleans
	bDownCastPositive = false;
	bRightCastPositive = false;
	bLeftCastPositive = false;

	//Search through every entity from top in hierarchy if they have a collider component, then check for collision
	std::vector<Collider*> colliders;
	this->GetOwner()->GetHighestEntityInHierarchy()->GetAllComponentsOfTypeInChildren(colliders);

	//Check for collision, only rigidbody objects should do collision checks
	GetOwner()->GetComponent<Collider>()->CheckCollision(colliders);

	if (GetOwner()->GetComponent<Collider>()->CollisionEntered()) {
		GetOwner()->localPosition = positionLastFrame;

		//Devide velocity by 2
		velocity = velocity / 2;

		onBlockedDelegate.Execute();
		return;
	}

	positionLastFrame = GetOwner()->localPosition;

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

	//Raycast checks to prevent sinking bodies, Size of entity will be used to determine bounds
	if (this->bPreventSinkingBodies) {
		Vec2 downRight = this->GetOwner()->GetPosition() + Vec2(this->GetOwner()->GetScale().x, this->GetOwner()->GetScale().y);
		Vec2 downLeft = this->GetOwner()->GetPosition() + Vec2(0, this->GetOwner()->GetScale().y);
		Vec2 upRight = this->GetOwner()->GetPosition() + Vec2(this->GetOwner()->GetScale().x, 0);
		Vec2 upLeft = this->GetOwner()->GetPosition();

		//Downwards casting
		if (Physics::Raycast(downRight, Vec2(0, 1), 2, RaycastHit(), colliders, { GetOwner()->GetComponent<Collider>() }) ||
			Physics::Raycast(downLeft, Vec2(0, 1), 2, RaycastHit(), colliders, { GetOwner()->GetComponent<Collider>() })) {
			if (velocity.y > 0) {
				velocity = Vec2(velocity.x, 0); bDownCastPositive = true;
			}
		}

		//Right casting
		if (Physics::Raycast(upRight, Vec2(1, 0), 2, RaycastHit(), colliders, { GetOwner()->GetComponent<Collider>() }) ||
			Physics::Raycast(downRight, Vec2(1, 0), 2, RaycastHit(), colliders, { GetOwner()->GetComponent<Collider>() })) {
			if (velocity.x > 0) {
				velocity = Vec2(0, velocity.y); bRightCastPositive = true;
			}
		}

		//Left casting
		if (Physics::Raycast(upLeft, Vec2(-1, 0), 2, RaycastHit(), colliders, { GetOwner()->GetComponent<Collider>() }) ||
			Physics::Raycast(downLeft, Vec2(-1, 0), 2, RaycastHit(), colliders, { GetOwner()->GetComponent<Collider>() })) {\
			if (velocity.x < 0) {
				velocity = Vec2(0, velocity.y); bLeftCastPositive = true;
			}
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

void RigidBody::OnComponentPropertiesEditor() {
	ImGui::Checkbox("Prevent Sinking Bodies", &bPreventSinkingBodies);
	ImGui::Checkbox("Simulate Gravity", &bSimulateGravity);
	ImGui::Checkbox("Simulate Drag", &bSimulateDrag);
	ImGui::Spacing();
	ImGui::InputFloat("Gravity Amount", &gravityAmount);
	ImGui::InputFloat("Drag Amount", &dragAmount);
}