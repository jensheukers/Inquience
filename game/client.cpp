// Source file for client class, client handles / supervises all gameplay
// Client is a singleton instance, and should never be destroyed during gameplay
//
// Version: 6/8/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, August 2019

//Include client header
#include "client.h"

//Include engine related headers
#include "../engine/debug.h"
#include "../engine/input.h"
#include "../engine/core.h"

//Include game related headers
#include "unit.h"

//Derived -- Note we dont include SceneManager because mainmenu.h already knows about it
#include "derived/mainmenu.h"

//Include components
#include "../engine/components/animator.h"


//GameSettings constructor
GameSettings::GameSettings(std::string worldSceneFile, int start_wood, int start_stones, int start_materials) {
	//Set values
	this->worldSceneFile = worldSceneFile;
	this->start_wood = start_wood;
	this->start_stones = start_stones;
	this->start_materials = start_materials;
}

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

	//Set default boolean states
	instance->inSession = false;
}

void Client::Update() {
	//Updates

	//In-Game Updates
	if (!instance->inSession) return; // If user is not playing a session we will return here

	//Hud updates
	instance->wood_hud_text->SetText(std::to_string(instance->wood));
	instance->stones_hud_text->SetText(std::to_string(instance->stones));
	instance->materials_hud_text->SetText(std::to_string(instance->materials));

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

void Client::StartGame(GameSettings settings) {
	//Load Scene using SceneManager
	Scene* scene = new Scene();
	scene->Load((char*)settings.worldSceneFile.c_str());
	scene->SetActiveCamera(new Camera());
	Core::SwitchScene(scene);

	//Set client resources
	instance->wood = settings.start_wood;
	instance->stones = settings.start_stones;
	instance->materials = settings.start_materials;

	//Set state
	instance->inSession = true;

	//Create hud (Display amount of wood, stones and materials)

	//Wood
	instance->wood_bg = new UIElement();
	instance->wood_bg->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/hud/bg_elements.tga"));
	instance->wood_bg->GetComponent<Sprite>()->Split(32, 56);
	instance->wood_bg->GetComponent<Sprite>()->SetScale(Vec2(2.5f, 1));
	instance->wood_bg->GetComponent<Sprite>()->SetZIndex(1);

	instance->wood_hud = new UIElement();
	instance->wood_hud->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/hud/bg_elements.tga"));
	instance->wood_hud->GetComponent<Sprite>()->Split(32, 48);
	instance->wood_hud->GetComponent<Sprite>()->SetScale(Vec2(1, 1));
	instance->wood_hud->GetComponent<Sprite>()->SetZIndex(1);

	instance->wood_hud_text = new Text(FontLoader::LoadFont("res/yoster.ttf"), "0");
	instance->wood_hud_text->SetSize(0.75f);

	//Stones
	instance->stones_bg = new UIElement();
	instance->stones_bg->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/hud/bg_elements.tga"));
	instance->stones_bg->GetComponent<Sprite>()->Split(32, 56);
	instance->stones_bg->GetComponent<Sprite>()->SetScale(Vec2(2.5f, 1));
	instance->stones_bg->GetComponent<Sprite>()->SetZIndex(1);

	instance->stones_hud = new UIElement();
	instance->stones_hud->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/hud/bg_elements.tga"));
	instance->stones_hud->GetComponent<Sprite>()->Split(32, 49);
	instance->stones_hud->GetComponent<Sprite>()->SetScale(Vec2(1, 1));
	instance->stones_hud->GetComponent<Sprite>()->SetZIndex(1);

	instance->stones_hud_text = new Text(FontLoader::LoadFont("res/yoster.ttf"), "0");
	instance->stones_hud_text->SetSize(0.75f);

	//Materials
	instance->materials_bg = new UIElement();
	instance->materials_bg->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/hud/bg_elements.tga"));
	instance->materials_bg->GetComponent<Sprite>()->Split(32, 56);
	instance->materials_bg->GetComponent<Sprite>()->SetScale(Vec2(2.5f, 1));
	instance->materials_bg->GetComponent<Sprite>()->SetZIndex(1);

	instance->materials_hud = new UIElement();
	instance->materials_hud->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/hud/bg_elements.tga"));
	instance->materials_hud->GetComponent<Sprite>()->Split(32, 50);
	instance->materials_hud->GetComponent<Sprite>()->SetScale(Vec2(1, 1));
	instance->materials_hud->GetComponent<Sprite>()->SetZIndex(1);

	instance->materials_hud_text = new Text(FontLoader::LoadFont("res/yoster.ttf"), "0");
	instance->materials_hud_text->SetSize(0.75f);

	//Positions
	instance->wood_bg->localPosition = Vec2(20, 10);
	instance->wood_hud->localPosition = instance->wood_bg->localPosition;
	instance->wood_hud_text->localPosition = instance->wood_bg->localPosition + Vec2(32, 29);

	instance->stones_bg->localPosition = Vec2(120, 10);
	instance->stones_hud->localPosition = instance->stones_bg->localPosition;
	instance->stones_hud_text->localPosition = instance->stones_bg->localPosition + Vec2(32, 29);

	instance->materials_bg->localPosition = Vec2(220, 10);
	instance->materials_hud->localPosition = instance->materials_bg->localPosition;
	instance->materials_hud_text->localPosition = instance->materials_bg->localPosition + Vec2(32, 29);

	//Add to scene
	scene->AddChild(instance->wood_bg);
	scene->AddChild(instance->wood_hud);
	scene->AddChild(instance->wood_hud_text);

	scene->AddChild(instance->stones_bg);
	scene->AddChild(instance->stones_hud);
	scene->AddChild(instance->stones_hud_text);

	scene->AddChild(instance->materials_bg);
	scene->AddChild(instance->materials_hud);
	scene->AddChild(instance->materials_hud_text);
}

