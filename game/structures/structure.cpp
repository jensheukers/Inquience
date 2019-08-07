// Source file for structure class
// Version: 7/8/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, August 2019

//Include structure.h
#include "structure.h"

//Include sprite.h
#include "../../engine/components/sprite.h"

void Structure::MasterUpdate() {
	//Check health
	if (health <= 0) {
		//Call destructor
		delete this;
	}
}

Structure::Structure() {
	//Set properties
	this->health = 100; // Set health to 100%

	//Add sprite component
	this->AddComponent<Sprite>();
}

void Structure::Update() {
	this->MasterUpdate(); // Call master update
}

void Structure::Damage(int amount) {
	this->health -= amount;
}

void Structure::Heal(int amount) {
	this->health += amount;
}