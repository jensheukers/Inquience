// Source file for SceneManager class.
// SceneManager is a singleton instance
//
// Version: 4/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#include "scenemanager.h"

SceneManager* SceneManager::instance; // Declare instance

SceneManager* SceneManager::GetInstance() {
	if (!instance) {
		instance = new SceneManager();
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

void SceneManager::Terminate() {
	if (GetActiveScene()) {
		delete GetActiveScene();
	}
	delete GetInstance();
}