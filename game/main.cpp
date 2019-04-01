// Main entry point for Dustville.
//
// Version: 26/3/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, March 2019
#include "../engine/core.h"
#include "../engine/entity.h"
#include "../engine/texture.h"

int main(int argc, char * argv[]) {
	Core::Initialize(argc, argv);

	Texture* test = TextureLoader::LoadTarga(Core::GetExecutableDirectoryPath().append("res/test.tga").c_str());

	while (Core::IsActive()) {
		Core::Update(); // Handle updates
	}

	Core::Destroy();

	return 0; // Return succes
}