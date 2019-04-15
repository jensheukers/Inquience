// Source file for vec2 class.
// Version: 6/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, April 2019

#include "vec2.h"
#include <math.h>

float Vec2::Distance(Vec2 other) {
	float dx = other.x - this->x;
	float dy = other.y - this->y;
	return sqrt(dx * dx + dy * dy);
}

float Vec2::Distance(Vec2 a, Vec2 b) {
	float dx = b.x - a.x;
	float dy = b.y - a.y;
	return sqrt(dx * dx + dy * dy);
}

Vec2 Vec2::Lerp(Vec2 a, Vec2 b, float amount) {
	return b * amount + a * (1.f - amount);
}