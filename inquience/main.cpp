// Main entry point.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, November 2019
#include <core.h>

#include "source/gamestate.h"

int main(int argc, char * argv[]) {
	Core::Initialize(argc, argv, Vec2(1280, 720), "Inquience");

	//Create GameState and insert levels as parameter
	GameState* state = new GameState(std::vector<std::string> { 
		"res/scenes/default.scene",
	});

	state->Initialize();

	while (Core::IsActive()) {
		Core::Update(); // Handle updates for engine
	}

	return 0; // return succes
}