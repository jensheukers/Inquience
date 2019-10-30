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

struct Gizmos {
	void Update(Vec2 origin, float lenght);
};

struct GridTile {
	Vec2 position;
	Vec2 bounds;
};

struct Grid {
	std::vector<GridTile*> gridTiles;

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
	GridTile* GetGridTile(Vec2 position);

	/**
	* Clears the grid
	*/
	void Clear();
};

//Editor is a singleton instance
class Editor {
private:
	static Editor* instance;
	Gizmos gizmos;
	Grid grid;

	//Menu activator booleans
	bool loadMenuActive = false;
	bool saveMenuActive = false;

	bool hierarchyActive = false;
	bool inspectorActive = false;

	bool createEntityActive = false;
	bool tileMapCreationActive = false;

	bool addComponentActive = false;

	bool renameEntityActive = false;

	//Instance pointers
	class Entity* currentSelectedEntity = nullptr;
	class Entity* referenceEntity = nullptr;

	bool bHoldingEntity;
	bool bSnapToGrid;

	/**
	* Constructor
	*/
	Editor();

	/**
	* Returns the editor instance
	*/
	static Editor* GetInstance();

	/**
	* Sets current selected entity if in bounds
	*/
	void SetCurrentSelectedEntityByPosition(Entity* parent, Vec2 pos);

	/**
	* Construct a treenode of entities
	*/
	void ConstructTreenode(class Entity* entity);

	/**
	* Handles all file tab relation menus
	*/
	void HandleFileMenus();

	/**
	* Handles all the view menu's
	*/
	void HandleViewMenus();

	/**
	* Handles all entity menus
	*/
	void HandleEntityMenus();
public:
	static bool editorActive;

	/**
	* Update gets called every frame by core
	*/
	static void Update();
	
	~Editor();
};


#endif // !EDITOR_H