// Header file for Editor class.
//
// Version: 3/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#ifndef EDITOR_H
#define EDITOR_H

//Include imgui.h & entity.h
#include "../external/imgui/imgui.h"
#include "entity.h"

//Grid structure
struct GridTile {
	bool occupied; /***< True if a tile is occupied*/
	Vec2 position; /***< Position of the gridTile*/
	Entity* tileEntity; /***< The entity that is occupying the tile*/
};

struct Grid {
	Vec2 scale; /***< The scale of the grid*/
	Vec2 tileScale; /*** The scale of one tile*/
	std::vector<GridTile> gridTiles; /***< Vector containing all gridTiles*/

	/**
	* Constructs the grid, fills gridTiles vector
	*/
	void Construct();

	/**
	* Returns the Tile nearest to position
	* @param Vec2
	* @return GridTile
	*/
	GridTile* GetTilePosition(Vec2 position);
};

class Editor {
private:
	//Local members
	bool loadMenuActive; /***< If true the loadMenu will be shown*/
	bool saveMenuActive; /***< If true the saveMenu will be shown*/

	bool levelEditorActive; /***< If true the levelEditor will be shown*/
	bool preferencesActive; /***< If true the preferences will be shown*/

	Entity* selectedEntity; /***< The selected entity instance*/

	//Grid
	Grid* grid; /***< The grid instance*/
public:
	/**
	* Constructor
	*/
	Editor();

	/**
	* Update gets called every frame
	*/
	void Update();
};

#endif // !EDITOR_H