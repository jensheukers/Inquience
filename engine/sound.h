//Sound imported from aquarite3d
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, September 2019

#ifndef SOUND_H
#define SOUND_H
#include <string>
#include <vector>
#include <AL/al.h>
#include <AL/alc.h>
#include <vorbis/vorbisfile.h>

//AudioSource struct
struct AudioSource {
	std::string sourcePath; /// @brief The path to the source
	ALint state; /// @brief The state of the source
	ALuint bufferID; /// @brief Pointer to the sound buffer object
	ALuint sourceID; /// @brief The source pointer to the buffer object
	ALenum format; /// @brief the sound data format
	ALsizei freq; /// @brief The frequency of the sound data
	std::vector<char> bufferData; /// @brief Teh sound buffer data from file
};

class Sound {
private:
	ALuint state; /// @brief The current source state
	AudioSource* source; /// @brief the current audio source

	//Properties
	ALfloat pitch; ///@brief The pitch of the sound
	ALfloat gain; ///@brief The gain of the sound
public:
	/**
	* Constructor
	*/
	Sound();

	/**
	* Destructor
	*/
	~Sound();

	/**
	* Loads a AudioSource and sets source pointer to it
	*/
	void LoadAudioSource(std::string path);

	/**
	* Set the pitch of the sound
	*/
	void SetPitch(float pitch);

	/**
	* Returns the pitch of the sound
	*/
	ALfloat GetPitch();

	/**
	* Set the gain of the sound
	*/
	void SetGain(float gain);

	/**
	* Returns the gain of the sound
	*/
	ALfloat GetGain();

	/**
	* If true sound will loop, else wont
	*/
	void Loop(bool state);

	/**
	* Returns the audio source, if not yet loaded returns nullptr
	*/
	AudioSource* GetAudioSource();

	/**
	* Sets the active audio source
	*/
	void SetAudioSource(AudioSource* source);
};


#endif // !SOUND_H