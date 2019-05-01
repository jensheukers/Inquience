// Header file Sprite class.
//
// Version: 1/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#ifndef SPRITE_H
#define SPRITE_H

//Include component, since sprite is a component
#include "../component.h"

//Include Texture.h and vector2 
#include "../texture.h"
#include "../math/vec2.h"

//Forward declare
class Sprite;

//Used to determine sprite uv coordinates
struct SpriteUV {
	Vec2 leftUp; /***< The leftUp texture coordinate*/
	Vec2 rightUp; /***< The rightUp texture coordinate*/
	Vec2 leftDown; /***< The leftDown texture coordinate*/
	Vec2 rightDown; /***< The rightDown texture coordinate*/

	/**
	* Default constructor
	*/
	SpriteUV();

	/**
	* Constructor, Specify Sprite, then amount of tiles in texture, and give index
	* @param Sprite*
	* @param int amount
	* @param int index
	*/
	SpriteUV(Sprite* texture, int amount, int index);
};

class Sprite : public Component {
private:
	Texture* texture; /***< The texture the sprite uses*/
	Vec2 scale; /***< The scale of the sprite, default set in constructor to (1, 1)*/
	int zIndex; /***< The z-index of the sprite*/
	int splits; /*** Amount of devides used if uvCoordinates have been modified (Allows for scaling in Renderer). */
public:
	SpriteUV uvCoordinates; /***< The UV Coordinates of the sprite*/

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

	/**
	* Returns the amount of splits
	* @return int
	*/
	int GetSplits();

	/**
	* Sets the amount of splits, unneccecary to call from user end
	* @param int
	*/
	void SetSplits(int splits);
};

#endif // !SPRITE_H