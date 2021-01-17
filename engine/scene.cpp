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

#include "unique_types.h"

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