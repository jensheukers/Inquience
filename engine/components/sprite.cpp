// Source file Sprite class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, September 2019
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

void Sprite::SetImGuiColorBuffer(ColorRGBA blendColor) {
	this->_imguiColorBuffer[0] = (float)(blendColor._r / 255);
	this->_imguiColorBuffer[1] = (float)(blendColor._g / 255);
	this->_imguiColorBuffer[2] = (float)(blendColor._b / 255);
	this->_imguiColorBuffer[3] = (float)(blendColor._a / 255);
}

Sprite::Sprite() {
	this->texture = nullptr;

	this->SetImGuiColorBuffer(this->blendColor);
	this->AddProperties();
}

Sprite::Sprite(const Sprite& sprite) {
	this->uv = sprite.uv;
	this->texture = sprite.texture;

	this->SetImGuiColorBuffer(this->blendColor);
	this->AddProperties();
}

void Sprite::AddProperties() {
	//Property to set texture
	AddProperty("Texture", [=](StringVector args) {
		SetTexture(TextureLoader::LoadTarga((char*)args[0].c_str()));
		}, [=]() -> StringVector {
			if (!this->GetTexture()) return {};
			return { std::string(this->GetTexture()->path) };
		});

	//Property to set uv
	AddProperty("SetUV", [=](StringVector args) {
		this->uv.leftUp = Vec2(std::stof(args[0]), std::stof(args[1])); // 0, 1
		this->uv.rightUp = Vec2(std::stof(args[2]), std::stof(args[3])); // 1, 1
		this->uv.leftDown = Vec2(std::stof(args[4]), std::stof(args[5])); // 0, 0
		this->uv.rightDown = Vec2(std::stof(args[6]), std::stof(args[7])); // 1, 0
		}, [=]() -> StringVector {
			return {
				std::to_string(this->uv.leftUp.x), std::to_string(this->uv.leftUp.y),
				std::to_string(this->uv.rightUp.x), std::to_string(this->uv.rightUp.y),
				std::to_string(this->uv.leftDown.x), std::to_string(this->uv.leftDown.y),
				std::to_string(this->uv.rightDown.x), std::to_string(this->uv.rightDown.y)
			};
	});

	//Property to set blendcolor
	AddProperty("SetBlendColor", [=](StringVector args) {
		this->blendColor._r = std::stoi(args[0]);
		this->blendColor._g = std::stoi(args[1]);
		this->blendColor._b = std::stoi(args[2]);
		this->blendColor._a = std::stoi(args[3]);
		SetImGuiColorBuffer(this->blendColor);
		}, [=]() -> StringVector {
			return {
				std::to_string(this->blendColor._r),
				std::to_string(this->blendColor._g),
				std::to_string(this->blendColor._b),
				std::to_string(this->blendColor._a)

			};
	});
}

Texture* Sprite::SetTexture(Texture* texture) {
	this->texture = texture;
	return this->GetTexture();
}

Texture* Sprite::GetTexture() {
	return this->texture;
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
				return uv;
			}
			_cur++;
		}
	}

	return uv;
}

UV Sprite::Split(Texture* texture, int pixelsPerTile, int index) {
	UV uv;
	if (!texture) {
		Debug::Log("Cannot split sprite - No texture present");
		return uv;
	}

	if (texture->textureData->width != texture->textureData->height) {
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
				return uv;
			}
			_cur++;
		}
	}

	return uv;
}