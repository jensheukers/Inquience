//Soundmanager imported from aquarite3d
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, September 2019

#include "soundmanager.h"
#include "debug.h"

SoundManager* SoundManager::_instance;

SoundManager* SoundManager::GetInstance() {
	if (!_instance) {
		_instance = new SoundManager();
	}
	return _instance;
}

void SoundManager::Init() {
	SoundManager::GetInstance()->device = alcOpenDevice(NULL);

	if (!SoundManager::GetInstance()->device) {
		Debug::Log("SoundManager: Could not load audio device");
	}

	SoundManager::GetInstance()->context = alcCreateContext(SoundManager::GetInstance()->device, NULL);
	if (!alcMakeContextCurrent(SoundManager::GetInstance()->context)) {
		Debug::Log("SoundManager: Could not set current audio context");
	}

	Debug::Log("SoundManager: Initialized");
}

void SoundManager::Update() {
	//Play sounds
	for (size_t i = 0; i < SoundManager::GetInstance()->_sounds.size(); i++) {
		if (SoundManager::GetInstance()->_sounds[i]->GetAudioSource()->state != AL_STOPPED) {
			alGetSourcei(SoundManager::GetInstance()->_sounds[i]->GetAudioSource()->sourceID, 
				AL_SOURCE_STATE, &SoundManager::GetInstance()->_sounds[i]->GetAudioSource()->state);
		}
	}
}

void SoundManager::AddSound(Sound* sound) {
	SoundManager::GetInstance()->_sounds.push_back(sound);
}

Sound* SoundManager::GetSound(int index) {
	return SoundManager::GetInstance()->_sounds[index];
}

void SoundManager::RemoveSound(Sound* sound) {
	for (int i = 0; i < SoundManager::GetInstance()->_sounds.size(); i++) {
		if (SoundManager::GetInstance()->_sounds[i] == sound) {
			SoundManager::GetInstance()->_sounds.erase(SoundManager::GetInstance()->_sounds.begin() + i);
		}
	}
}

void SoundManager::RemoveSound(int index) {
	SoundManager::GetInstance()->_sounds.erase(SoundManager::GetInstance()->_sounds.begin() + index);
}

void SoundManager::PlaySound(int index) {
	alSourcePlay(SoundManager::GetSound(index)->GetAudioSource()->sourceID);
}

void SoundManager::PlaySound(Sound* sound) {
	for (size_t i = 0; i < SoundManager::GetInstance()->_sounds.size(); i++) {
		if (SoundManager::GetInstance()->_sounds[i] == sound) {
			SoundManager::PlaySound(i);
		}
	}
}

void SoundManager::StopSound(int index) {
	alSourceStop(SoundManager::GetSound(index)->GetAudioSource()->sourceID);
}

void SoundManager::StopSound(Sound* sound) {
	for (size_t i = 0; i < SoundManager::GetInstance()->_sounds.size(); i++) {
		if (SoundManager::GetInstance()->_sounds[i] == sound) {
			SoundManager::StopSound(i);
		}
	}
}

void SoundManager::Destroy() {
	alcMakeContextCurrent(NULL);
	alcDestroyContext(SoundManager::GetInstance()->context);
	alcCloseDevice(SoundManager::GetInstance()->device);

	//Remove and DELETE all sounds
	for (size_t i = 0; i < SoundManager::GetInstance()->_sounds.size(); i++) {
		delete SoundManager::GetInstance()->_sounds[i]; // Delete instance
		SoundManager::RemoveSound(i); // Remove pointer

		Debug::Log("SoundManager: Removed sound instance: " + std::to_string(i));
	}
}