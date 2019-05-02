// Source file for unit class.
//
// Version: 1/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#include "unit.h"
#include "../engine/core.h"
#include "../engine/components/sprite.h"

Unit::Unit() {
	//Unit has a sprite component added by default, and default texture is set to placeholder
	this->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/placeholder.tga"));

	// Unit default speed is 100
	this->speed = 100;
}

void Unit::Update() {
	//Unit movement
	if (Vec2::Distance(this->localPosition, this->destination) > 32) {
		Vec2 direction = this->GetPosition().DirectionNormalized(this->destination);
		this->localPosition = this->localPosition + (direction * (Core::GetDeltaTime() * this->speed));
	}
}

Vec2 Unit::GetDestination() {
	return destination;
}

void Unit::SetDestination(Vec2 destination) {
	this->destination = destination;
}