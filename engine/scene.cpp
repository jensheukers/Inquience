// Source file for scene class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, 2021
#include "scene.h"

//Include core.h for scene saving/loading
#include "core.h"

//Include fstream and sstream for saving/loading
#include <fstream>
#include <sstream>
#include <iomanip>

#include "unique_types.h"
#include "parser.h"

//Include json
#include "json.hpp"

#include "component_register.h"

Scene::Scene() {
	this->activeCamera = nullptr; // Set active camera to nullptr
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

void Scene::WriteToJsonFile(std::string destination) {
	nlohmann::json jsonData;

	//Kvps
	nlohmann::json kvpJsonArray = nlohmann::json::array();
	for (KeyValuePair kvp : this->keyValuePairs) {
		nlohmann::json kvpJsonObject;
		kvpJsonObject["keyname"] = kvp.key;
		kvpJsonObject["value"] = kvp.value;
		kvpJsonArray.push_back(kvpJsonObject);
	}

	nlohmann::json entityJsonArray = nlohmann::json::array();

	for (Entity* e : this->GetChildren()) {
		nlohmann::json entityJsonObject;

		entityJsonObject["tag"] = e->tag;
		entityJsonObject["position"] = { e->localPosition.x, e->localPosition.y };
		entityJsonObject["scale"] = { e->localScale.x, e->localScale.y };

		//Components array
		nlohmann::json componentsJsonArray = nlohmann::json::array();
		for (Component* c : e->GetComponents()) {
			nlohmann::json componentJsonObject;

			componentJsonObject["name"] = c->GetName();

			//Property array
			nlohmann::json propertyJsonArray = nlohmann::json::array();
			for (ComponentProperty* p : c->GetProperties()) {
				nlohmann::json propertyJsonObject;

				StringVector valueVector = p->getCallback();
				std::string value;

				for (size_t i = 0; i < valueVector.size(); i++) {
					value += valueVector[i];

					if (i + 1 != valueVector.size()) {
						value += ",";
					}
				}

				propertyJsonObject[p->key] = value;
				propertyJsonArray.push_back(propertyJsonObject);
			}
			

			componentJsonObject["properties"] = propertyJsonArray;

			componentsJsonArray.push_back(componentJsonObject);
		}
		entityJsonObject["components"] = componentsJsonArray;

		entityJsonArray.push_back(entityJsonObject);
	}

	jsonData["entities"] = entityJsonArray;
	jsonData["kvps"] = kvpJsonArray;

	jsonData["camerapos"] = { this->activeCamera->GetPosition().x, this->activeCamera->GetPosition().y };

	std::ofstream o(Core::GetExecutableDirectoryPath() + destination);
	o << std::setw(4) << jsonData << std::endl;
}

void Scene::ReadFromJsonFile(std::string path) {
	Parser* parser = new Parser(Core::GetExecutableDirectoryPath() + path, true, true);

	nlohmann::json jsonData = nlohmann::json::parse(parser->GetFile());

	this->SetActiveCamera(new Camera());
	this->GetActiveCamera()->position = Vec2(jsonData["camerapos"][0], jsonData["camerapos"][1]);

	for (auto& kvps : jsonData["kvps"]) {
		this->AddKVP(KeyValuePair(kvps["keyname"], kvps["value"]));
	}

	for (auto& e : jsonData["entities"]) {
		Entity* entity = new Entity();
		entity->tag = e["tag"];
		entity->localPosition = Vec2(e["position"][0], e["position"][1]);
		entity->localScale = Vec2(e["scale"][0], e["scale"][1]);

		for (auto& c : e["components"]) {
			Component* component = Component_Register::GetNewComponentInstance(c["name"]);

			for (auto& p : c["properties"]) {
				for (auto& i : p.items()) {
					std::string value = i.value();
					if (value == "") continue;

					if (value.find(',') != std::string::npos) {
						std::string segment;
						StringVector strv;
						std::stringstream ss(value);

						//SPLIT AT COMMA
						while (std::getline(ss, segment, ',')) {
							strv.push_back(segment);
						}

						component->SetProperty(i.key(), strv);
					}
					else {
						component->SetProperty(i.key(), StringVector{ i.value() });
					}
				}
			}

			entity->AddExistingComponentInstance(component);
		}

		this->AddChild(entity);
	}

	delete parser;
}

Scene::~Scene() {
	if (activeCamera) {
		delete activeCamera;
	}
}