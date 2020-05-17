#include "player.h"

Player::Player() {
	this->collider = this->AddComponent<BoxCollider>();
	this->rigidBody = this->AddComponent<RigidBody>();
	this->movementComponent = this->AddComponent<MovementComponent>();
}