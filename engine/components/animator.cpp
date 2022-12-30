// Source file Animator class.
//
// Animations are to be created in one texture, also the parent of this component should have a Sprite class component attached.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, November 2019
#include "animator.h"

#include "../core.h"
#include "../entity.h"
#include "../input.h"
#include "../parser.h"

Animation::Animation(std::vector<UV*> frames) {
	this->frames = frames;
	this->frameTime = DEFAULT_FRAME_TIME;

	this->name = "Animation";

	this->currentFrameIndex = 0;
	this->mirrored = false;
}

Animation::Animation() {
	this->frameTime = DEFAULT_FRAME_TIME;
	this->name = "Animation";

	this->currentFrameIndex = 0;
	this->mirrored = false;
}

UV* Animation::NextFrame() {
	if (currentFrameIndex + 1 != (int)frames.size()) {
		currentFrameIndex = currentFrameIndex + 1;
	}
	else {
		currentFrameIndex = 0;
	}
	return CurrentFrame();
}

UV* Animation::PreviousFrame() {
	if (currentFrameIndex != 0) {
		currentFrameIndex = currentFrameIndex - 1;
	}
	else {
		currentFrameIndex = (int)frames.size() - 1;
	}
	return CurrentFrame();
}

UV* Animation::CurrentFrame() {
	return frames[currentFrameIndex];
}

UV* Animation::AddFrame(UV frameData) {
	UV* uv = new UV(frameData);
	this->frames.push_back(uv);
	return uv;
}

UV* Animation::RemoveFrame(UV* frameData) {
	for (size_t i = 0; i < frames.size(); i++) {
		if (frames[i] == frameData) {
			frames.erase(frames.begin() + i);
		}
	}
	return frameData;
}

void Animation::ShiftRight(UV* frameData) {
	for (int i = frames.size() - 1; i >= 0; i--) {
		if (frames[i] == frameData && i + 1 != frames.size()) {
			UV* swap = frames[i + 1];
			frames[i + 1] = frameData;
			frames[i] = swap;
		}
	}
}

void Animation::ShiftLeft(UV* frameData) {
	for (int i = frames.size() - 1; i >= 0; i--) {
		if (frames[i] == frameData && i - 1 != -1) {
			UV* swap = frames[i - 1];
			frames[i - 1] = frameData;
			frames[i] = swap;
		}
	}
}

void Animation::Mirror() {
	for (size_t i = 0; i < frames.size(); i++) {
		UV _temp = *frames[i];

		frames[i]->leftUp = _temp.rightUp;
		frames[i]->leftDown = _temp.rightDown;
		frames[i]->rightUp = _temp.leftUp;
		frames[i]->rightDown = _temp.leftDown;
	}

	this->mirrored = !this->mirrored;
}

bool Animation::Mirrored() {
	return this->mirrored;
}

Animator::Animator() {
	this->currentAnimation = nullptr; // Set to nullptr
	this->lastPlayTime = Core::GetTimeElapsed();

	/*this->AddProperty("Animations", [=](StringVector args) {
		for (size_t i = 0; i < args.size(); i++) {
			//Read animation from file
		}
	}, [=]() -> StringVector {
		for (size_t i = 0; i < animations.size(); i++) {
			
		}
	});*/
}

Animation* Animator::AddAnimation(Animation* animation) {
	animations.push_back(animation);
	return animation;
}

Animation* Animator::AddAnimation(std::string directory) {
	Parser* parser = new Parser(directory, true);
	Animation* animation = parser->ReadAnimationFromFile();
	delete parser;

	animations.push_back(animation);
	return animation;
}


Animation* Animator::RemoveAnimation(Animation* animation) {
	for (size_t i = 0; i < animations.size(); i++) {
		if (animation == animations[i]) {
			animations.erase(animations.begin() + i);
		}
	} 
	return animation;
}

Animation* Animator::GetAnimation(int index) {
	return animations[index];
}

Animation* Animator::GetAnimation(std::string name) {
	for (size_t i = 0; i < animations.size(); i++) {
		if (animations[i]->name == name) {
			return animations[i];
		}
	}
	return nullptr;
}

Animation* Animator::SetActiveAnimation(int index) {
	return currentAnimation = animations[index];
}

Animation* Animator::SetActiveAnimation(std::string name) {
	return currentAnimation = GetAnimation(name);
}

void Animator::Update() {
	if (!this->currentAnimation || !this->GetOwner()->GetComponent<Sprite>()) return; // ensure

	if (lastPlayTime + currentAnimation->frameTime < Core::GetTimeElapsed()) {
		//Set uv coordinates
		this->GetOwner()->GetComponent<Sprite>()->uv = *currentAnimation->NextFrame();

		//Set last playtime
		lastPlayTime = Core::GetTimeElapsed();
	}
}