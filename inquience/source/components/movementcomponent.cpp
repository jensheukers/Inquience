#include "movementcomponent.h"

#include <debug.h>

#include <entity.h>
#include <components/rigidbody.h>

MovementComponent::MovementComponent() {
	this->jumpForce = 100;
	this->stepSpeed = 200;
	this->grounded = false; // Initialize as false, so gravity will be added
}

void MovementComponent::BeginPlay() {
	if (this->rigidBody = GetOwner()->GetComponent<RigidBody>()) {
		rigidBody->onBlockedDelegate.AddLambda([=]() {
			rigidBody->SetVelocity(Vec2(0.f, 0.f));
		});

		RaycastData* groundRayCast = rigidBody->AddRaycast(new RaycastData(Vec2(0, 1), 32));
		groundRayCast->onRaycastHit.AddLambda([=]() {
			this->grounded = true;
		});
	}
	else {
		Debug::Log("WARNING: MovementComponent added BEFORE rigidbody, Rigidbody should always be added before MovementComponent");
	}
}

void MovementComponent::Step(int side) {
	rigidBody->SetVelocity(Vec2((float)side * this->stepSpeed, rigidBody->GetVelocity().y));
}

void MovementComponent::Jump() {
	if (this->grounded) {
		rigidBody->AddForce(Vec2(0, -jumpForce));
		this->grounded = false;
	}
}

void MovementComponent::SetJumpForce(float force) {
	this->jumpForce = force;
}

const float MovementComponent::GetJumpForce() {
	return this->jumpForce;
}

void MovementComponent::SetStepSpeed(float speed) {
	this->stepSpeed = speed;
}

const float MovementComponent::GetStepSpeed() {
	return this->stepSpeed;
}

const bool MovementComponent::IsGrounded() {
	return this->grounded;
}