// Source file for SceneManager class.
// SceneManager is a singleton instance
//
// Version: 4/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, 2021
#include "scenemanager.h"
#include <debug.h>


SceneManager* SceneManager::instance; // Declare instance

SceneManager* SceneManager::GetInstance() {
	if (!instance) {
		instance = new SceneManager();

		Debug::Log("SceneManager : Instance Created");
	}
	//Return instance
	return instance;
}

Scene* SceneManager::SetActiveScene(Scene* scene) {
	GetInstance()->activeScene = scene;
	return scene;
}

Scene* SceneManager::GetActiveScene() {
	return GetInstance()->activeScene;
}

Scene* SceneManager::SwapScene(Scene* scene) {
	if (SceneManager::GetInstance()->activeScene) {
		delete SceneManager::GetInstance()->activeScene;
	}

	SceneManager::GetInstance()->SetActiveScene(scene);

	Debug::Log("SceneManager : Swapped Scene");

	return SceneManager::GetActiveScene();
}

void SceneManager::Terminate() {
	if (GetActiveScene()) {
		delete GetActiveScene();
	}
	delete GetInstance();
}