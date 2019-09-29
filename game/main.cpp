// Main entry point.
<<<<<<< HEAD
=======
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, September 2019
>>>>>>> development
#include "../engine/core.h"
#include "../engine/input.h"

#include "../engine/scenemanager.h"

#include "../engine/luascript.h"

int main(int argc, char * argv[]) {
	Core::Initialize(argc, argv);

	while (Core::IsActive()) {
		Core::Update(); // Handle updates for engine
	}

	return 0; // return succes
}