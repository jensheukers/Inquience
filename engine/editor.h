// Header file for editor class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#ifndef EDITOR_H
#define EDITOR_H

#include <vector>

#include "math/vec2.h"

struct GridTile {
	Vec2 position;
	Vec2 bounds;
};

struct Grid {
	std::vector<GridTile> gridTiles;

	/**
	* Constructs a grid
	* @param Vec2 size
	* @param Vec2 gridSize
	*/
	void Construct(Vec2 size, Vec2 tileSize);

	/**
	* Returns the gridtile depending on param
	* @param GridTile tile
	*/
	GridTile GetGridPosition(Vec2 position);

	/**
	* Clears the grid
	*/
	void Clear();
};

//Editor is a singleton instance
class Editor {
private:
	static Editor* instance;
	Grid grid;

	/**
	* Returns the editor instance
	*/
	static Editor* GetInstance();
public:
	static bool editorActive;

	/**
	* Update gets called every frame by core
	*/
	static void Update();
};


#endif // !EDITOR_H