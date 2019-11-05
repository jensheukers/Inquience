// Main entry point.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, November 2019
#include <core.h>
#include <component_register.h>

#include <luascript.h>
#include <scenemanager.h>

#include <input.h>

//Include components that we need to register
#include <source/components/healthcomponent.h>
#include <source/components/movementcomponent.h>

#include <source/creature.h>

int main(int argc, char * argv[]) {
	Core::Initialize(argc, argv, Vec2(1280, 720), "Inquience");

	Input::AddAxis("Horizontal", new Axis(KEYCODE_D, KEYCODE_A));

	LuaScript::RunFunction("res/scenes/default.scene", "Initialize");

	// We need to register our 'custom' components to the engine
	Component_Register::RegisterComponent<HealthComponent>();
	Component_Register::RegisterComponent<MovementComponent>();

	Creature* player = new Creature();
	player->localPosition = Vec2(100, 50);
	player->localScale = Vec2(32, 32);

	player->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/textures/placeholder.tga"));
	player->AddComponent<BoxCollider>()->outer = Vec2(32, 32);
	player->AddComponent<RigidBody>();
	player->AddComponent<MovementComponent>();
	player->GetComponent<MovementComponent>()->AddRaycasts(16);

	SceneManager::GetActiveScene()->AddChild(player);

	while (Core::IsActive()) {
		if (!player->GetComponent<MovementComponent>()->IsGrounded()) {
			player->GetComponent<RigidBody>()->AddForce(Vec2(0, 9.f));
		}

		if (Input::GetKeyDown(KEYCODE_SPACE)) {
			player->GetComponent<MovementComponent>()->Jump(100.f);
		}

		float speed = 200;
		switch ((int)Input::GetAxis("Horizontal")) {
		case -1:
			player->GetComponent<MovementComponent>()->StepLeft(speed);
			break;
		case 1:
			player->GetComponent<MovementComponent>()->StepRight(speed);
			break;
		default:
			player->GetComponent<MovementComponent>()->ResetHorizontalVelocity();
			break;
		}

		Core::Update(); // Handle updates for engine
	}

	return 0; // return succes
}