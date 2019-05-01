// Source file for entity class.
//
// Version: 15/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, March 2019
#include "entity.h"
#include "core.h"
#include "renderer.h"

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
		child->UpdateComponents(); // Update components
	}
}

void Entity::UpdateComponents() {
	for (Component* component : components) {
		component->Update();
	}
}

Entity::Entity() {
	this->parent = nullptr; // Set parent to nullptr
}

Entity* Entity::AddChild(Entity* entity) {
	this->children.push_back(entity);
	entity->SetParent(this);

	//Register to renderer
	Core::GetRendererInstance()->RegisterEntity(entity);

	return entity;
}

Entity* Entity::RemoveChild(int index) {
	for (int i = 0; i < (int)this->children.size(); i++) {
		if (i == index) {
			Entity* entity = this->children[i];
			Core::GetRendererInstance()->RemoveEntity(entity);
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