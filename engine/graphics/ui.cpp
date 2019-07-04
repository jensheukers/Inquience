// Source file for User Interface Elements.
//
// Version: 2/7/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, July 2019
#include "ui.h"

//Include input for position checks, and physics for checks
#include "../input.h"
#include "../math/physics.h"

void UIElement::Update() {
	if (!this->HasComponent<Sprite>()) return;
	if (!this->GetComponent<Sprite>()->GetTexture()) return;

	Vec2 bounds = Vec2((float)this->GetComponent<Sprite>()->GetTexture()->textureData->width, (float)this->GetComponent<Sprite>()->GetTexture()->textureData->height) * this->GetComponent<Sprite>()->GetScale();
	if (Physics::InBounds(Input::GetMousePosition(), this->GetPosition(), this->GetPosition() + (bounds / (float)this->GetComponent<Sprite>()->slices))) {
		if (!entered) {
			OnEnter();
			entered = true;
		}
		else {
			OnStay();
		}
	}
	else {
		if (entered) {
			OnLeave();
			entered = false;
		}
	}
}