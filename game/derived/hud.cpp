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

//engine related headers
#include "../../engine/graphics/font.h"
#include "../../engine/input.h"

//Include client.h we want the hud to know about client
#include "../client.h"

CatalogueItem::CatalogueItem(const char* tilemapTexture, int index, int pixelsPerTile) {
	this->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga((char*)tilemapTexture));
	this->GetComponent<Sprite>()->Split(pixelsPerTile, index);
}

Catalogue::Catalogue() {
	this->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/placeholder.tga"));
	this->GetComponent<Sprite>()->SetScale(Vec2(5.0f, 1.0f));
}

void Catalogue::AddCatalogueItem(CatalogueItem * item) {
	static float xOffset = 0;

	//Set position
	item->localPosition = Vec2(xOffset, 0);
	xOffset += 32; //Increment

	//Add to children
	this->AddChild(item);
	catalogueItems.push_back(item);

	//Disable item for now
	item->SetActive(false);
}

SideBarButton::SideBarButton(const char* tilemapTexture, int index, int pixelsPerTile) {
	this->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga((char*)tilemapTexture));
	this->GetComponent<Sprite>()->Split(pixelsPerTile, index);

	//set catalogue to nullptr
	this->catalogue = nullptr;
}

void SideBarButton::OnStay() {
	//If clicked
	if (Input::GetButtonDown(BUTTONCODE_LEFT)) {
		//Open catalogue if not open yet & if instance is present
		if (this->catalogue) {
			if (!catalogue->Active()) {
				catalogue->SetActive(true);
			}
			else {
				catalogue->SetActive(false);
			}
		}
	}
}

void SideBarButton::SetCatalogue(Catalogue* catalogue) {
	//If we already have a catalogue, remove it from children
	if (this->catalogue) {
		this->RemoveChild(this->catalogue);
	}

	//Set pointer
	this->catalogue = catalogue;

	//Set catalogue not active
	this->catalogue->SetActive(false);
	this->AddChild(this->catalogue);

	//Set position correct
	this->catalogue->localPosition = Vec2(32, 0);
}

Catalogue* SideBarButton::GetCatalogue() {
	return this->catalogue;
}

SideBar::SideBar() {
	//First construct the background of the sidebar 
	this->AddComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/placeholder.tga"));
	this->GetComponent<Sprite>()->SetScale(Vec2(1.0f, 12.5f));

	SideBarButton* structureShop = AddSideBarButton(new SideBarButton("res/hud/bg_elements.tga", 50));
	structureShop->SetCatalogue(new Catalogue());
	structureShop->GetCatalogue()->AddCatalogueItem(new CatalogueItem("res/hud/bg_elements.tga", 50));
	structureShop->GetCatalogue()->AddCatalogueItem(new CatalogueItem("res/hud/bg_elements.tga", 50));
}

SideBarButton* SideBar::AddSideBarButton(SideBarButton* button) {
	static float yOffset = 0; 

	//Set position
	button->localPosition = Vec2(0, yOffset);
	yOffset += 32; // Increment

	//Add to children
	this->AddChild(button);
	this->buttons.push_back(button);
	return button;
}

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

	//Add the sidebar
	SideBar* sidebar = new SideBar();

	//Set position
	sidebar->localPosition = Vec2(0, 150);

	//Add child
	this->AddChild(sidebar);
}