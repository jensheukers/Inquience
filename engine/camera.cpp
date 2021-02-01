// Source file for vec2 class.
//
// Version: 23/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, 2021
#include "camera.h"
#include "entity.h"
#include "core.h"


Camera::Camera() {
	this->target = nullptr;
}

void Camera::Update() {
	if (this->target) {
		Vec2 center = this->target->GetPosition() + this->target->GetScale();

		this->position = center - Vec2(Core::GetRendererInstance()->GetResolution().x / 2, Core::GetRendererInstance()->GetResolution().y / 2);
	}
}

void Camera::SetTarget(Entity* target) {
	this->target = target;
}

Vec2 Camera::GetPosition() {
	return this->position;
}