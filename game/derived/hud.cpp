// Source file for Head up display class, this class will handle every HUD event (Pause Menu, buttons, building tab ect...)
//
// Version: 8/8/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, August 2019

//Include hud.h
#include "hud.h"

//Include font.h for text
#include "../../engine/graphics/font.h"

//Include client.h we want the hud to know about client
#include "../client.h"

Hud::Hud() {
	// Create hud (Display amount of wood, stones and materials)
	//Wood
	this->wood_bg = new UIElement();
	this->wood_bg->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/hud/bg_elements.tga"));
	this->wood_bg->GetComponent<Sprite>()->Split(32, 56);
	this->wood_bg->GetComponent<Sprite>()->SetScale(Vec2(2.5f, 1));
	this->wood_bg->GetComponent<Sprite>()->SetZIndex(1);

	UIElement* wood_hud = new UIElement();
	wood_hud->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/hud/bg_elements.tga"));
	wood_hud->GetComponent<Sprite>()->Split(32, 48);
	wood_hud->GetComponent<Sprite>()->SetScale(Vec2(1, 1));
	wood_hud->GetComponent<Sprite>()->SetZIndex(1);

	Text* wood_hud_text = new Text(FontLoader::LoadFont("res/font/pixelplay.ttf"), "0");
	wood_hud_text->SetSize(0.75f);

	//Stones
	this->stones_bg = new UIElement();
	this->stones_bg->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/hud/bg_elements.tga"));
	this->stones_bg->GetComponent<Sprite>()->Split(32, 56);
	this->stones_bg->GetComponent<Sprite>()->SetScale(Vec2(2.5f, 1));
	this->stones_bg->GetComponent<Sprite>()->SetZIndex(1);

	UIElement* stones_hud = new UIElement();
	stones_hud->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/hud/bg_elements.tga"));
	stones_hud->GetComponent<Sprite>()->Split(32, 49);
	stones_hud->GetComponent<Sprite>()->SetScale(Vec2(1, 1));
	stones_hud->GetComponent<Sprite>()->SetZIndex(1);

	Text* stones_hud_text = new Text(FontLoader::LoadFont("res/font/pixelplay.ttf"), "0");
	stones_hud_text->SetSize(0.75f);

	//Materials
	this->materials_bg = new UIElement();
	this->materials_bg->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/hud/bg_elements.tga"));
	this->materials_bg->GetComponent<Sprite>()->Split(32, 56);
	this->materials_bg->GetComponent<Sprite>()->SetScale(Vec2(2.5f, 1));
	this->materials_bg->GetComponent<Sprite>()->SetZIndex(1);

	UIElement* materials_hud = new UIElement();
	materials_hud->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/hud/bg_elements.tga"));
	materials_hud->GetComponent<Sprite>()->Split(32, 50);
	materials_hud->GetComponent<Sprite>()->SetScale(Vec2(1, 1));
	materials_hud->GetComponent<Sprite>()->SetZIndex(1);

	Text* materials_hud_text = new Text(FontLoader::LoadFont("res/font/pixelplay.ttf"), "0");
	materials_hud_text->SetSize(0.75f);

	//Positions
	this->wood_bg->localPosition = Vec2(20, 10);
	wood_hud_text->localPosition = Vec2(32, 29);

	this->stones_bg->localPosition = Vec2(120, 10);
	stones_hud_text->localPosition = Vec2(32, 29);

	this->materials_bg->localPosition = Vec2(220, 10);
	materials_hud_text->localPosition = Vec2(32, 29);

	//Add to scene
	this->AddChild(this->wood_bg);
	this->wood_bg->AddChild(wood_hud);
	this->wood_bg->AddChild(wood_hud_text);

	this->AddChild(this->stones_bg);
	this->stones_bg->AddChild(stones_hud);
	this->stones_bg->AddChild(stones_hud_text);

	this->AddChild(this->materials_bg);
	this->materials_bg->AddChild(materials_hud);
	this->materials_bg->AddChild(materials_hud_text);
}