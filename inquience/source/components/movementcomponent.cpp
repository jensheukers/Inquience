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
	GetOwner()->RefreshGlobalPosition();

	std::vector<Collider*> colliders;
	GetOwner()->GetHighestEntityInHierarchy()->GetAllComponentsOfTypeInChildren<Collider>(colliders);
	collider->CheckCollision(colliders);

	if (collider->CollisionActive() || collider->CollisionEntered()) {
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

	if (Input::GetKey(KEYCODE_W)) {
		this->StepBackward(movementSpeed);
	}

	if (Input::GetKey(KEYCODE_S)) {
		this->StepForward(movementSpeed);
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

void MovementComponent::StepForward(float speed) {
	Move(Vec2(0, (float)speed));
	this->direction = Direction::Down;
}

void MovementComponent::StepBackward(float speed) {
	Move(Vec2(0, -(float)speed));
	this->direction = Direction::Up;
}

Direction MovementComponent::GetDirection() {
	return this->direction;
}