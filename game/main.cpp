// Main entry point for Dustville.
//
// Version: 1/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#include "../engine/core.h"
#include "../engine/input.h"
#include "../engine/scenemanager.h"
#include "unit.h"

//Define default tilemap scale
#define DEFAULT_TILEMAP_TILE_SCALE 32 

int main(int argc, char * argv[]) {
	Core::Initialize(argc, argv);

	SceneManager::SetActiveScene(new Scene());
	SceneManager::GetActiveScene()->SetActiveCamera(new Camera());

	Unit* unit = new Unit();
	SceneManager::GetActiveScene()->AddChild(unit);

	while (Core::IsActive()) {
		Core::Update(); // Handle updates

		float angle = unit->GetPosition().Angle(Input::GetMousePosition());
		Debug::Log(std::to_string(Vec2::RadToDeg(angle)));

		if (Input::GetButtonDown(BUTTONCODE_LEFT)) {
			unit->SetDestination(Input::GetMousePosition());
		}
	}

	Core::Destroy();
	return 0; // return succes
}