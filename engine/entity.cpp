// Source file for entity class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, September 2019
#include "entity.h"
#include "core.h"
#include "editor.h"

//Include font.h to check if child is text when added
#include <iostream>

#include "luascript.h"
#include "unique_types.h"

void Entity::GenerateUniqueID() {
	static int currentUniqueId = 0;
	this->uniqueId = currentUniqueId;
	currentUniqueId++;
}

void Entity::HandleParentTransformations() {
	if (!this->parent) { // If we have no parent
		this->position = localPosition;
		this->scale = localScale;
	}
	else { // If we do have a parent, we want to include translations
		this->position = this->parent->position + localPosition;
		this->scale = this->parent->scale + localScale;
	}
}

void Entity::UpdateChildren() {
	for (Entity* child : children) {
		if (child->active) {
			if (!Editor::editorActive) {
				child->Update(); // Update child
				child->UpdateComponents(); // Update components
			}

			child->HandleParentTransformations(); // Handle the parent transformations
		}

		child->UpdateChildren(); // Update their children
	}
}

void Entity::UpdateComponents() {
	for (Component* component : components) {
		component->Update();
	}
}

LuaParsableLineVector Entity::GetChildLines(Entity* entity, unsigned tabs) {
	LuaParsableLineVector returnVector;


	returnVector.push_back(LuaParsableLine(LuaScriptFile::CreateLuaFunctionCallString("BeginEntity"), tabs));

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

Entity::Entity() {
	this->parent = nullptr;
	this->position = Vec2(0,0);
	this->scale = Vec2(0, 0);
	this->tag = "Entity";

	this->GenerateUniqueID();
	
	this->active = true;
}

Entity::Entity(const Entity& entity) {
	this->parent = nullptr;
	this->position = entity.position;
	this->localPosition = entity.localPosition;
	this->scale = entity.scale;
	this->localScale = entity.localScale;
	this->tag = entity.tag;

	this->GenerateUniqueID();

	//Copy components
	for (size_t i = 0; i < entity.components.size(); i++) {
		this->AddExistingComponentInstance(entity.components[i]->Copy());
	}

	//Copy children
	for (size_t i = 0; i < entity.children.size(); i++) {
		this->AddChild(new Entity(*entity.children[i]));
	}
}

void Entity::SetActive(bool state) {
	this->active = state;

	for each (Entity* child in children) {
		child->SetActive(state);
	}
}

bool Entity::Active() {
	return this->active;
}

Entity* Entity::AddChild(Entity* entity) {
	this->children.push_back(entity);
	entity->SetParent(this);
	return entity;
}

Entity* Entity::RemoveChild(int index) {
	for (int i = 0; i < (int)this->children.size(); i++) {
		if (i == index) {
			Entity* entity = this->children[i];
			this->children.erase(this->children.begin() + i);
			return entity;
		}
	}

	return nullptr;
}

Entity* Entity::RemoveChild(Entity* entity) {
	for (int i = 0; i < (int)this->children.size(); i++) {
		if (this->children[i] == entity) {
			Entity* entity = this->children[i];
			this->children.erase(this->children.begin() + i);
			return entity;
		}
	}

	return nullptr;
}

Entity* Entity::GetChild(int index) {
	for (int i = 0; i < (int)this->children.size(); i++) {
		if (i == index) {
			return this->children[i];
		}
	}
	return nullptr;
}

Entity* Entity::GetChild(Entity* entity) {
	for (int i = 0; i < (int)this->children.size(); i++) {
		if (this->children[i] == entity) {
			return this->children[i];
		}
	}
	return nullptr;
}

bool Entity::HasChild(Entity* entity) {
	for (int i = 0; i < (int)this->children.size(); i++) {
		if (children[i] == entity) {
			return true;
		}
	}
	return false;
}

Entity* Entity::GetParent() {
	return this->parent;
}

std::vector<Entity*> Entity::GetChildren() {
	return this->children;
}

void Entity::SetParent(Entity* entity) {
	this->parent = entity;
}

Vec2 Entity::GetPosition() {
	return this->position;
}

Vec2 Entity::GetScale() {
	return this->scale;
}

Entity* Entity::GetChildByTag(std::string tag) {
	if (this->tag == tag) return this;

	Entity* found = nullptr;
	for (size_t i = 0; i < children.size(); i++) {
		found = children[i]->GetChildByTag(tag);
		if (found) return found;
	}
	return found;
}

void Entity::MoveChildUp(Entity* child) {
	for (int i = children.size() - 1; i >= 0; i--) {
		if (children[i] == child && i - 1 != -1) {
			Entity* swap = children[i - 1];
			children[i - 1] = child;
			children[i] = swap;
		}
	}
}

void Entity::MoveChildDown(Entity* child) {
	for (int i = children.size() - 1; i >= 0; i--) {
		if (children[i] == child && i + 1 != children.size()) {
			Entity* swap = children[i + 1];
			children[i + 1] = child;
			children[i] = swap;
		}
	}
}

void Entity::WriteToLuaFile(LuaScriptFile& file, std::string funcName) {
	LuaParsableLineVector lines;

	LuaParsableLineVector childLines = GetChildLines(this, 1);
	lines.insert(lines.end(), childLines.begin(), childLines.end());

	file.ParseFunction(funcName, StringVector{}, lines);
}

Entity::~Entity() {
	for (size_t i = 0; i < (int)children.size(); i++) {
		delete children[i];
	}

	//Delete all components
	for (size_t i = 0; i < (int)components.size(); i++) {
		delete components[i];
	}
}