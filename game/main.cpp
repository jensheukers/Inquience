// Main entry point for Dustville.
//
// Version: 2/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#include "../engine/core.h"
#include "../engine/input.h"

//Define default tilemap scale
#define DEFAULT_TILEMAP_TILE_SCALE 32 

int main(int argc, char * argv[]) {
	Core::Initialize(argc, argv);

	bool editorActive = false;

	while (Core::IsActive()) {
		Core::Update(); // Handle updates

		if (Input::GetKeyDown(KEYCODE_INSERT)) {
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