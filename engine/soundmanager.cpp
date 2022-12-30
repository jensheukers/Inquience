//Soundmanager imported from aquarite3d
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, September 2019

#include "soundmanager.h"
#include "debug.h"

SoundManager::SoundManager() {
	this->device = alcOpenDevice(NULL);

	if (this->device) {
		Debug::Log("SoundManager: Could not load audio device");
	}

	this->context = alcCreateContext(this->device, NULL);
	if (!alcMakeContextCurrent(this->context)) {
		Debug::Log("SoundManager: Could not set current audio context");
	}

	Debug::Log("SoundManager: Initialized");
}

void SoundManager::Update() {
	//Play sounds
	for (size_t i = 0; i < this->_sounds.size(); i++) {
		if (this->_sounds[i]->GetAudioSource()->state != AL_STOPPED) {
			alGetSourcei(this->_sounds[i]->GetAudioSource()->sourceID,
				AL_SOURCE_STATE, &this->_sounds[i]->GetAudioSource()->state);
		}
	}
}

void SoundManager::AddSound(Sound* sound) {
	this->_sounds.push_back(sound);
}

Sound* SoundManager::GetSound(int index) {
	return this->_sounds[index];
}

void SoundManager::RemoveSound(Sound* sound) {
	for (unsigned i = 0; i < this->_sounds.size(); i++) {
		if (this->_sounds[i] == sound) {
			this->_sounds.erase(this->_sounds.begin() + i);
		}
	}
}

void SoundManager::RemoveSound(int index) {
	this->_sounds.erase(this->_sounds.begin() + index);
}

void SoundManager::PlaySound(int index) {
	alSourcePlay(SoundManager::GetSound(index)->GetAudioSource()->sourceID);
}

void SoundManager::PlaySound(Sound* sound) {
	for (size_t i = 0; i < this->_sounds.size(); i++) {
		if (this->_sounds[i] == sound) {
			SoundManager::PlaySound(i);
		}
	}
}

void SoundManager::StopSound(int index) {
	alSourceStop(SoundManager::GetSound(index)->GetAudioSource()->sourceID);
}

void SoundManager::StopSound(Sound* sound) {
	for (size_t i = 0; i < this->_sounds.size(); i++) {
		if (this->_sounds[i] == sound) {
			this->StopSound(i);
		}
	}
}

void SoundManager::Destroy() {
	alcMakeContextCurrent(NULL);
	alcDestroyContext(this->context);
	alcCloseDevice(this->device);

	//Remove and DELETE all sounds
	for (size_t i = 0; i < this->_sounds.size(); i++) {
		delete this->_sounds[i]; // Delete instance
		this->RemoveSound(i); // Remove pointer

		Debug::Log("SoundManager: Removed sound instance: " + std::to_string(i));
	}
}