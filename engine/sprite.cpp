// Source file Sprite class.
//
// Version: 1/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, April 2019
#include "sprite.h"

Texture* Sprite::SetTexture(Texture* texture) {
	this->texture = texture;
	return this->GetTexture();
}

Texture* Sprite::GetTexture() {
	return this->texture;
}