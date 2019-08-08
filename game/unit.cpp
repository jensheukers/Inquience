// Source file for unit class.
//
// Version: 7/8/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, August 2019
#include "unit.h"

//include engine related headers
#include "../engine/core.h"
#include "../engine/components/sprite.h"

//Game related headers
#include "structures/structure.h"

void Unit::MasterUpdate() {
	//Unit movement
	if (Vec2::Distance(this->localPosition, this->destination) > 32) {
		Vec2 direction = this->GetPosition().DirectionNormalized(this->destination);
		this->localPosition = this->localPosition + (direction * (Core::GetDeltaTime() * this->speed));
		atDestination = false;
	}
	else {
		atDestination = true;
	}
}

Unit::Unit() {
	//Unit has a sprite component added by default, and default texture is set to placeholder
	this->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/placeholder.tga"));

	// Unit default speed is 100
	this->speed = 100;
	moveable = true;
}

void Unit::Update() {
	//Master update
	this->MasterUpdate();
}

Vec2 Unit::GetDestination() {
	return destination;
}

void Unit::SetDestination(Vec2 destination) {
	this->destination = destination;
	this->atDestination = false;
}

Worker::Worker(Structure* house, Vec2 workplace) {
	this->house = house;
	this->workplace = workplace;

	//Set our position to house position
	this->localPosition = house->localPosition;

	//Worker automaticly goes to the workplace when initialized
	this->SetDestination(workplace);

	//Make worker not movable by user
	moveable = false;
}

void Worker::Update() {
	//Master update
	this->MasterUpdate();

	if (this->GetDestination() == workplace && atDestination) {
		//Worker is at its workplace
		Work();
	}

	if (this->GetDestination() == this->house->localPosition && atDestination) {
		//Worker is home
		AtHome();
	}
}

void Worker::Work() {
	this->SetDestination(this->house->localPosition);
}

void Worker::AtHome() {
	this->SetDestination(workplace);
}