// Main entry point for Dustville.
//
// Version: 1/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#include "../engine/core.h"

int main(int argc, char * argv[]) {
	Core::Initialize(argc, argv);

	while (Core::IsActive()) {
		Core::Update(); // Handle updates
	}

	Core::Destroy();

	return 0; // Return succes
}