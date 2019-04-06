// Source file Sprite class.
//
// Version: 6/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, April 2019
#include "sprite.h"

Sprite::Sprite() {
	this->scale = Vec2(1, 1);
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
	return this->scale;
}