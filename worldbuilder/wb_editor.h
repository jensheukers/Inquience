// Main entry point.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers
#ifndef WB_EDITOR_H
#define WB_EDITOR_H

#include <Windows.h>

#include <vector>
#include <string>

#include "wb_editorwindow.h"

//Fwd declare
class Scene;
class Camera;



class WB_Editor {
private:
	std::vector<WB_EditorWindow*> activeEditorWindows; /** list containing editor window pointers*/
	Scene* currentScene;
	Camera* camera;

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
	float cameraMovementSpeed = 5; /***< The movement speed of the camera*/

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
	WB_EditorWindow* AddEditorWindow(WB_EditorWindow* w);

	/**
	* @brief Removes a editor window from the activeEditorWindows List
	* @param WB_EditorWindow* editor window instance
	* @return WB_EditorWindow*
	*/
	WB_EditorWindow* RemoveEditorWindow(WB_EditorWindow* w);

	/**
	* @brief Removes a editor window from the activeEditorWindows List
	* @param int index
	* @return WB_EditorWindow*
	*/
	WB_EditorWindow* RemoveEditorWindow(int i);

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
};

#endif // !EDITOR_H