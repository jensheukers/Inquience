// Source file for component class.
//
// Version: 5/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#include "component.h"

Entity* Component::SetOwner(Entity* entity) {
	this->owner = entity;
	return entity;
}

Entity* Component::GetOwner() {
	return this->owner;
}