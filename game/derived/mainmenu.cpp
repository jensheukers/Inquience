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

	Font* font = new Font(Core::GetExecutableDirectoryPath().append("res/yoster.ttf"));
	text = new Text(font, "Play");
	text->localPosition = Vec2(35, 110);
	this->AddChild(text);
}

void StartButton::OnEnter() {
	text->SetColor(glm::vec3(1,0,0));
}

void StartButton::OnLeave() {
	text->SetColor(glm::vec3(1, 1, 1));
}

void StartButton::OnStay() {
}

OptionButton::OptionButton() {
	this->AddComponent<Sprite>();
	this->GetComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/ui_elements.tga"));
	this->GetComponent<Sprite>()->uvCoordinates = SpriteUV(this->GetComponent<Sprite>(), 64, 12);
	this->GetComponent<Sprite>()->SetScale(Vec2(3, 3));

	Font* font = new Font(Core::GetExecutableDirectoryPath().append("res/yoster.ttf"));
	text = new Text(font, "Options");
	text->localPosition = Vec2(0, 110);
	this->AddChild(text);
}

void OptionButton::OnEnter() {
	text->SetColor(glm::vec3(1, 0, 0));
}

void OptionButton::OnLeave() {
	text->SetColor(glm::vec3(1, 1, 1));
}

void OptionButton::OnStay() {

}

QuitButton::QuitButton() {
	this->AddComponent<Sprite>();
	this->GetComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/ui_elements.tga"));
	this->GetComponent<Sprite>()->uvCoordinates = SpriteUV(this->GetComponent<Sprite>(), 64, 12);
	this->GetComponent<Sprite>()->SetScale(Vec2(3, 3));

	Font* font = new Font(Core::GetExecutableDirectoryPath().append("res/yoster.ttf"));
	text = new Text(font, "Quit");
	text->localPosition = Vec2(35, 110);
	this->AddChild(text);
}

void QuitButton::OnEnter() {
	text->SetColor(glm::vec3(1, 0, 0));
}

void QuitButton::OnLeave() {
	text->SetColor(glm::vec3(1, 1, 1));
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