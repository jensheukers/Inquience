// Main entry point for Dustville.
//
// Version: 6/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, April 2019
#include "../engine/core.h"
#include "../engine/scenemanager.h"
#include "../engine/input.h"

int main(int argc, char * argv[]) {
	Core::Initialize(argc, argv);

	SceneManager::SetActiveScene(new Scene());
	Entity* ent = new Entity();
	ent->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/test.tga"));
	TextureLoader::LoadTarga("res/test.tga");

	SceneManager::GetActiveScene()->AddChild(ent);

	while (Core::IsActive()) {
		Core::Update(); // Handle updates

		if (Input::GetButtonDown(BUTTONCODE_LEFT)) {

		}
	}

	Core::Destroy();

	return 0; // Return succes
}