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

LuaParsableLineVector Scene::GetChildLines(Entity* entity, unsigned tabs) {
	LuaParsableLineVector returnVector;

	if (dynamic_cast<UIElement*>(entity)) {
		returnVector.push_back(LuaParsableLine(LuaScriptFile::CreateLuaFunctionCallString("BeginEntityUI"), tabs));
	}
	else {
		returnVector.push_back(LuaParsableLine(LuaScriptFile::CreateLuaFunctionCallString("BeginEntity"), tabs));
	}

	//Store position and scale and tag
	returnVector.push_back(LuaParsableLine(LuaScriptFile::CreateLuaFunctionCallString("SetPosition",
		StringVector{ std::to_string(entity->localPosition.x), std::to_string(entity->localPosition.y) }), tabs));
	returnVector.push_back(LuaParsableLine(LuaScriptFile::CreateLuaFunctionCallString("SetScale",
		StringVector{ std::to_string(entity->localScale.x), std::to_string(entity->localScale.y) }), tabs));
	returnVector.push_back(LuaParsableLine(LuaScriptFile::CreateLuaFunctionCallString("SetTag", StringVector{ "\"" + entity->tag + "\"" }), tabs));

	//Add components
	for (size_t i = 0; i < entity->GetComponents().size(); i++) {
		Component* component = entity->GetComponents()[i];
		returnVector.push_back(LuaParsableLine(LuaScriptFile::CreateLuaFunctionCallString("BeginComponent", StringVector{ "\"" + Essentials::Split(component->GetName(), ' ')[1] + "\"" }), tabs + 1));
		for (size_t ii = 0; ii < component->GetProperties().size(); ii++) {
			ComponentProperty* p = component->GetProperties()[ii];

			StringVector cb = p->getCallback();
			if (cb.size() == 0) continue;

			StringVector sv = { "\"" + p->key + "\"" };
			sv.insert(sv.end(), cb.begin(), cb.end());

			returnVector.push_back(LuaParsableLine(LuaScriptFile::CreateLuaFunctionCallString("SetProperty", sv), tabs + 2));
		}
		returnVector.push_back(LuaParsableLine(LuaScriptFile::CreateLuaFunctionCallString("EndComponent"), tabs + 1));
	}

	for (size_t i = 0; i < entity->GetChildren().size(); i++) {
		LuaParsableLineVector childReturnVector = GetChildLines(entity->GetChildren()[i], tabs + 1);
		returnVector.insert(returnVector.end(), childReturnVector.begin(), childReturnVector.end());
	}

	returnVector.push_back(LuaParsableLine(LuaScriptFile::CreateLuaFunctionCallString("EndEntity"), tabs));
	return returnVector;
}

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

int Scene::WriteToLuaFile(LuaScriptFile& file, std::string funcName) {
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
	return 0;
}

Scene::~Scene() {
	if (activeCamera) {
		delete activeCamera;
	}
}