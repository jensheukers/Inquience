// Main entry point.
//
// Version: 8/8/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, August 2019
#include "../engine/core.h"
#include "../engine/input.h"

#include "../engine/scenemanager.h"

//Define default tilemap scale
#define DEFAULT_TILEMAP_TILE_SCALE 32 

int main(int argc, char * argv[]) {
	Core::Initialize(argc, argv);

	Entity* entity = new Entity();
	entity->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/placeholder.tga"));

	Scene* scene = new Scene();
	scene->SetActiveCamera(new Camera());

	SceneManager::SetActiveScene(scene);

	scene->AddChild(entity);

	while (Core::IsActive()) {
		Core::Update(); // Handle updates for engine
	}

	return 0; // return succes
}