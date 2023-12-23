// Source file for component class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#include "managed.h"
#include "component.h"
#include "debug.h"

//Include imgui for editor purpose
#include "imgui.h"

Component::Component() {
	this->owner = nullptr;
}

size_t Component::GetPropertyIndex(std::string key) {
	for (size_t i = 0; i < properties.size(); i++) {
		if (properties[i]->key == key) {
			return i;
		}
	}
	return -1;
}

void Component::AddProperty(std::string key, PropertySetCallback setCallback, PropertyGetCallback getCallback) {
	ComponentProperty* p = new ComponentProperty();
	p->key = key;
	p->setCallback = setCallback;
	p->getCallback = getCallback;
	properties.push_back(p);
}

void Component::SetProperty(std::string key, StringVector args) {
	if (GetPropertyIndex(key) == -1) {
		Debug::Log(std::string("\'") + typeid(*this).name() + std::string("\'") + std::string(" does not have a property named: ") + key);
		return;
	}

	properties[GetPropertyIndex(key)]->setCallback(args);
}

StringVector Component::GetProperty(std::string key) {
	if (GetPropertyIndex(key) == -1) {
		Debug::Log(std::string("\'") + typeid(*this).name() + std::string("\'") + std::string(" does not have a property named: ") + key);
		return StringVector();
	}

	return properties[GetPropertyIndex(key)]->getCallback();
}

Entity* Component::SetOwner(Entity* entity) {
	this->owner = entity;
	return entity;
}

Entity* Component::GetOwner() {
	return this->owner;
}

void Component::ShowComponentProperties()
{
	ImGui::Text("Component properties have not been derrived!");
}