// Header file Sprite class.
//
// Version: 2/7/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, July 2019
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
struct UV {
	Vec2 leftUp; /***< The leftUp texture coordinate*/
	Vec2 rightUp; /***< The rightUp texture coordinate*/
	Vec2 leftDown; /***< The leftDown texture coordinate*/
	Vec2 rightDown; /***< The rightDown texture coordinate*/

	/**
	* Constructor
	*/
	UV();
};

class Sprite : public Component {
private:
	Texture* texture; /***< The texture the sprite uses*/
	Vec2 scale; /***< The scale of the sprite, default set in constructor to (1, 1)*/
	int zIndex; /***< The z-index of the sprite*/
public:
	UV uv; /***< The UV Coordinates of the sprite*/
	int slices; /*** Amount of devides used if uv coordinates have been modified (Allows for scaling in Renderer). */
	int tileMapIndex; /***< The tile map index if the sprite has been split, else returns 0*/

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
	* Returns the center of the sprite in local space
	* @return Vec2
	*/
	Vec2 GetCenter();

	/**
	* Splits the texture, sprite should have a texture assigned and texture width & height should be equal to work
	* sets uv to uvData for tile index
	* (This function allows for TileMaps to be used)
	* @param int, amount of pixels per tile
	* @param int, index for selected tile
	* @return UV
	*/
	UV Split(int pixelsPerTile, int index);

	/**
	* Splits the texture, texture needs to be provided, UV coordinates of split are returned
	* @param Texture*, pointer to texture
	* @param int, amount of pixels per tile
	* @param int, index for selected tile
	* @return UV
	*/
	static UV Split(Texture* texture, int pixelsPerTile, int index);

	/**
	* Destructor
	*/
	~Sprite();
};

#endif // !SPRITE_H