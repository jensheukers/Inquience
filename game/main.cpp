// Main entry point.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, September 2019
#include "../engine/core.h"
#include "../engine/input.h"

#include "../engine/scenemanager.h"

#include "../engine/luascript.h"

#include "../engine/components/collider.h"
#include "../engine/components/rigidbody.h"

int main(int argc, char * argv[]) {
	Core::Initialize(argc, argv);

	SceneManager::SetActiveScene(new Scene());
	SceneManager::GetActiveScene()->SetActiveCamera(new Camera());

	//LuaScript::RunFunction("res/example.lua", "Example");

	Entity* entity = new Entity();
	entity->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/placeholder.tga"));
	entity->localScale = Vec2(32, 32);
	
	entity->AddComponent<BoxCollider>();
	entity->GetComponent<BoxCollider>()->outer = entity->localScale;

	entity->GetComponent<BoxCollider>()->onCollisionEnter.AddLambda([]() {
		Debug::Log("Hit");
	});

	Entity* entity2 = new Entity();
	entity2->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/placeholder.tga"));
	entity2->localScale = Vec2(32, 32);

	entity2->AddComponent<BoxCollider>();
	entity2->GetComponent<BoxCollider>()->outer = entity->localScale;

	entity2->GetComponent<BoxCollider>()->onCollisionEnter.AddLambda([]() {
		Debug::Log("Hit2");
	});

	SceneManager::GetActiveScene()->AddChild(entity);
	SceneManager::GetActiveScene()->AddChild(entity2);


	float speed = 100.f;

	entity->localPosition = Vec2(300, 10);
	entity->AddComponent<RigidBody>();

	entity->GetComponent<RigidBody>()->AddForce(Vec2(-100, 0));

	while (Core::IsActive()) {
		Core::Update(); // Handle updates for engine
	}

	return 0; // return succes
}