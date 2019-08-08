// Header file for Head up display class, this class will handle every HUD event (Pause Menu, buttons, building tab ect...)
//
// Version: 8/8/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, August 2019
#ifndef HUD_H
#define HUD_H

//Include ui.h
#include "../../engine/graphics/ui.h"

//A catalogue item has a description, whenever a user presses on the item within a catalogue, the code defined in OnStay() will be executed
class CatalogueItem : public UIElement {
private:
	std::string description; /***< The description of the catalogueItem*/
public:
	/**
	* Constructor
	* @param const char* TilemapTexture
	* @param int index
	* @param int pixelsPerTile
	*/
	CatalogueItem(const char* tilemapTexture, int index, int pixelsPerTile = 32);
};

//Catalogue holds Items
class Catalogue : public UIElement {
private:
	std::vector<CatalogueItem*> catalogueItems; /***< List of catalogueItems*/
public:
	/**
	* Constructor
	*/
	Catalogue();

	/**
	* Adds a catalogueItem to the catalogueList
	*/
	void AddCatalogueItem(CatalogueItem* item);
};

//SidebarButton, when this button is pressed a catalogue should be displayed on screen. (Catalogue holds all buttons for final structure buy)
class SideBarButton : public UIElement {
private:
	Catalogue* catalogue; /***< The catalogue for this button*/
public:
	/**
	* Constructor
	* @param const char* TilemapTexture
	* @param int index
	* @param int pixelsPerTile
	*/
	SideBarButton(const char* tilemapTexture, int index, int pixelsPerTile = 32);

	/**
	* Overwritten function OnStay()
	*/
	void OnStay() override;

	/**
	* Sets the catalogue pointer to given pointer value
	* @param Catalogue*
	*/
	void SetCatalogue(Catalogue* catalogue);

	/**
	* Returns the catalogue used for this button
	* @return Catalogue*
	*/
	Catalogue* GetCatalogue();
};

//Sidebar, has buttons for building system (shop)
class SideBar : public UIElement {
private:
	std::vector<SideBarButton*> buttons; /***< Vector containing all sidebar buttons*/
public:
	/**
	* Constructor
	*/
	SideBar();

	/**
	* Adds a sidebarbutton
	*/
	SideBarButton* AddSideBarButton(SideBarButton* button);
};

//Main hud class
class Hud : public UIElement {
public:
	UIElement* wood_bg;
	UIElement* stones_bg;
	UIElement* materials_bg;

	/**
	* Constructor
	*/
	Hud();
};

#endif // !HUD_H