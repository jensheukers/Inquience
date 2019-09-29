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
	static SoundManager* _instance; /// @brief The SoundManager instance

	//Local members
	ALCdevice* device; /// @brief The currently used audio device
	ALCcontext* context; /// @brief The currently active context

	std::vector<Sound*> _sounds; /// @brief Vector containing registered sounds

	/**
	* Returns the instance or creates one if not exists
	*/
	static SoundManager* GetInstance();

public:
	/**
	* Initializes the SoundManager
	*/
	static void Init();

	/**
	* Update gets called each frame by core
	*/
	static void Update();

	/**
	* Adds a new sound to the sounds list
	*/
	static void AddSound(Sound* sound);

	/**
	* Returns a sound pointer, where index matches
	*/
	static Sound* GetSound(int index);

	/**
	* Removes a sound from the sounds list
	*/
	static void RemoveSound(Sound* sound);

	/**
	* Removes a sound from the sounds list
	*/
	static void RemoveSound(int index);

	/**
	* Plays the sound
	*/
	static void PlaySound(int index);

	//Overload

	/**
	* Plays the sound
	*/
	static void PlaySound(Sound* sound);

	/**
	* Stops playing the sound
	*/
	static void StopSound(int index);

	//Overload

	/**
	* Stops playing the sound
	*/
	static void StopSound(Sound* sound);

	/**
	* Destroys the sound manager
	*/
	static void Destroy();
};


#endif // !SOUNDMANAGER_H
