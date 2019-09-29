// Source file for scene class.
//
// Version: 23/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, March 2019
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

void Scene::Load(char* filePath) {
	std::string line;
	std::ifstream file = std::ifstream(Core::GetExecutableDirectoryPath().append(filePath));

	if (file.is_open()) {
		Entity* currentEntity; // The current entity being processed
		while (std::getline(file, line))
		{
			if (line == " ") continue;
			if (line == "#ENTITY") {
				currentEntity = new Entity();
				AddChild(currentEntity);
				continue;
			}

			//Split
			std::stringstream ss(line);
			std::string segment;
			std::vector<std::string> segments;
			while (std::getline(ss, segment, '=')) {
				segments.push_back(segment);
			}

			if (currentEntity) {
				// Split at commas
				std::stringstream ss2(segments[1]);
				std::string comma;
				std::vector<std::string> commas;
				while (std::getline(ss2, comma, ',')) {
					commas.push_back(comma);
				}

				if (segments[0] == "position") {
					currentEntity->localPosition = Vec2(std::stof(commas[0]), std::stof(commas[1]));
					continue;
				}

				if (segments[0] == "scale") {
					currentEntity->localScale = Vec2(std::stof(commas[0]), std::stof(commas[1]));
					continue;
				}

				if (segments[0] == "sprite") {
					currentEntity->AddComponent<Sprite>();

					//Texture
					currentEntity->GetComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga((char*)commas[0].c_str()));

					//Uv data
					// Split at ampersand
					std::stringstream ss3(commas[1]);
					std::string ampersand;
					std::vector<std::string> ampersands;
					while (std::getline(ss3, ampersand, '&')) {
						ampersands.push_back(ampersand);
					}

					UV uvData = UV();
					uvData.leftUp.x = std::stof(ampersands[0]);
					uvData.leftUp.y = std::stof(ampersands[1]);
					uvData.rightUp.x = std::stof(ampersands[2]);
					uvData.rightUp.y = std::stof(ampersands[3]);
					uvData.leftDown.x = std::stof(ampersands[4]);
					uvData.leftDown.y = std::stof(ampersands[5]);
					uvData.rightDown.x = std::stof(ampersands[6]);
					uvData.rightDown.y = std::stof(ampersands[7]);
					currentEntity->GetComponent<Sprite>()->uv = uvData;
				}
			}
		}
		file.close();

		Debug::Log("Scene " + std::string(filePath) + " Loaded!");
	}
}

void Scene::Save(char* filePath) {
	std::ofstream file;
	file.open(Core::GetExecutableDirectoryPath().append(filePath));
	
	for (int i = 0; i < (int)GetChildren().size(); i++) {
		file << "#ENTITY\n";
		file << "position=" << GetChild(i)->GetPosition().x << "," << GetChild(i)->GetPosition().y << "\n";
		file << "scale=" << GetChild(i)->localScale.x << "," << GetChild(i)->localScale.y << "\n";
		
		if (GetChild(i)->HasComponent<Sprite>() && GetChild(i)->GetComponent<Sprite>()->GetTexture()) {
			file << "sprite=" << GetChild(i)->GetComponent<Sprite>()->GetTexture()->path << ",";

			UV uvData = GetChild(i)->GetComponent<Sprite>()->uv;
			file << uvData.leftUp.x << "&" << uvData.leftUp.y << "&";
			file << uvData.rightUp.x << "&" << uvData.rightUp.y << "&";
			file << uvData.leftDown.x << "&" << uvData.leftDown.y << "&";
			file << uvData.rightDown.x << "&" << uvData.rightDown.y << ",";

			file << "\n";	
		}
	}

	file.close();
	Debug::Log("Scene " + std::string(filePath) + " Saved!");
}