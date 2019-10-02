// Source file for scene class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#include "scene.h"

//Include core.h for scene saving/loading
#include "core.h"

//Include fstream and sstream for saving/loading
#include <fstream>
#include <sstream>

Scene::Scene() {
	this->activeCamera = nullptr; // Set active camera to nullptr
}

void Scene::Update() {
	this->UpdateChildren(); 
}

void Scene::SetActiveCamera(Camera* camera) {
	this->activeCamera = camera;
}

Camera* Scene::GetActiveCamera() {
	return this->activeCamera;
}

Scene::~Scene() {
	if (activeCamera) {
		delete activeCamera;
	}
}