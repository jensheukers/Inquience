// Source file for scene class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, 2021
#include "scene.h"

Scene::Scene() {
	this->activeCamera = nullptr; // Set active camera to nullptr
	this->tag = "Scene";
}

void Scene::UpdateScene(bool gamePaused) {
	this->UpdateChildren(gamePaused); 

	//Update camera
	if (this->activeCamera && !gamePaused) {
		this->activeCamera->Update();
	}
}

void Scene::SetActiveCamera(Camera* camera) {
	this->activeCamera = camera;
}

Camera* Scene::GetActiveCamera() {
	return this->activeCamera;
}

void Scene::AddKVP(KeyValuePair kvp) {
	this->keyValuePairs.push_back(kvp);
}

void Scene::RemoveKVP(std::string key) {
	for (size_t i = 0; i < this->keyValuePairs.size(); i++) {
		if (this->keyValuePairs[i].key == key) {
			this->keyValuePairs.erase(this->keyValuePairs.begin() + i);
		}
	}
}

std::string Scene::GetKeyValue(std::string key) {
	for (size_t i = 0; i < this->keyValuePairs.size(); i++) {
		if (this->keyValuePairs[i].key == key) {
			return this->keyValuePairs[i].value;
		}
	}

	return "";
}

Scene::~Scene() {
	if (activeCamera) {
		delete activeCamera;
	}
}