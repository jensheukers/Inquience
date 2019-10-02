// Source file for component class.
//
// Version: 5/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#include "component.h"
#include "debug.h"

void Component::AddProperty(std::string key, std::function<void(std::vector<std::string>)> value) {
	properties[key] = value;
}

void Component::SetProperty(std::string key, std::vector<std::string> args) {
	if (!properties[key]) {
		Debug::Log(std::string("\'") + typeid(*this).name() + std::string("\'") + std::string(" does not have a property named: ") + key);
		return;
	}

	properties[key](args);
}

Entity* Component::SetOwner(Entity* entity) {
	this->owner = entity;
	return entity;
}

Entity* Component::GetOwner() {
	return this->owner;
}