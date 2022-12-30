//Soundmanager imported from aquarite3d
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, September 2019

#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <vector>
#include <AL/al.h>
#include <AL/alc.h>
#include "sound.h"

class SoundManager {
private:
	//Local members
	ALCdevice* device; /// @brief The currently used audio device
	ALCcontext* context; /// @brief The currently active context

	std::vector<Sound*> _sounds; /// @brief Vector containing registered sounds

public:
	/**
	* Constructor
	*/
	SoundManager();

	/**
	* Update gets called each frame by core
	*/
	void Update();

	/**
	* Adds a new sound to the sounds list
	*/
	void AddSound(Sound* sound);

	/**
	* Returns a sound pointer, where index matches
	*/
	Sound* GetSound(int index);

	/**
	* Removes a sound from the sounds list
	*/
	void RemoveSound(Sound* sound);

	/**
	* Removes a sound from the sounds list
	*/
	void RemoveSound(int index);

	/**
	* Plays the sound
	*/
	void PlaySound(int index);

	//Overload

	/**
	* Plays the sound
	*/
	void PlaySound(Sound* sound);

	/**
	* Stops playing the sound
	*/
	void StopSound(int index);

	//Overload

	/**
	* Stops playing the sound
	*/
	void StopSound(Sound* sound);

	/**
	* Destroys the sound manager
	*/
	void Destroy();
};


#endif // !SOUNDMANAGER_H
