// Main entry point for Dustville.
//
// Version: 2/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#include "../engine/core.h"

//Define default tilemap scale
#define DEFAULT_TILEMAP_TILE_SCALE 32 

int main(int argc, char * argv[]) {
	Core::Initialize(argc, argv);

	Core::EnableEditor(true);

	while (Core::IsActive()) {
		Core::Update(); // Handle updates
	}

	Core::Destroy();
	return 0; // return succes
}