// Header file Sprite class.
//
// Version: 6/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, April 2019
#ifndef SPRITE_H
#define SPRITE_H

//Include component, since sprite is a component
#include "component.h"

//Include Texture.h and vector2 
#include "texture.h"
#include "math/vec2.h"

class Sprite : public Component {
private:
	Texture* texture; /***< The texture the sprite uses*/
	Vec2 scale; /***< The scale of the sprite, default set in constructor to (1, 1)*/
	int zIndex; /***< The z-index of the sprite*/
public:
	/**
	* Constructor
	*/
	Sprite();

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

	/**
	* Sets the z-Index of the sprite
	* @param int
	* @return void
	*/
	void SetZIndex(int index);

	/**
	* Returns the zIndex
	* @return int
	*/
	int GetZIndex();

	/**
	* Sets the scale vec2
	* @param Vec2 scale
	* @return void
	*/
	void SetScale(Vec2 scale);

	/**
	* Returns the scale vec2
	* @return Vec2
	*/
	Vec2 GetScale();
};

#endif // !SPRITE_H