// Source file for vec2 class.
// Version: 6/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, April 2019

#include "vec2.h"

float Vec2::Distance(Vec2 other) {
	return other.x - this->x + other.y + this->y;
}

float Vec2::Distance(Vec2 a, Vec2 b) {
	return b.x - a.x + b.y + a.y;
}