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
#include "../engine/input.h"

//Include game related headers
#include "unit.h"

//Derived -- Note we dont include SceneManager because mainmenu.h already knows about it
#include "derived/mainmenu.h"

#include "../engine/components/animator.h"

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
	//instance->mainMenu = new MainMenu();
	
	//Set active
	//SceneManager::SetActiveScene(instance->mainMenu);

	Scene* scene = new Scene();
	scene->SetActiveCamera(new Camera());
	SceneManager::SetActiveScene(scene);

	std::vector<UV> frames;
	frames.push_back(Sprite::Split(TextureLoader::LoadTarga("res/terrain_tiles.tga"), 32, 56));
	frames.push_back(Sprite::Split(TextureLoader::LoadTarga("res/terrain_tiles.tga"), 32, 57));
	frames.push_back(Sprite::Split(TextureLoader::LoadTarga("res/terrain_tiles.tga"), 32, 58));

	Entity* ent = new Entity();
	ent->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/terrain_tiles.tga"));
	ent->AddComponent<Animator>();

	ent->GetComponent<Animator>()->AddAnimation(new Animation(frames));
	ent->GetComponent<Animator>()->SetActiveAnimation(0);

	scene->AddChild(ent);
}

void Client::Update() {
	//Updates

	//In-Game Updates
	if (!instance->inSession) return; // If user is not playing a session we will return here

	//If Right Mouse Button is clicked, units within X pixels of the click location, will be added to selectedUnits vector
	//If there are no units found, we will unselect all units
	bool _found = false;
	if (Input::GetButtonDown(BUTTONCODE_RIGHT)) {
		for each (Unit* u in instance->units) {
			if (Vec2::Distance(u->GetPosition(), Input::GetMousePosition() + SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition()) < UNIT_SELECT_RANGE) {
				//Check if control is not pressed, we unselect all selected units
				if (!Input::GetKeyDown(KEYCODE_LEFT_CONTROL)) {
					for (unsigned i = 0; i < instance->selectedUnits.size(); i++) {
						instance->selectedUnits.erase(instance->selectedUnits.begin() + i);
					}
				}

				instance->selectedUnits.push_back(u);
				_found = true;
			}
		}

		if (!_found) {
			for (unsigned i = 0; i < instance->selectedUnits.size(); i++) {
				instance->selectedUnits.erase(instance->selectedUnits.begin() + i);
			}
		}
	}

	//If Left Mouse button is clicked, all selected units will move to that position on the screen
	if (Input::GetButtonDown(BUTTONCODE_LEFT)) {
		for each (Unit* u in instance->selectedUnits) {
			u->SetDestination(Input::GetMousePosition() + SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition());
		}
	}
}