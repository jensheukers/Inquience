// Source file Sprite class.
//
// Version: 1/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#include "sprite.h"
#include "../debug.h"

SpriteUV::SpriteUV() {
	this->leftUp = Vec2(0, 0); // 0, 1
	this->rightUp = Vec2(1, 0); // 1, 1
	this->leftDown = Vec2(0, 1); // 0, 0
	this->rightDown = Vec2(1, 1); // 1, 0
}

SpriteUV::SpriteUV(Sprite* sprite, int amount, int index) {
	//Check if width and height equal, if not call default constructor, and return
	if (sprite->GetTexture()->textureData->width != sprite->GetTexture()->textureData->height) {
		Debug::Log("SpriteUV: Texture width does not equal texture height, calling default constructor...");
		SpriteUV();
		return;
	}

	//Iterate through image
	int _index = 0;
	for (int y = 0; y < (int)sprite->GetTexture()->textureData->height; y += amount) {
		for (int x = 0; x < (int)sprite->GetTexture()->textureData->width; x += amount) {
			if (_index == index) {
				//Normalize positions
				this->leftUp = Vec2((float)x / sprite->GetTexture()->textureData->width, (float)y / sprite->GetTexture()->textureData->height);
				this->rightUp = Vec2((float)x / sprite->GetTexture()->textureData->width + (float) amount / sprite->GetTexture()->textureData->width, (float)y / sprite->GetTexture()->textureData->height);
				this->leftDown = Vec2((float)x / sprite->GetTexture()->textureData->width, (float)y / sprite->GetTexture()->textureData->height + (float)amount / sprite->GetTexture()->textureData->height);
				this->rightDown = Vec2((float)x / sprite->GetTexture()->textureData->width + (float)amount / sprite->GetTexture()->textureData->width, (float)y / sprite->GetTexture()->textureData->height + (float)amount / sprite->GetTexture()->textureData->height);
				sprite->SetSplits(sprite->GetTexture()->textureData->width / amount);
				return;
			}
			_index++;
		}
	}

	//Set splits to 1 because there is 1 image in the "row"
	sprite->SetSplits(1);
}

Sprite::Sprite() {
	this->scale = Vec2(1, 1);
	this->splits = 1;
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
	return this->scale;
}

int Sprite::GetSplits() {
	return this->splits;
}

void Sprite::SetSplits(int splits) {
	this->splits = splits;
}