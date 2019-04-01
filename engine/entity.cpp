// Source file for entity class.
//
// Version: 28/3/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, March 2019
#include "entity.h"

void Entity::HandleParentTransformations() {
	if (!this->parent) { // If we have no parent
		this->position = localPosition;
		this->rotation = localRotation;
		this->scale = localScale;
	}
	else { // If we do have a parent, we want to include translations
		this->position = this->parent->position + localPosition;
		this->rotation = this->parent->rotation + localRotation;
		this->scale = this->parent->scale + localScale;
	}
}

void Entity::UpdateChildren() {
	for (Entity* child : children) {
		child->HandleParentTransformations(); // Handle the parent transformations
		child->UpdateChildren(); // Update their children
		child->Update(); // Update child
	}
}

Entity::Entity() {
	this->parent = nullptr; // Set parent to nullptr
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
}

Entity* Entity::GetChild(int index) {
	for (int i = 0; i < (int)this->children.size(); i++) {
		if (i == index) {
			return this->children[i];
		}
	}
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