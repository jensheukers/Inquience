#include "player.h"

//Get Input
#include <input.h>

Player::Player() {
	this->collider = this->AddComponent<BoxCollider>();
	this->rigidBody = this->AddComponent<RigidBody>();
	this->movementComponent = this->AddComponent<MovementComponent>();

	//Add movment axises
	Input::AddAxis("Horizontal", new Axis(KEYCODE_D, KEYCODE_A));
}

void Player::Update() {
}