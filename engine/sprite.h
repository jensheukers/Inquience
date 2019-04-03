// Header file Sprite class.
//
// Version: 1/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, April 2019
#ifndef SPRITE_H
#define SPRITE_H

//Include Texture.h and vector2 
#include "texture.h"
#include "math/vec2.h"

class Sprite {
private:
	Texture* texture; /***< The texture the sprite uses*/
public:
	/**
	* Sets the texture of the sprite, then returns that texture
	* @param Texture*
	* @return Texture*
	*/
	Texture* SetTexture(Texture* texture);

	/**
	* Returns the texture of the sprite
	* @return Texture*
	*/
	Texture* GetTexture();
};

#endif // !SPRITE_H