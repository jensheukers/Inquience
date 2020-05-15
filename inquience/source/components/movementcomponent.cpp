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

const bool MovementComponent::IsGrounded() {
	return this->grounded;
}