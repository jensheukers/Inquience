// Main entry point.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, April 2020
#include <core.h>
#include <parser.h>
#include <components/animator.h>

#include "source/gamestate.h"

#include "../engine/component_register.h"

int main(int argc, char * argv[]) {
	Core::Initialize(argc, argv, Vec2(1280, 720), "Inquience");

	//Create GameState and insert levels as parameter
	static GameState* gameState = new GameState(std::vector<std::string> { 
		"res/scenes/mainmenu.json",
		"res/scenes/test_world.json"
	});
	gameState->Initialize();

	//Native Luascript functions (Game)
	{
			Core::GetLuaScript()->AddNativeFunction("NextLevel_Internal", [](lua_State* state) -> int {
			Core::LateFrameExecute([=]() {
				gameState->NextLevel();
			});
			return 0;
		});
	}

	while (Core::IsActive()) {
		Core::Update(); // Handle updates for engine
	}

	return 0; // return succes
}