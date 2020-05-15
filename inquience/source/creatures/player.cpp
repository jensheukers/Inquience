#include "player.h"

//Get Input
#include <input.h>

Player::Player() {
	this->collider = this->AddComponent<BoxCollider>();
	this->rigidBody = this->AddComponent<RigidBody>();
	this->movementComponent = this->AddComponent<MovementComponent>();
}

void Player::Update() {
	if (Input::GetKey(KEYCODE_D)) {
		this->movementComponent->StepRight(50.f);
	}

	if (Input::GetKey(KEYCODE_A)) {
		this->movementComponent->StepLeft(50.f);
	}

	if (Input::GetKeyDown(KEYCODE_SPACE)) {
		this->movementComponent->Jump(200.f);
	}
}