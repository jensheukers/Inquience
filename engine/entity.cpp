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

Entity::Entity() {
	this->parent = nullptr; // Set parent to nullptr
	this->position = Vec2(0,0);
	this->scale = Vec2(0, 0);
	this->tag = "Entity";

	//Set id
	static int currentUniqueId = 0;
	this->uniqueId = currentUniqueId;
	currentUniqueId++;

	//Set entity active
	this->active = true;
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

Entity::~Entity() {
	for (size_t i = 0; i < (int)children.size(); i++) {
		delete children[i];
	}

	//Delete all components
	for (size_t i = 0; i < (int)components.size(); i++) {
		delete components[i];
	}
}