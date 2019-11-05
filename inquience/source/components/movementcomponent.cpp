#include "movementcomponent.h"

#include <debug.h>

#include <entity.h>
#include <components/rigidbody.h>

MovementComponent::MovementComponent() {
	this->canStepLeft = true;
	this->canStepRight = true;
	this->grounded = false;
}

void MovementComponent::BeginPlay() {
	if (!(this->rigidBody = GetOwner()->GetComponent<RigidBody>())) {
		Debug::Log("WARNING: MovementComponent added BEFORE rigidbody, Rigidbody should always be added before MovementComponent");
	}
}

void MovementComponent::ResetHorizontalVelocity() {
	rigidBody->SetVelocity(Vec2(0, rigidBody->GetVelocity().y));
}

void MovementComponent::StepLeft(float speed) {
	if (!this->canStepLeft) return;
	rigidBody->SetVelocity(Vec2(-(float)speed, rigidBody->GetVelocity().y));

	this->canStepRight = true;
}

void MovementComponent::StepRight(float speed) {
	if (!this->canStepRight) return;
	rigidBody->SetVelocity(Vec2((float)speed, rigidBody->GetVelocity().y));

	this->canStepLeft = true;
}

void MovementComponent::Jump(float force) {
	if (!this->grounded) return;
	rigidBody->AddForce(Vec2(0, -force));
	this->grounded = false;
}

void MovementComponent::AddRaycasts(const float size) {
	//These raycasts must stop the player from moving in x velocity when stopped
	//Raycast to check if entity is grounded
	groundRaycast = rigidBody->AddRaycast(new RaycastData(Vec2(0, 1), size, Vec2(size, size)));
	groundRaycast->onRaycastHit.AddLambda([=]() {
		this->grounded = true;
		rigidBody->SetVelocity(Vec2(rigidBody->GetVelocity().x, 0));
		});

	//Raycast to check if entity is walking into walls, (So they cant clip to walls)
	leftRaycast = rigidBody->AddRaycast(new RaycastData(Vec2(-1, 0), size, Vec2(size, size)));
	leftRaycast->onRaycastHit.AddLambda([=]() {
		this->canStepLeft = false;
		rigidBody->SetVelocity(Vec2(0, rigidBody->GetVelocity().y));
		});

	rightRaycast = rigidBody->AddRaycast(new RaycastData(Vec2(1, 0), size, Vec2(size, size)));
	rightRaycast->onRaycastHit.AddLambda([=]() {
		this->canStepRight = false;
		rigidBody->SetVelocity(Vec2(0, rigidBody->GetVelocity().y));
		});
}

void MovementComponent::RemoveRaycasts() {
	delete rigidBody->RemoveRaycast(this->groundRaycast);
	delete rigidBody->RemoveRaycast(this->leftRaycast);
	delete rigidBody->RemoveRaycast(this->rightRaycast);
}

const bool MovementComponent::IsGrounded() {
	return this->grounded;
}

MovementComponent::~MovementComponent() {
	this->RemoveRaycasts();
}