// Source file for vec2 class.
//
// Version: 2/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019

#include "vec2.h"
#include <math.h>

float Vec2::Distance(Vec2 other) {
	float dx = other.x - this->x;
	float dy = other.y - this->y;
	return (float)sqrt(dx * dx + dy * dy);
}

Vec2 Vec2::Direction(Vec2 other) {
	return Vec2(other.x - this->x, other.y - this->y);
}

Vec2 Vec2::DirectionNormalized(Vec2 other) {
	return Vec2(other.x - this->x, other.y - this->y) / Vec2::Distance(*this, other);
}

float Vec2::Angle(Vec2 other) {
	return (float)atan2(this->y - other.y, this->x - other.x);
}

float Vec2::Magnitude() {
	return (float)sqrt(this->x * this->x + this->y * this->y);
}

float Vec2::Distance(Vec2 a, Vec2 b) {
	float dx = b.x - a.x;
	float dy = b.y - a.y;
	return (float)sqrt(dx * dx + dy * dy);
}

Vec2 Vec2::Lerp(Vec2 a, Vec2 b, float amount) {
	return b * amount + a * (1.f - amount);
}

float Vec2::RadToDeg(float radians) {
	return (float)(radians * 180 / PI) + 180;
}

float Vec2::DegToRad(float degrees) {
	return (float)(degrees * PI / 180);
}