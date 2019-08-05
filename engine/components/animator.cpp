// Source file Animator class.
//
// Animations are to be created in one texture, also the parent of this component should have a Sprite class component attached.
//
// Version: 1/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#include "../core.h"
#include "../entity.h"
#include "animator.h"

Animation::Animation(std::vector<UV> frames) {
	this->frames = frames;
	this->frameTime = DEFAULT_FRAME_TIME;
}

UV Animation::NextFrame() {
	if (currentFrameIndex != (int)frames.size() - 1) {
		currentFrameIndex = currentFrameIndex + 1;
	}
	else {
		currentFrameIndex = 0;
	}
	return CurrentFrame();
}

UV Animation::PreviousFrame() {
	if (currentFrameIndex != 0) {
		currentFrameIndex = currentFrameIndex - 1;
	}
	else {
		currentFrameIndex = (int)frames.size() - 1;
	}
	return CurrentFrame();
}

UV Animation::CurrentFrame() {
	return frames[currentFrameIndex];
}

Animator::Animator() {
	this->currentAnimation = nullptr; // Set to nullptr
	this->lastPlayTime = Core::GetTimeElapsed();
}

void Animator::AddAnimation(Animation* animation) {
	animations.push_back(animation);
}

void Animator::RemoveAnimation(int index) {
	for (int i = 0; i < (int)animations.size(); i++) {
		if (i == index) {
			animations.erase(animations.begin() + i);
		}
	}
}

Animation* Animator::GetAnimation(int index) {
	return animations[index];
}

Animation* Animator::SetActiveAnimation(int index) {
	return currentAnimation = animations[index];
}

void Animator::Update() {
	if (!currentAnimation) return;

	if (lastPlayTime + currentAnimation->frameTime < Core::GetTimeElapsed()) {
		//Set uv coordinates
		this->GetOwner()->GetComponent<Sprite>()->uv = currentAnimation->NextFrame();
		this->GetOwner()->GetComponent<Sprite>()->slices = currentAnimation->NextFrame().slices;

		//Set last playtime
		lastPlayTime = Core::GetTimeElapsed();
	}
}