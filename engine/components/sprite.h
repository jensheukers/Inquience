// Header file Sprite class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2020
#ifndef SPRITE_H
#define SPRITE_H

//Include component, since sprite is a component
#include "../component.h"

//Include Texture.h and vector2 
#include "../texture.h"
#include "../math/vec2.h"

#include "../graphics/color.h"

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

	float _imguiColorBuffer[4]; /***< Buffer for imgui color picker*/

	/**
	* Sets the imgui color buffer to blendcolor
	*/
	void SetImGuiColorBuffer(ColorRGBA blendColor);
public:
	UV uv; /***< The UV Coordinates of the sprite*/
	ColorRGBA blendColor; /***< Blend Color*/

	/**
	* Constructor
	*/
	Sprite();

	/**
	* Copy constructor
	*/
	Sprite(const Sprite& sprite);

	/**
	* Adds properties to sprite
	*/
	void AddProperties();

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

	virtual Sprite* New() override { return new Sprite(); }
	virtual Sprite* Copy() const { return new Sprite(*this); }
};

#endif // !SPRITE_H