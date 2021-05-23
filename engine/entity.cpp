// Source file for entity class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, September 2019
#include "entity.h"
#include "core.h"

//Include font.h to check if child is text when added
#include <iostream>

#include "unique_types.h"

void Entity::GenerateUniqueID() {
	static int currentUniqueId = 0;
	this->uniqueId = currentUniqueId;
	currentUniqueId++;
}

void Entity::HandleParentTransformations() {
	if (this->parent) {
		this->globalPosition = this->position + this->parent->GetGlobalPosition();
		this->globalScale = this->scale + this->parent->GetGlobalScale();
	}
}

void Entity::UpdateChildren(bool gamePaused) {
	for (Entity* child : children) {
		if (child->active) {
			if (!gamePaused) {
				child->Update(); // Update child
				child->UpdateComponents(); // Update components
			}
			child->HandleParentTransformations(); // Handle the parent transformations
		}

		child->UpdateChildren(gamePaused); // Update their children
	}
}

void Entity::UpdateComponents() {
	for (Component* component : components) {
		component->Update();
	}

	for (Component* component : components) {
		component->LateUpdate();
	}
}

Entity::Entity() {
	this->parent = nullptr;
	this->position = Vec2(0,0);
	this->scale = Vec2(0, 0);
	this->tag = "Entity";
	this->isPrefab = false;
	this->prefabFilePath = "";

	this->GenerateUniqueID();
	
	this->active = true;
}

Entity::Entity(const Entity& entity) {
	this->parent = nullptr;
	this->position = entity.position;
	this->scale = entity.scale;
	this->tag = entity.tag;
	this->isPrefab = entity.isPrefab;
	this->prefabFilePath = entity.prefabFilePath;

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

	for (Entity* child : children) {
		child->SetActive(state);
	}
}

bool Entity::Active() {
	return this->active;
}

Entity* Entity::AddChild(Entity* entity) {
	this->children.push_back(entity);
	entity->SetParent(this);

	//Make sure to instantly set correct transformations
	entity->HandleParentTransformations();
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

void Entity::FindEntitiesNearPosition(std::vector<Entity*>& vectorRef, Vec2 position, float distance) {
	if (Vec2::Distance(position, this->position) <= distance) {
		vectorRef.push_back(this);
	}

	for (size_t i = 0; i < children.size(); i++) {
		children[i]->FindEntitiesNearPosition(vectorRef, position, distance);
	}
}

Entity* Entity::GetHighestEntityInHierarchy() {
	if (!this->GetParent()) {
		return this;
	}
	else {
		return this->GetParent()->GetHighestEntityInHierarchy();
	}
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