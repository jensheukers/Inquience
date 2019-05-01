// Header file Animator class.
//
// Animations are to be created in one texture, also the parent of this component should have a Sprite class component attached.
//
// Animations only know uv coordinates, so the texture of the sprite is to be used
//
// Version: 1/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#ifndef ANIMATOR_H
#define ANIMATOR_H

//Default frame time
#define DEFAULT_FRAME_TIME 0.5f

//Include component.h & sprite.h
#include "../component.h"
#include "sprite.h"

//Include vector
#include <vector>

//Animation class
class Animation {
private:
	std::vector<SpriteUV> frames; /***< The uv coordinates of every frame, note that animation should use one default texture*/
	int currentFrameIndex; /***< The index of the current frame*/
public:
	float frameTime; /***< The time between frames, default set to DEFAULT_FRAME_TIME*/

	/**
	* Constructor, takes in vector containing all frames
	* @param std::vector<SpriteUV>
	*/
	Animation(std::vector<SpriteUV> frames);

	/**
	* Return the next frame coordinates
	* @return SpriteUV
	*/
	SpriteUV NextFrame();

	/**
	* Return the previous frame coordinates
	* @return SpriteUV
	*/
	SpriteUV PreviousFrame();

	/**
	* Returns the current frame coordinates
	* @return SpriteUV
	*/
	SpriteUV CurrentFrame();
	
};

//Animator class
class Animator : public Component {
private:
	std::vector<Animation*> animations; /*** List of animations*/
	Animation* currentAnimation; /*** The currently playing animation*/
	float lastPlayTime; /***< Last time a frame was changed*/
public:
	/**
	* Constructor
	*/
	Animator();

	/**
	* Adds a animation to the animations list
	* @param Animation*
	*/
	void AddAnimation(Animation* animation);

	/**
	* Removes a animation from the animations list, where index matches
	* @param int
	*/
	void RemoveAnimation(int index);

	/**
	* Gets a animation from the animations list, where index matches
	* @param int
	* @return Animation*
	*/
	Animation* GetAnimation(int index);

	/**
	* Sets the currently active animation where index matches
	* @param int
	* @return Animation*
	*/
	Animation* SetActiveAnimation(int index);

	/**
	* Animator Update function
	* Updates are called automaticly by parent, through this method animations can be played
	*/
	void Update() override;
};

#endif // !ANIMATOR_H
