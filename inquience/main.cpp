// Main entry point.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, November 2019
#include <core.h>
#include <luascript.h>
#include <scenemanager.h>
#include <input.h>

#include <component_register.h>

//Include components that we need to register
#include <source/components/healthcomponent.h>

//Game includes
#include <source/creature.h>

int main(int argc, char * argv[]) {
	Core::Initialize(argc, argv, Vec2(1280, 720), "Inquience");

	//Add axises
	Input::AddAxis("Horizontal", new Axis(KEYCODE_D, KEYCODE_A));
	Input::AddAxis("Vertical", new Axis(KEYCODE_S, KEYCODE_W));

	// We need to register our 'custom' components to the engine
	Component_Register::RegisterComponent<HealthComponent>();

	//Load default world for testing
	LuaScript::RunFunction("res/scenes/default.scene", "Initialize");

	Creature* creature = new Creature();
	creature->localScale = Vec2(32, 32);
	creature->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/textures/placeholder.tga"));
	creature->AddComponent<BoxCollider>();
	creature->GetComponent<BoxCollider>()->outer = Vec2(32, 32);
	creature->GetComponent<BoxCollider>()->bDrawDebugLines = true;

	creature->AddComponent<RigidBody>();

	SceneManager::GetActiveScene()->AddChild(creature);

	while (Core::IsActive()) {
		Core::Update(); // Handle updates for engine
		creature->localPosition = creature->localPosition + Vec2(Input::GetAxis("Horizontal"), Input::GetAxis("Vertical"));
	}

	return 0; // return succes
}