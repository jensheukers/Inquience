// Main entry point for Dustville.
//
// Version: 15/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, April 2019
#include "../engine/core.h"
#include "../engine/scenemanager.h"
#include "../engine/input.h"
#include "unit.h"

int main(int argc, char * argv[]) {
	Core::Initialize(argc, argv);

	Unit* unit = new Unit();
	SceneManager::SetActiveScene(new Scene());
	SceneManager::GetActiveScene()->AddChild(unit);

	while (Core::IsActive()) {
		Core::Update(); // Handle updates
		unit->SetDestination(Input::GetMousePosition());
	}

	Core::Destroy();

	return 0; // Return succes
}