// Main entry point.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers
#include <core.h>
#include "wb_editor.h"


int main(int argc, char* argv[]) {
	Core::Initialize(argc, argv, Vec2(1280, 720), "World Builder");

	WB_Editor* editor = new WB_Editor();

	while (Core::IsActive()) {
		editor->Update();
		Core::Update(); // Handle updates for engine
	}

	return 0; // return succes
}