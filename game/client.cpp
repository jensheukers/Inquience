// Source file for client class, client handles / supervises all gameplay
// Client is a singleton instance, and should never be destroyed during gameplay
//
// Version: 23/8/2019
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
#include "structures/structure.h"
#include "derived/hud.h"

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

//Declare instances
Client* Client::instance;
bool Client::buildMode;
StructureType Client::structureType;

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
	//In-Game Updates
	if (!instance->inSession) return; // If user is not playing a session we will return here

	//Hud updates -- NOTE: We expect the text child to be the second child in the child array
	dynamic_cast<Text*>(instance->hud->wood_bg->GetChild(1))->SetText(std::to_string(instance->wood));
	dynamic_cast<Text*>(instance->hud->stones_bg->GetChild(1))->SetText(std::to_string(instance->stones));
	dynamic_cast<Text*>(instance->hud->materials_bg->GetChild(1))->SetText(std::to_string(instance->materials));

	//Camera movement
	Camera* camera = SceneManager::GetActiveScene()->GetActiveCamera();

	if (Input::GetKey(KEYCODE_W)) {
		camera->SetPosition(camera->GetPosition() - Vec2(0, Core::GetDeltaTime() * instance->cameraMovementSpeed));
	}

	if (Input::GetKey(KEYCODE_A)) {
		camera->SetPosition(camera->GetPosition() - Vec2(Core::GetDeltaTime() * instance->cameraMovementSpeed, 0));
	}

	if (Input::GetKey(KEYCODE_S)) {
		camera->SetPosition(camera->GetPosition() + Vec2(0, Core::GetDeltaTime() * instance->cameraMovementSpeed));
	}

	if (Input::GetKey(KEYCODE_D)) {
		camera->SetPosition(camera->GetPosition() + Vec2(Core::GetDeltaTime() * instance->cameraMovementSpeed, 0));
	}

	//When buildmode is activated, return out of function when if statement is finished
	if (buildMode) {
		if (Input::GetButtonDown(BUTTONCODE_LEFT)) {
			BuildStructure(structureType, Input::GetMousePosition() + camera->GetPosition());
		}
		return;
	}

	//If Right Mouse Button is clicked, units within X pixels of the click location, will be added to selectedUnits vector
	//If there are no units found, we will unselect all units
	bool _found = false;
	if (Input::GetButtonDown(BUTTONCODE_RIGHT)) {
		for each (Unit* u in instance->units) {
			if (!u->moveable) continue;
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
	instance->scene = new Scene();
	instance->scene->Load((char*)settings.worldSceneFile.c_str());
	instance->scene->SetActiveCamera(new Camera());
	Core::SwitchScene(instance->scene);

	//Set client resources
	instance->wood = settings.start_wood;
	instance->stones = settings.start_stones;
	instance->materials = settings.start_materials;

	//Set state
	instance->inSession = true;

	//Create hud instance, and add is as a child to the scene
	instance->hud = new Hud();
	instance->scene->AddChild(instance->hud); 
}

void Client::BuildStructure(StructureType type, Vec2 position) {
	if (!SceneManager::GetActiveScene()) return;

	Structure* structure;
	Worker* worker;

	//Create instance
	switch (type)
	{
	case STRUCTURE_WOODCUTTER_HUT: {
			structure = new WoodCutterHut();
			structure->localPosition = position; // Set position
			worker = new Worker(structure, Vec2(600, 600)); 
		}
		break;
	case STRUCTURE_STONE_MINE_HUT:
		break;
	case STRUCTURE_MATERIAL_MINE_HUT:
		break;
	default:
		break;
	}

	//Check costs, if user cannot afford building building will be deleted (as well as worker)
	if (structure->cost[0] > instance->wood || structure->cost[1] > instance->stones || structure->cost[2] > instance->materials) {
		delete structure;
		delete worker;

		Debug::Log("Cannot afford building");
	}
	else {
		//Remove resources
		instance->wood -= structure->cost[0];
		instance->stones -= structure->cost[1];
		instance->materials -= structure->cost[2];
	}

	instance->structures.push_back(structure); // Push back
	instance->scene->AddChild(structure);

	instance->scene->AddChild(worker);
	instance->units.push_back(worker);
}