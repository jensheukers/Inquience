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

#include "luascript.h"
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

void Scene::WriteToLuaFile(LuaScriptFile& file, std::string funcName) {
	LuaParsableLineVector lines;

	if (GetActiveCamera()) {
		lines.push_back(LuaParsableLine(LuaScriptFile::CreateLuaFunctionCallString("NewScene", StringVector{ "true" }), 1));
		lines.push_back(LuaParsableLine(LuaScriptFile::CreateLuaFunctionCallString("SetCameraPosition", 
						StringVector{ std::to_string(GetActiveCamera()->GetPosition().x), std::to_string(GetActiveCamera()->GetPosition().y) }), 1));
	}
	else {
		lines.push_back(LuaParsableLine(LuaScriptFile::CreateLuaFunctionCallString("NewScene", StringVector{ "false" }), 1));
	}



	for (size_t i = 0; i < GetChildren().size(); i++) {
		LuaParsableLineVector childLines = GetChildLines(GetChildren()[i], 1);
		lines.insert(lines.end(), childLines.begin(), childLines.end());
	}

	file.ParseFunction(funcName, StringVector{}, lines);
}

Scene::~Scene() {
	if (activeCamera) {
		delete activeCamera;
	}
}