// Source file for scene class.
//
// Version: 23/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, March 2019
#include "scene.h"

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