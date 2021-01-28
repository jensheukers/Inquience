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

	for (Entity* e : this->GetChildren()) {
		jsonData[std::to_string(e->uniqueId)]["tag"] = e->tag;
		jsonData[std::to_string(e->uniqueId)]["position"] = { e->localPosition.x, e->localPosition.y };
		jsonData[std::to_string(e->uniqueId)]["scale"] = { e->localScale.x, e->localScale.y };

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
				
				Debug::Log(value);

				propertyJsonObject[p->key] = value;
				propertyJsonArray.push_back(propertyJsonObject);
			}
			

			componentJsonObject["properties"] = propertyJsonArray;

			componentsJsonArray.push_back(componentJsonObject);
		}

		jsonData[std::to_string(e->uniqueId)]["components"] = componentsJsonArray;
	}

	std::ofstream o(Core::GetExecutableDirectoryPath() + destination);
	o << std::setw(4) << jsonData << std::endl;
}

void Scene::ReadFromJsonFile(std::string path) {
	Parser* parser = new Parser(Core::GetExecutableDirectoryPath() + path, true, true);

	nlohmann::json jsonData = nlohmann::json::parse(parser->GetFile());
	for (auto& e : jsonData) {
		Entity* entity = new Entity();
		entity->tag = e["tag"];
		entity->localPosition = Vec2(e["position"][0], e["position"][1]);
		entity->localScale = Vec2(e["scale"][0], e["scale"][1]);

		for (auto& c : e["components"]) {
			Component* component = Component_Register::GetNewComponentInstance(c["name"]);

			for (auto& p : c["properties"]) {
				for (auto& i : p.items()) {
					std::string value = i.value();

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