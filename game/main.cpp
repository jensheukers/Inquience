// Main entry point for Dustville.
//
// Version: 1/8/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, August 2019
#include "../engine/core.h"
#include "../engine/input.h"

//Include client
#include "client.h"

//Define default tilemap scale
#define DEFAULT_TILEMAP_TILE_SCALE 32 

int main(int argc, char * argv[]) {
	Core::Initialize(argc, argv);

	//Create the client
	new Client();

	//Editor
	bool editorActive = false;

	while (Core::IsActive()) {
		Core::Update(); // Handle updates for engine
		Client::Update(); // Update for game

		if (Input::GetKeyDown(KEYCODE_GRAVE_ACCENT)) {
			if (!editorActive) {
				Core::EnableEditor(true);
				editorActive = true;
			}
			else {
				Core::EnableEditor(false);
				editorActive = false;
			}

		}
	}

	Core::Destroy();
	return 0; // return succes
}