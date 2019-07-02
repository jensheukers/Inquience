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

//Include core to receive resolution
#include "../../engine/core.h"

//Include input
#include "../../engine/input.h"

StartButton::StartButton() {
	this->AddComponent<Sprite>();
	this->GetComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/ui_elements.tga"));
	this->GetComponent<Sprite>()->uvCoordinates = SpriteUV(this->GetComponent<Sprite>(), 64, 12);
	this->GetComponent<Sprite>()->SetScale(Vec2(3, 3));
}
void StartButton::OnEnter() {
}
void StartButton::OnLeave() {
}
void StartButton::OnStay() {
}

OptionButton::OptionButton() {
	this->AddComponent<Sprite>();
	this->GetComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/ui_elements.tga"));
	this->GetComponent<Sprite>()->uvCoordinates = SpriteUV(this->GetComponent<Sprite>(), 64, 12);
	this->GetComponent<Sprite>()->SetScale(Vec2(3, 3));
}
void OptionButton::OnEnter() {
}
void OptionButton::OnLeave() {

}
void OptionButton::OnStay() {
}

QuitButton::QuitButton() {
	this->AddComponent<Sprite>();
	this->GetComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/ui_elements.tga"));
	this->GetComponent<Sprite>()->uvCoordinates = SpriteUV(this->GetComponent<Sprite>(), 64, 12);
	this->GetComponent<Sprite>()->SetScale(Vec2(3, 3));
}

void QuitButton::OnEnter() {
}

void QuitButton::OnLeave() {

}

void QuitButton::OnStay() {
}

MainMenu::MainMenu() {
	//Construct scene
	this->SetActiveCamera(new Camera());

	//Add start button
	StartButton* startButton = new StartButton();
	startButton->localPosition = Core::GetRendererInstance()->GetResolution() / 2 - Vec2((startButton->GetComponent<Sprite>()->GetScale().x * startButton->GetComponent<Sprite>()->GetTextureScale().x) / 2,
																						 (startButton->GetComponent<Sprite>()->GetScale().y * startButton->GetComponent<Sprite>()->GetTextureScale().y) / 2);
	startButton->localPosition = startButton->localPosition + Vec2(0, -50);
	this->AddChild(startButton);

	//Add options button
	OptionButton* optionButton = new OptionButton();
	optionButton->localPosition = Core::GetRendererInstance()->GetResolution() / 2 - Vec2((optionButton->GetComponent<Sprite>()->GetScale().x * optionButton->GetComponent<Sprite>()->GetTextureScale().x) / 2,
																						  (optionButton->GetComponent<Sprite>()->GetScale().y * optionButton->GetComponent<Sprite>()->GetTextureScale().y) / 2);
	optionButton->localPosition = optionButton->localPosition + Vec2(0, 100);
	this->AddChild(optionButton);

	//Add quit button
	QuitButton* quitButton = new QuitButton();
	quitButton->localPosition = Core::GetRendererInstance()->GetResolution() / 2 - Vec2((quitButton->GetComponent<Sprite>()->GetScale().x * quitButton->GetComponent<Sprite>()->GetTextureScale().x) / 2,
																						(quitButton->GetComponent<Sprite>()->GetScale().y * quitButton->GetComponent<Sprite>()->GetTextureScale().y) / 2);
	quitButton->localPosition = quitButton->localPosition + Vec2(0, 250);
	this->AddChild(quitButton);
}