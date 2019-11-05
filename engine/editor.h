// Header file for editor class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#ifndef EDITOR_H
#define EDITOR_H

#define EDITOR_SCENE_SUFFIX ".scene"

#define EDITOR_LUA_LOAD_FUNCNAME "Initialize"

#include <vector>

#include "math/vec2.h"
#include "unique_types.h"

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
	* Destructor
	*/
	~Grid();
};

class EditorWindow {
public:
	bool active; /***< If true the editor window will be displayed*/

	/**
	* Constructor
	*/
	EditorWindow();

	/**
	* Handle gets called every frame when active
	*/
	virtual void Handle(class Editor* editor) = 0;
};

class EditorInputWindow : public EditorWindow {
private:
	const char* title; /***< The title of the input window*/
	char buffer[256] = ""; /***< Buffer holds string*/
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

class EditorGridSettings : public EditorWindow {
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

	/**
	* Handles the input
	*/
	void HandleInput();
public:
	static bool editorActive; /***If true the editor will be called by core*/
	Grid* grid; /***< Grid reference*/

	//Instance pointers
	class Entity* currentSelectedEntity = nullptr;

	bool bHoldingEntity; /***< If true the cursor is holding a entity*/

	//Key Combos
	std::vector<struct KeyComboEvent> combos;

	/**
	* Sets current selected entity if in bounds
	*/
	void SetCurrentSelectedEntityByPosition(class Entity* parent, Vec2 pos);

	/**
	* Adds a window instance to the windows list
	* @param EditorWindow*
	*/
	static void AddEditorWindow(EditorWindow* window);

	/**
	* Checks if any window of type is active (open atm)
	* @return bool
	*/
	template <typename T>
	static bool AnyWindowOfTypeActive() {
		for (size_t i = 0; i < GetInstance()->windows.size(); i++) {
			if (dynamic_cast<T*>(GetInstance()->windows[i])) return true;
		}
		return false;
	}

	/**
	* Returns the entity if on the tile, iterates through all children of given enity parameter until found, else return nullptr
	* Note that this is a heavy operation, should not be executed every frame
	* @param GridTile* tile
	* @param Entity* entity
	* @return Entity*	Entity occupying the tile
	*/
	class Entity* GetEntityOnTile(GridTile* tile, class Entity* entity);

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