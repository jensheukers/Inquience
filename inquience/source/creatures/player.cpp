#include "player.h"

Player::Player() {
	this->collider = this->AddComponent<Collider>();
	this->movementComponent = this->AddComponent<MovementComponent>();
}

void Player::Update() {
	if (this->movementComponent->GetDirection() == Direction::Right) {
		this->PlayAnimation("run_right");
	}

	if (this->movementComponent->GetDirection() == Direction::Left) {
		this->PlayAnimation("run_left");
	}
}