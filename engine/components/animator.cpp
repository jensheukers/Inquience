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
#include "../editor.h"
#include "../parser.h"

Animation::Animation(std::vector<UV*> frames) {
	this->frames = frames;
	this->frameTime = DEFAULT_FRAME_TIME;

	this->name = "Animation";

	this->currentFrameIndex = 0;
}

Animation::Animation() {
	this->frameTime = DEFAULT_FRAME_TIME;
	this->name = "Animation";

	this->currentFrameIndex = 0;
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

Animation* Animator::SetActiveAnimation(int index) {
	return currentAnimation = animations[index];
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

void Animator::OnComponentPropertiesEditor() {
	if (!this->GetOwner()->HasComponent<Sprite>()) {
		ImGui::Text("Warning: No Sprite component present on entity, Animator wont work");
		return;
	}

	ImGui::Text("Animations: ");
	if (ImGui::Button("Add New")) {
		this->AddAnimation(new Animation());
	}

	if (ImGui::Button("Load from file")) {
		EditorInputWindow* window = new EditorInputWindow();
		window->onApply.AddLambda([=]() {
			Parser* parser = new Parser(window->GetBuffer(), false);
			this->AddAnimation(parser->ReadAnimationFromFile());
			delete parser;
		});

		Editor::AddEditorWindow(window);
	}

	ImGui::BeginChild("Animations");
	for (size_t i = 0; i < this->animations.size(); i++) {
		ImGui::Text(this->animations[i]->name.c_str()); ImGui::SameLine();
		if (ImGui::Button(std::string("Select" + std::string("##") + std::to_string(i)).c_str())) {
			this->currentAnimation = this->animations[i];
		} ImGui::SameLine();
		if (ImGui::Button(std::string("Remove" + std::string("##") + std::to_string(i)).c_str())) {
			if (this->animations[i] == this->currentAnimation) {
				this->currentAnimation = nullptr;
			}

			delete this->RemoveAnimation(this->animations[i]);
		}

		if (ImGui::Button("Save to file")) {
			EditorInputWindow* window = new EditorInputWindow();
			window->onApply.AddLambda([=]() {
				Parser* parser = new Parser(window->GetBuffer(), false);
				parser->WriteAnimationToFile(this->animations[i]);
				delete parser;
			});

			Editor::AddEditorWindow(window);
		}
	}

	ImGui::EndChild();

	if (this->currentAnimation) {
		ImGui::Begin("Animation Editor");
		if (this->GetOwner()->GetComponent<Sprite>()->GetTexture()) {
			static bool addFrameWindowActive = false;	

			static UV* currentSelectedFrame = nullptr;

			if (addFrameWindowActive) {
				ImGui::Begin("Add Frame", &addFrameWindowActive);
				static int index;
				static int pixelsPerTile;
				ImGui::InputInt("Index", &index);
				ImGui::InputInt("Pixels per tile", &pixelsPerTile);
				ImGui::SameLine();
				UV uv = Sprite::Split(this->GetOwner()->GetComponent<Sprite>()->GetTexture(), pixelsPerTile, index);
				if (ImGui::Button("Add")) {
					currentSelectedFrame = this->currentAnimation->AddFrame(uv);
				}

				ImGui::Text("Preview: ");
				ImGui::Image((void*)(intptr_t)this->GetOwner()->GetComponent<Sprite>()->GetTexture()->_glTexture, ImVec2(128, 128),
							ImVec2(uv.leftDown.x, uv.leftDown.y), ImVec2(uv.rightUp.x, uv.rightUp.y));
				ImGui::End();
			}
			ImGui::Text("Properties: ");
			ImGui::InputFloat("Frame Time", &this->currentAnimation->frameTime);


			ImGui::InputText("Name", (char*)this->currentAnimation->name.c_str(), sizeof(this->currentAnimation->name));

			ImGui::Text("Timeline: ");
			if (ImGui::Button("Add Frame")) {
				addFrameWindowActive = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove Frame")) {
				if (currentSelectedFrame) {
					delete this->currentAnimation->RemoveFrame(currentSelectedFrame);
					currentSelectedFrame = nullptr;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Shift Left")) {
				if (currentSelectedFrame) {
					this->currentAnimation->ShiftLeft(currentSelectedFrame);
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Shift Right")) {
				if (currentSelectedFrame) {
					this->currentAnimation->ShiftRight(currentSelectedFrame);
				}
			}

			for (size_t i = 0; i < this->currentAnimation->GetFrames().size(); i++) {
				UV* frame = this->currentAnimation->GetFrames()[i];

				//Display images
				ImGui::Image((void*)(intptr_t)this->GetOwner()->GetComponent<Sprite>()->GetTexture()->_glTexture, ImVec2(64, 64),
								ImVec2(frame->leftDown.x, frame->leftDown.y), ImVec2(frame->rightUp.x, frame->rightUp.y));

				if (ImGui::IsItemHovered() && Input::GetButtonDown(BUTTONCODE_LEFT)) {
					currentSelectedFrame = this->currentAnimation->GetFrames()[i];
				}

				if (i + 1 != this->currentAnimation->GetFrames().size()) {
					ImGui::SameLine();
				}
			}

			ImGui::Spacing();
			ImGui::Text("Preview: ");
			if (this->currentAnimation->GetFrames().size() > 0) {
				if (ImGui::Button("Next Frame")) {
					this->currentAnimation->NextFrame();
				}
				ImGui::SameLine();

				if (ImGui::Button("Previous Frame")) {
					this->currentAnimation->PreviousFrame();
				}
				ImGui::SameLine();

				static bool playing = false;
				if (playing) {
					if (ImGui::Button("Stop")) {
						playing = false;
					}

					this->Update(); // Call update function, we want to play the animator
				}
				else {
					if (ImGui::Button("Play")) {
						playing = true;
						this->currentAnimation = this->currentAnimation;
					}
				}

				UV* uv = this->currentAnimation->CurrentFrame();
				ImGui::Image((void*)(intptr_t)this->GetOwner()->GetComponent<Sprite>()->GetTexture()->_glTexture, ImVec2(64, 64),
					ImVec2(uv->leftDown.x, uv->leftDown.y), ImVec2(uv->rightUp.x, uv->rightUp.y));
			}
			else {
				ImGui::Text("No frames to display");
			}
		}
		else {
			ImGui::Text("Cannot display settings as there is no texture on sprite");
		}

		ImGui::End();
	}
	else {
		ImGui::Text("There is no animation selected");
	}
}