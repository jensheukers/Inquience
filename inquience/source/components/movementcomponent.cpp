#include "movementcomponent.h"

#include <debug.h>

#include <entity.h>
#include <core.h>

#include <components/collider.h>

//Get Input
#include <input.h>

void MovementComponent::Move(Vec2 direction) {
	Vec2 oldPos = GetOwner()->position;

	GetOwner()->position = GetOwner()->position + (direction * Core::GetDeltaTime());

	std::vector<Collider*> colliders;
	GetOwner()->GetHighestEntityInHierarchy()->GetAllComponentsOfTypeInChildren<Collider>(colliders);
	collider->CheckCollision(colliders);

	if (collider->CollisionActive()) {
		GetOwner()->position = oldPos;
	}
}

MovementComponent::MovementComponent() {
	direction = Direction::Right;
}

void MovementComponent::BeginPlay() {
	if (!(this->collider = GetOwner()->GetComponent<Collider>())) {
		Debug::Log("Error: No collider component added to Owner");
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
	Move(Vec2(-(float)speed, 0));
	this->direction = Direction::Left;
}

void MovementComponent::StepRight(float speed) {
	Move(Vec2((float)speed, 0));
	this->direction = Direction::Right;
}

void MovementComponent::Jump(float force) {
	Move(Vec2(0, -force));
	this->direction = Direction::Up;
}

Direction MovementComponent::GetDirection() {
	return this->direction;
}