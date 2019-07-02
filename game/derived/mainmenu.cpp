// Main menu source file, derives from Scene
//
// Version: 2/7/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, July 2019

//Include mainmenu header
#include "mainmenu.h"

StartButton::StartButton() {
	this->AddComponent<Sprite>();
	this->GetComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/ui_elements.tga"));
	this->GetComponent<Sprite>()->uvCoordinates = SpriteUV(this->GetComponent<Sprite>(), 64, 12);
	this->GetComponent<Sprite>()->SetScale(Vec2(2.5f, 2.5f));
}

void StartButton::OnEnter() {
	Debug::Log("asdfasdf");
}

void StartButton::OnLeave() {

}

void StartButton::OnStay() {

}

MainMenu::MainMenu() {
	//Construct scene
	this->SetActiveCamera(new Camera());

	StartButton* button = new StartButton();
	this->AddChild(button);
}