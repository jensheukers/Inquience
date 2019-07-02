// Source file for client class, client handles / supervises all gameplay
// Client is a singleton instance, and should never be destroyed during gameplay
//
// Version: 2/7/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, July 2019

//Include client header
#include "client.h"

//Include engine related headers
#include "../engine/debug.h"

//Include game related headers
#include "unit.h"

//Derived -- Note we dont include SceneManager because mainmenu.h already knows about it
#include "derived/mainmenu.h"

Client* Client::instance; // Declare instance

Client::Client() {
	if (!instance) {
		instance = this;
		instance->Start();
		Debug::Log("Client created!");
	}
	else {
		delete this;
	}
}

void Client::Start() {
	//Load the main menu first
	instance->mainMenu = new MainMenu();
	
	//Set active
	SceneManager::SetActiveScene(instance->mainMenu);
}

void Client::Update() {
	//Updates
}