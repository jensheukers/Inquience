// Main entry point.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, April 2020
#include <core.h>
#include <parser.h>


#include "source/gamestate.h"

int main(int argc, char * argv[]) {
	Core::Initialize(argc, argv, Vec2(1280, 720), "Inquience");

	/*//Create GameState and insert levels as parameter
	static GameState* gameState = new GameState(std::vector<std::string> { 
		"res/scenes/mainmenu.scene",
		"res/scenes/test_world.scene",
		"res/scenes/levels/level_0.scene"
	});
	gameState->Initialize();*/

	Scene* scene = new Scene();
	SceneManager::SetActiveScene(scene);
	SceneManager::GetActiveScene()->SetActiveCamera(new Camera());

	while (Core::IsActive()) {
		Core::Update(); // Handle updates for engine
	}

	return 0; // return succes
}