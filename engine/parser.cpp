// Header file for scene class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, March 2021
#include "parser.h"
#include "debug.h"

#include "scene.h"
#include "entity.h"
#include "component_register.h"

//Include fstream and sstream for saving/loading
#include <fstream>
#include <sstream>
#include <iomanip>

//Include core.h for scene saving/loading
#include "core.h"

#include <sstream>

void Parser::BufferEntityToJsonArray(Entity* e, nlohmann::json& _array) {
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

	nlohmann::json entityJsonArray = nlohmann::json::array();
	for (size_t i = 0; i < e->GetChildren().size(); i++) {
		BufferEntityToJsonArray(e->GetChild(i), entityJsonArray);
	}

	entityJsonObject["entities"] = entityJsonArray;

	_array.push_back(entityJsonObject);
}

Entity* Parser::ReadEntityFromJsonData(nlohmann::json& jsonData) {
	Entity* entity = new Entity();
	entity->tag = jsonData["tag"];
	entity->localPosition = Vec2(jsonData["position"][0], jsonData["position"][1]);
	entity->localScale = Vec2(jsonData["scale"][0], jsonData["scale"][1]);

	for (nlohmann::json& c : jsonData["components"]) {
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

	for (auto& e : jsonData["entities"]) {
		entity->AddChild(ReadEntityFromJsonData(e));
	}

	return entity;
}

Parser::Parser(std::string destination, bool read, bool debug) {
	this->destination = Core::GetExecutableDirectoryPath() + destination;
	this->_file = std::fstream(this->destination);
	this->_read = read;
	this->debug = debug;

	//Create file if not exist
	if (!this->_file.good()) {
		if (!read) {
			std::ofstream __file(this->destination);
			__file.close();

			this->_file = std::fstream(this->destination);
		}
		else {
			Debug::Log("Parser : File " + this->destination + " does not exist");
		}
	}
}

bool Parser::fileIsOpen() {
	return this->_file.is_open();
}

std::string Parser::ReadLine() {
	if (!this->_read) {
		Debug::Log("Parser : File is write only");
		return "";
	}

	if (this->fileIsOpen()) {
		std::getline(this->_file, this->_curLine);

		Debug::Log("Parser [READ] : " + this->_curLine);
		return this->_curLine;
	}
	Debug::Log("Parser : File is not open");
	return "";
}

void Parser::WriteLine(std::string line) {
	if (this->_read) {
		Debug::Log("Parser : File is read only");
		return;
	}

	if (this->fileIsOpen()) {
		this->_file << line + "\n";

		Debug::Log("Parser [WRITE] : " + line);
	}
	else {
		Debug::Log("Parser : File is not open");
	}
}

std::vector<std::string> Parser::SplitLine(std::string line, char c) {
	std::stringstream ss(line);
	std::string segment;
	std::vector<std::string> segments;

	while (std::getline(ss, segment, c)) {
		segments.push_back(segment);
	}
	return segments;
}

std::vector<std::string> Parser::SplitLine(char c) {
	return this->SplitLine(this->_curLine, c);
}

std::fstream& Parser::GetFile() {
	return this->_file;
}

void Parser::WriteSceneToFile(Scene* scene) {
	nlohmann::json jsonData;

	//Kvps
	nlohmann::json kvpJsonArray = nlohmann::json::array();
	for (KeyValuePair kvp : scene->GetKeyValuePairList()) {
		nlohmann::json kvpJsonObject;
		kvpJsonObject["keyname"] = kvp.key;
		kvpJsonObject["value"] = kvp.value;
		kvpJsonArray.push_back(kvpJsonObject);
	}

	nlohmann::json entityJsonArray = nlohmann::json::array();

	for (Entity* e : scene->GetChildren()) {
		BufferEntityToJsonArray(e, entityJsonArray);
	}

	jsonData["entities"] = entityJsonArray;
	jsonData["kvps"] = kvpJsonArray;

	jsonData["camerapos"] = { scene->GetActiveCamera()->GetPosition().x, scene->GetActiveCamera()->GetPosition().y };

	std::ofstream o(destination + FILETYPE_SCENE);
	o << std::setw(4) << jsonData << std::endl;
}

Scene* Parser::ReadSceneFromFile() {
	nlohmann::json jsonData = nlohmann::json::parse(this->GetFile());

	Scene* scene = new Scene();

	scene->SetActiveCamera(new Camera());
	scene->GetActiveCamera()->position = Vec2(jsonData["camerapos"][0], jsonData["camerapos"][1]);

	for (auto& kvps : jsonData["kvps"]) {
		scene->AddKVP(KeyValuePair(kvps["keyname"], kvps["value"]));
	}

	for (auto& e : jsonData["entities"]) {
		scene->AddChild(ReadEntityFromJsonData(e));
	}

	return scene;
}

void Parser::WritePrefabToFile(Entity* entity) {
	nlohmann::json jsonData;

	nlohmann::json entityJsonArray = nlohmann::json::array();
	BufferEntityToJsonArray(entity, entityJsonArray);

	jsonData["prefab"] = entityJsonArray;

	std::ofstream o(destination + FILETYPE_PREFAB);
	o << std::setw(4) << jsonData << std::endl;
}

Entity* Parser::ReadPrefabFromFile() {
	nlohmann::json jsonData = nlohmann::json::parse(this->GetFile());

	Entity* entity = new Entity();
	for (auto& e : jsonData["prefab"]) {
		entity->AddChild(ReadEntityFromJsonData(e));
	}

	return entity;
}

Parser::~Parser() {
	if (fileIsOpen()) {
		this->_file.close();
	}
}