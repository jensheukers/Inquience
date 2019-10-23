// Source file for editor class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#include "editor.h"

//FIX INCLUDES
#include "../external/imgui/imgui.h"

#include "scenemanager.h"
#include "input.h"

#include "math/physics.h"

void Grid::Construct(Vec2 size, Vec2 tileSize) {
	for (int x = 0; x < (int)size.x; x += (int)tileSize.x) {
		for (int y = 0; y < (int)size.y; y += (int)tileSize.y) {
			GridTile tile;
			tile.position = Vec2(x, y);
			tile.bounds = Vec2(x + tileSize.x, y + tileSize.y);

			gridTiles.push_back(tile);
		}
	}
}

GridTile Grid::GetGridPosition(Vec2 position) {
	for (size_t i = 0; i < gridTiles.size(); i++) {
		if (Physics::InBounds(position, gridTiles[i].position, gridTiles[i].bounds)) {
			return gridTiles[i];
		}
	}
}

void Grid::Clear() {
	for (size_t i = 0; i < gridTiles.size(); i++) {
		gridTiles.erase(gridTiles.begin() + i);
	}
}

Editor* Editor::instance;
bool Editor::editorActive = false;

Editor* Editor::GetInstance() {
	if (!instance) {
		instance = new Editor();
	}
	return instance;
}

void Editor::Update() {
	//Handle events
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[KEYCODE_BACKSPACE] = Input::GetKey(KEYCODE_BACKSPACE);
	io.KeysDown[KEYCODE_ENTER] = Input::GetKeyDown(KEYCODE_ENTER);
	io.KeysDown[KEYCODE_DOWN] = Input::GetKeyDown(KEYCODE_DOWN);
	io.KeysDown[KEYCODE_UP] = Input::GetKeyDown(KEYCODE_UP);
	io.KeysDown[KEYCODE_LEFT] = Input::GetKeyDown(KEYCODE_LEFT);
	io.KeysDown[KEYCODE_RIGHT] = Input::GetKeyDown(KEYCODE_RIGHT);

	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File")) {
		if (ImGui::MenuItem("New")) { /**/ }
		if (ImGui::MenuItem("Load")) { /**/ }
		if (ImGui::MenuItem("Save")) { /**/ }
		if (ImGui::MenuItem("Exit")) { editorActive = false; }
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}