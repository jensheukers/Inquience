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

//Include core so we can get renderer instance
#include "../core.h"

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

	//Register to renderer
	Core::GetRendererInstance()->RegisterSprite(this);
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

Vec2 Sprite::GetCenter() {
	return ((Vec2((float)texture->textureData->width, (float)texture->textureData->height) * scale) / (float)slices) / 2;
}

UV Sprite::Split(int pixelsPerTile, int index) {
	if (!this->texture) {
		Debug::Log("Cannot split sprite - No texture present");
		return uv;
	}

	if (this->texture->textureData->width != this->texture->textureData->height) {
		Debug::Log("Cannot split sprite - Texture width != height");
		return uv;
	}

	int _cur = 0;
	for (int y = 0; y < (int)texture->textureData->height; y += pixelsPerTile) {
		for (int x = 0; x < (int)texture->textureData->width; x += pixelsPerTile) {
			if (_cur == index) {
				uv.leftUp = Vec2((float)x / texture->textureData->width, (float)y / texture->textureData->height);
				uv.rightUp = Vec2((float)x / texture->textureData->width + (float)pixelsPerTile / texture->textureData->width, (float)y / texture->textureData->height);
				uv.leftDown = Vec2((float)x / texture->textureData->width, (float)y / texture->textureData->height + (float)pixelsPerTile / texture->textureData->height);
				uv.rightDown = Vec2((float)x / texture->textureData->width + (float)pixelsPerTile / texture->textureData->width, (float)y / texture->textureData->height + (float)pixelsPerTile / texture->textureData->height);

				//We can use width since width and height should always equal
				slices = texture->textureData->width / pixelsPerTile;
				tileMapIndex = index;

				return uv;
			}
			_cur++;
		}
	}

	return uv;
}

Sprite::~Sprite() {
	//Remove from renderer
	Core::GetRendererInstance()->RemoveSprite(this);
}