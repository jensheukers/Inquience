// Main entry point.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, November 2019
#include <core.h>
#include <component_register.h>

//Include components that we need to register
#include "source/components/healthcomponent.h"

int main(int argc, char * argv[]) {
	Core::Initialize(argc, argv, Vec2(1280, 720), "Inquience");

	// We need to register our 'custom' components to the engine
	Component_Register::RegisterComponent<HealthComponent>();

	while (Core::IsActive()) {
		Core::Update(); // Handle updates for engine
	}

	return 0; // return succes
}