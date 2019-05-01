// Source file for vec2 class.
//
// Version: 23/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#include "camera.h"

void Camera::SetPosition(Vec2 position) {
	this->position = position;
}

Vec2 Camera::GetPosition() {
	return this->position;
}