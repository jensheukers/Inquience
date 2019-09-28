//Sound imported from aquarite3d
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, September 2019

#include "core.h"
#include "sound.h"
#include "debug.h"

#define BUFFER_SIZE 32768 // 32 KB buffers

#define DEFAULT_DISTANCE_MODEL AL_LINEAR_DISTANCE_CLAMPED
#define DEFAULT_ROLLOF_FACTOR 3.3f
#define DEFAULT_PITCH 1.0f
#define DEFAULT_GAIN 1.0f
#define DEFAULT_MAX_DIST 50.0f
#define DEFAULT_MAX_REF_DIST 5.0f

Sound::Sound() {
	this->source = nullptr;
	this->pitch = 1;
	this->gain = 1;
}

Sound::~Sound() {
	if (this->source == nullptr) return;
	alDeleteSources(1, &this->source->sourceID);
	alDeleteBuffers(1, &this->source->bufferID);
}

void Sound::LoadAudioSource(std::string path) {
	Debug::Log("Loading audio source : " + path);
	
	ALCenum error;

	AudioSource* aSource = new AudioSource(); // Create new audio source struct instance

	aSource->sourcePath = Core::GetExecutableDirectoryPath() + path; // Set path
	alGenSources((ALuint)1, &aSource->sourceID); // Generate sources

	error = alGetError();
	if (error != AL_NO_ERROR) { // If error
		Debug::Log("Could not load audio source : " + error);
		return;
	}

	int endian = 0;
	int bitStream;
	long bytes;
	char array[BUFFER_SIZE];
	FILE* f;

	//Open for binary reading
	f = fopen(aSource->sourcePath.c_str(), "rb");

	if (f == NULL) {
		Debug::Log("Could not load .ogg file");
		return;
	}

	vorbis_info* pInfo;
	OggVorbis_File oggFile;
	ov_open(f, &oggFile, NULL, 0); // Open the ogg file

								   //Get some information about the ogg file
	pInfo = ov_info(&oggFile, -1);

	// Check the number of channels... always use 16-bit samples
	if (pInfo->channels == 1)
		aSource->format = AL_FORMAT_MONO16;
	else
		aSource->format = AL_FORMAT_STEREO16;
	// End if

	//Frequency of the sampling rate
	aSource->freq = pInfo->rate;

	//Decode the data
	do {
		//Read up to a buffers worth of decoded sound data
		bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
		//Append to end of buffer
		aSource->bufferData.insert(aSource->bufferData.end(), array, array + bytes);
	} while (bytes > 0);

	ov_clear(&oggFile);

	//We have loaded the .ogg file into memory, we can now generate the buffers and buffer the data
	//Generate buffers
	alGenBuffers((ALuint)1, &aSource->bufferID);

	error = alGetError();
	if (error != AL_NO_ERROR) { // If error
		Debug::Log("Could not load audio source : " + error);
		return;
	}
	
	//Buffer the data
	alBufferData(aSource->bufferID, aSource->format, &aSource->bufferData[0], 
				 aSource->bufferData.size() * sizeof(char), aSource->freq);

	error = alGetError();
	if (error != AL_NO_ERROR) { // If error
		Debug::Log("Could not buffer audio data");
		return;
	}

	alSourcei(aSource->sourceID, AL_BUFFER, aSource->bufferID);
	this->source = aSource; // Set the source

	//Set default values
	alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
	this->SetPitch(DEFAULT_PITCH);
	this->SetGain(DEFAULT_GAIN);
	this->Loop(false);

	Debug::Log("Loaded audio source : " + path);
}

void Sound::SetPitch(float pitch) {
	if (this->source == nullptr) return;

	alSourcef(this->source->sourceID, AL_PITCH, (ALfloat)pitch);
	this->pitch = (ALfloat)pitch;
}

ALfloat Sound::GetPitch() {
	return this->pitch;
}

void Sound::SetGain(float gain) {
	if (this->source == nullptr) return;

	alSourcef(this->source->sourceID, AL_GAIN, (ALfloat)gain);
	this->gain = (ALfloat)gain;
}

ALfloat Sound::GetGain() {
	return this->gain;
}

void Sound::Loop(bool state) {
	if (this->source == nullptr) return;

	ALboolean s;
	if (state) {
		s = AL_TRUE;
	}
	else {
		s = AL_FALSE;
	}
	alSourcei(this->source->sourceID, AL_LOOPING, s);
}

AudioSource* Sound::GetAudioSource() {
	return this->source;
}

void Sound::SetAudioSource(AudioSource* source) {
	this->source = source;
}