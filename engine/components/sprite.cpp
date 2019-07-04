// Source file Sprite class.
//
// Version: 2/7/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, July 2019
#include "sprite.h"
#include "../debug.h"

UV::UV() {
	this->leftUp = Vec2(0, 0); // 0, 1
	this->rightUp = Vec2(1, 0); // 1, 1
	this->leftDown = Vec2(0, 1); // 0, 0
	this->rightDown = Vec2(1, 1); // 1, 0
}

Sprite::Sprite() {
	this->zIndex = 0;
	this->scale = Vec2(1, 1);
	this->slices = 1;
	this->texture = nullptr;
}

Texture* Sprite::SetTexture(Texture* texture) {
	this->texture = texture;
	return this->GetTexture();
}

Texture* Sprite::GetTexture() {
	return this->texture;
}

void Sprite::SetZIndex(int index) {
	this->zIndex = index;
}

int Sprite::GetZIndex() {
	return this->zIndex;
}

void Sprite::SetScale(Vec2 scale) {
	this->scale = scale;
}

Vec2 Sprite::GetScale() {
	return this->scale / (float)slices;
}