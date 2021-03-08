// Header file Animator class.
//
// Animations are to be created in one texture, also the parent of this component should have a Sprite class component attached.
//
// Animations only know uv coordinates, so the texture of the sprite is to be used
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, November 2019
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
	std::vector<UV*> frames; /***< The uv coordinates of every frame, note that animation should use one default texture*/
	int currentFrameIndex; /***< The index of the current frame*/
	bool mirrored; /***< If true UV Coordinates will be mirrored */
public:
	float frameTime; /***< The time between frames, default set to DEFAULT_FRAME_TIME*/
	std::string name; /***< The name of the animation, so we can reference it*/

	/**
	* Constructor, takes in vector containing all frames* @param std::vector<UV>
	*/
	Animation(std::vector<UV*> frames);

	/**
	* Constructor, Empty animation
	*/
	Animation();

	/**
	* Return the next frame coordinates
	* @return UV
	*/
	UV* NextFrame();

	/**
	* Return the previous frame coordinates
	* @return UV
	*/
	UV* PreviousFrame();

	/**
	* Returns the current frame coordinates
	* @return UV
	*/
	UV* CurrentFrame();

	/**
	* Adds a frame to the frames vector
	*/
	UV* AddFrame(UV frameData);

	/**
	* Removes a frame from the frames vector
	*/
	UV* RemoveFrame(UV* frameData);

	/**
	* Shifts a frame to the right of the frames array
	*/
	void ShiftRight(UV* frameData);

	/**
	* Shifts a frame to the left of the frames array
	*/
	void ShiftLeft(UV* frameData);

	/**
	* Mirrors the animation
	*/
	void Mirror();

	/**
	* Returns if animations is mirrored
	*/
	bool Mirrored();

	/**
	* Returns the frames UV data vector
	*/
	std::vector<UV*> GetFrames() const { return this->frames; };
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
	* @return Animation*
	*/
	Animation* AddAnimation(Animation* animation);

	/**
	* Adds a animation to the animations list
	* @param std::string file
	* @return Animation*
	*/
	Animation* AddAnimation(std::string file);

	/**
	* Removes a animation from the animations list
	* @param Animation*
	* @return Animation*
	*/
	Animation* RemoveAnimation(Animation* animation);

	/**
	* Gets a animation from the animations list, where index matches
	* @param int
	* @return Animation*
	*/
	Animation* GetAnimation(int index);

	/**
	* Gets a animation from the animations list, where name matches
	* @param std::string name
	* @return Animation*
	*/
	Animation* GetAnimation(std::string name);

	/**
	* Sets the currently active animation where index matches
	* @param int
	* @return Animation*
	*/
	Animation* SetActiveAnimation(int index);

	/**
	* Sets the currently active animation by name
	* @param std::string name
	* @return Animation*
	*/
	Animation* SetActiveAnimation(std::string name);

	/**
	* Animator Update function
	* Updates are called automaticly by parent, through this method animations can be played
	*/
	void Update() override;

	virtual Animator* New() override { return new Animator(); }
	virtual Animator* Copy() const { return new Animator(*this); }

	virtual void OnComponentPropertiesEditor() override;
};

#endif // !ANIMATOR_H
