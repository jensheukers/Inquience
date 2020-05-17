#include "movementcomponent.h"

#include <debug.h>

#include <entity.h>
#include <components/rigidbody.h>

//Get Input
#include <input.h>

void MovementComponent::BeginPlay() {
	if (!(this->rigidBody = GetOwner()->GetComponent<RigidBody>())) {
		Debug::Log("WARNING: MovementComponent added BEFORE rigidbody, Rigidbody should always be added before MovementComponent");
	}
}

void MovementComponent::Update() {
	if (Input::GetKey(KEYCODE_D)) {
		this->StepRight(movementSpeed);
	}

	if (Input::GetKey(KEYCODE_A)) {
		this->StepLeft(movementSpeed);
	}

	if (Input::GetKeyDown(KEYCODE_SPACE)) {
		this->Jump(jumpForce);
	}
}

void MovementComponent::StepLeft(float speed) {
	if (rigidBody->LeftCastPositive()) return;
	rigidBody->SetVelocity(Vec2(-(float)speed, rigidBody->GetVelocity().y));
}

void MovementComponent::StepRight(float speed) {
	if (rigidBody->RightCastPositive()) return;
	rigidBody->SetVelocity(Vec2((float)speed, rigidBody->GetVelocity().y));
}

void MovementComponent::Jump(float force) {
	if (!rigidBody->DownCastPositive()) return;
	rigidBody->AddForce(Vec2(0, -force));
}