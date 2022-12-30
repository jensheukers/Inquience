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

#include "parser.h"

SceneManager::SceneManager() {
	this->activeScene = nullptr;
}

Scene* SceneManager::SetActiveScene(Scene* scene) {
	this->activeScene = scene;
	return scene;
}

Scene* SceneManager::GetActiveScene() {
	return this->activeScene;
}

Scene* SceneManager::SwapScene(Scene* scene) {
	if (this->activeScene) {
		delete this->activeScene;
	}

	this->SetActiveScene(scene);

	Debug::Log("SceneManager : Swapped Scene");

	return this->GetActiveScene();
}

Scene* SceneManager::ReadFromFileAndSwap(std::string path) {
	Parser* parser = new Parser(path, true);
	Scene* scene = this->SwapScene(parser->ReadSceneFromFile());
	delete parser;
	return scene;
}

void SceneManager::Terminate() {
	if (GetActiveScene()) {
		delete GetActiveScene();
	};
}