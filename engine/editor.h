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

#define DEFAULT_TILEMAP_SIZE 256
#define DEFAULT_TILE_SIZE 32

//Include vector
#include <vector>

//Include entity.h & vec2.h
#include "../external/imgui/imgui.h"
#include "../engine/math/vec2.h"

//Forward declare
class Entity;
class Camera;

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
	//Bools
	bool loadMenuActive;
	bool saveMenuActive;

	bool levelEditorActive;

	//References
	Entity* selectionEntity; /***< The entity used as a reference, will be copied when placed*/
	Grid* grid; /***< Reference to the grid instance*/

	Camera* editorCamera; /***< The editor camera instance, will be set active whenever editor is active*/
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