// Main entry point.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers
#ifndef EDITOR_H
#define EDITOR_H

#include <Windows.h>

#include <vector>
#include <string>

#include <unique_types.h>

//Fwd declare
class Scene;
class Camera;

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
	virtual void Handle(class WB_Editor* editor) = 0;
};

class EditorInputWindow : public EditorWindow {
protected:
	const char* title; /***< The title of the input window*/
	char buffer[256] = ""; /***< Buffer holds string*/
public:
	struct Delegate onApply; /***< Gets executed when apply is clicked*/

	/**
	* Constructor
	*/
	EditorInputWindow(const char* title = "InputWindow");

	virtual void Handle(WB_Editor* editor) override;

	/**
	* Returns the buffer
	*/
	const char* GetBuffer() { return this->buffer; }
};

class EditorTwoInputWindow : public EditorInputWindow {
protected:
	char buffer2[256] = ""; /***< Buffer holds string*/
public:
	virtual void Handle(WB_Editor* editor) override;

	/**
	* Returns the second buffer
	*/
	const char* GetSecondBuffer() { return this->buffer2; }
};

class EditorHierarchy : public EditorWindow {
private:
	/**
	* Construct a treenode of entities
	*/
	void ConstructTreenode(WB_Editor* editor, class Entity* entity);
public:
	void Handle(WB_Editor* editor) override;
};

class EditorInspector : public EditorWindow {
private:
	bool addComponentActive;
public:
	void Handle(WB_Editor* editor) override;
};

class EditorCreateEntityWizard : public EditorWindow {
public:
	void Handle(WB_Editor* editor) override;
};

class EditorKeyValuePairWizard : public EditorWindow {
public:
	void Handle(WB_Editor* editor) override;
};

enum Editor_ScaleMode {
	Horizontal,
	Vertical,
	Both
};

class WB_Editor {
private:
	std::vector<EditorWindow*> activeEditorWindows; /** list containing editor window pointers*/
	Scene* currentScene;
	Camera* camera;

	Editor_ScaleMode scalemode;


	/**
	* @brief loads a scene from file
	* @param string path
	* @return Scene*
	*/
	Scene* LoadScene(std::string path);

	/**
	* @brief Creates a new empty scene
	* @return Scene*
	*/
	Scene* NewScene();

	/**
	* @brief Opens a window search dialog to and returns path
	* @param char* filters
	* @param HWND owner reference
	*/
	std::string OpenFileName(char* filter = "Supported Files(*.json)\0*.json;\0", HWND owner = NULL);

	/**
	* @brief Opens a window save dialog to and returns path
	* @param char* filters
	* @param HWND owner reference
	*/
	std::string SaveFileName(char* filter = "Supported Files(*.json)\0*.json;\0", HWND owner = NULL);

	/**
	* Handles the input
	*/
	void HandleInput();

public:
	//Instance pointers
	class Entity* currentSelectedEntity = nullptr;
	
	float cameraMovementSpeed = 5; /***< The movement speed of the camera*/

	bool bHoldingEntity; /***< If true the cursor is holding a entity*/

	//Key Combos
	std::vector<struct KeyComboEvent> combos;

	/**
	* @brief Constructor
	*/
	WB_Editor();

	/**
	* @brief Handles editor updates
	*/
	void Update();

	/**
	* @brief Adds a new editor window to the activeEditorWindows List
	* @param WB_EditorWindow* editor window instance
	* @return WB_EditorWindow*
	*/
	EditorWindow* AddEditorWindow(EditorWindow* w);

	/**
	* @brief Removes a editor window from the activeEditorWindows List
	* @param WB_EditorWindow* editor window instance
	* @return WB_EditorWindow*
	*/
	EditorWindow* RemoveEditorWindow(EditorWindow* w);

	/**
	* @brief Removes a editor window from the activeEditorWindows List
	* @param int index
	* @return WB_EditorWindow*
	*/
	EditorWindow* RemoveEditorWindow(int i);

	/**
	* @brief Checks if any window of type is active (open atm)
	* @return bool
	*/
	template <typename T>
	bool AnyWindowOfTypeActive() {
		for (size_t i = 0; i < this->activeEditorWindows.size(); i++) {
			if (dynamic_cast<T*>(activeEditorWindows[i])) return true;
		}
		return false;
	}

	/**
	* Sets current selected entity if in bounds
	*/
	void SetCurrentSelectedEntityByPosition(class Entity* parent, class Vec2 pos);
};

#endif // !EDITOR_H