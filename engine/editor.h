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
#include "unique_types.h"

struct GridTile {
	Vec2 position;
	Vec2 bounds;
	class Entity* entity = nullptr;
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

class EditorWindow {
public:
	bool active; /***< If true the editor window will be displayed*/

	/**
	* Constructor
	*/
	EditorWindow();

	/**
	* Handle gets called every frame when active, base class handle should ALWAYS be called, to handle destruction
	*/
	virtual void Handle(class Editor* editor);
};

class EditorInputWindow : public EditorWindow {
private:
	const char* title; /***< The title of the input window*/
	char buffer[256]; /***< Buffer holds string*/
public:
	struct Delegate onApply; /***< Gets executed when apply is clicked*/

	/**
	* Constructor
	*/
	EditorInputWindow(const char* title = "InputWindow");

	void Handle(Editor* editor) override;

	/**
	* Returns the buffer
	*/
	const char* GetBuffer() { return this->buffer; }
};

class EditorHierarchy : public EditorWindow {
private:
	/**
	* Construct a treenode of entities
	*/
	void ConstructTreenode(Editor* editor, class Entity* entity);
public:
	void Handle(Editor* editor) override;
};

class EditorInspector : public EditorWindow {
private:
	bool addComponentActive;
public:
	void Handle(Editor* editor) override;
};

class EditorCreateEntityWizard : public EditorWindow {
public:
	void Handle(Editor* editor) override;
};

class EditorTileEdit : public EditorWindow {
public:
	void Handle(Editor* editor) override;
};

//Editor is a singleton instance
class Editor {
private:
	static Editor* instance;

	//Menus
	std::vector<EditorWindow*> windows;

	/**
	* Constructor
	*/
	Editor();

	/**
	* Returns the editor instance
	*/
	static Editor* GetInstance();
public:
	static bool editorActive; /***If true the editor will be called by core*/
	Grid grid; /***< Grid reference*/

	//Instance pointers
	class Entity* currentSelectedEntity = nullptr;
	class Entity* referenceEntity = nullptr;

	bool bHoldingEntity; /***< If true the cursor is holding a entity*/
	bool bSnapToGrid; /***< If true the current selected will try to snap to grid*/

	/**
	* Sets current selected entity if in bounds
	*/
	void SetCurrentSelectedEntityByPosition(class Entity* parent, Vec2 pos);

	/**
	* Update gets called every frame by core
	*/
	static void Update();
	

	/**
	* Destructor
	*/
	~Editor();
};


#endif // !EDITOR_H