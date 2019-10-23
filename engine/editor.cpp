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
#include "luascript.h"
#include "input.h"

#include "math/physics.h"

#include "components/sprite.h"

void Gizmos::Update(Vec2 origin, float lenght) {
	/* Implement */
}

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

Editor* Editor::GetInstance() {
	if (!instance) {
		instance = new Editor();
	}
	return instance;
}

void Editor::SetCurrentSelectedEntityByPosition(Entity* parent, Vec2 pos) {
	for (size_t i = 0; i < parent->GetChildren().size(); i++) {
		Entity* child = parent->GetChildren()[i];
		if (Physics::InBounds(pos, child->GetPosition(), child->GetPosition() + child->GetScale())) {
			currentSelectedEntity = child;
			bHoldingEntity = true;
		}
	}
}

void Editor::ConstructTreenode(Entity* entity) {
	if (ImGui::TreeNode((std::to_string(entity->uniqueId) + " " + entity->tag).c_str())) {
		if (SceneManager::GetActiveScene()) {
			if (ImGui::BeginPopupContextItem()) {
				if (entity != SceneManager::GetActiveScene()) {
					if (ImGui::MenuItem("Select")) {
						currentSelectedEntity = entity;
					}
				}
				if (ImGui::MenuItem("Parent to")) {
					currentSelectedEntity->GetParent()->RemoveChild(currentSelectedEntity);
					entity->AddChild(currentSelectedEntity);
				}

				ImGui::EndPopup();
			}
		}

		for (size_t i = 0; i < entity->GetChildren().size(); i++) {
			ConstructTreenode(entity->GetChildren()[i]);
		}

		ImGui::TreePop();
	}
}

void Editor::HandleFileMenus() {
	if (GetInstance()->loadMenuActive) {
		ImGui::Begin("Load", &GetInstance()->loadMenuActive);
		static char buffer[128]; // Allocate buffer
		ImGui::InputText("Path", buffer, sizeof(buffer));
		ImGui::SameLine();
		if (ImGui::Button("Load")) {
			ImGui::Text(".lua file extension automaticly added");
			LuaScript::RunFunction(std::string(buffer + std::string(".lua")), std::string(buffer));
			GetInstance()->loadMenuActive = false;
		}
		ImGui::End();
	}

	if (GetInstance()->saveMenuActive) {
		ImGui::Begin("Save", &GetInstance()->saveMenuActive);
		static char buffer[128]; // Allocate buffer
		ImGui::InputText("Path", buffer, sizeof(buffer));
		ImGui::SameLine();
		if (ImGui::Button("Save")) {
			ImGui::Text(".lua file extension automaticly added");
			SceneManager::GetActiveScene()->WriteToLuaFile(LuaScriptFile::LuaScriptFile(std::string(buffer)), std::string(buffer));
			GetInstance()->saveMenuActive = false;
		}
		ImGui::End();
	}
}

void Editor::HandleViewMenus() {
	//Hierarchy
	if (GetInstance()->hierarchyActive) {
		ImGui::Begin("Hierarchy", &GetInstance()->hierarchyActive);
		if (SceneManager::GetActiveScene()) {
			ConstructTreenode(SceneManager::GetActiveScene());
		}
	
		ImGui::End();
	}

	//Inspector
	if (GetInstance()->inspectorActive) {
		ImGui::Begin("Inpector", &GetInstance()->inspectorActive);
		if (currentSelectedEntity) {
			ImGui::Text(currentSelectedEntity->tag.c_str());
		}
		else {
			ImGui::Text("No entity selected");
		}

		ImGui::End();
	}
}

bool Editor::editorActive = false;

void Editor::Update() {
	//Handle events
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[KEYCODE_BACKSPACE] = Input::GetKey(KEYCODE_BACKSPACE);
	io.KeysDown[KEYCODE_ENTER] = Input::GetKeyDown(KEYCODE_ENTER);
	io.KeysDown[KEYCODE_DOWN] = Input::GetKeyDown(KEYCODE_DOWN);
	io.KeysDown[KEYCODE_UP] = Input::GetKeyDown(KEYCODE_UP);
	io.KeysDown[KEYCODE_LEFT] = Input::GetKeyDown(KEYCODE_LEFT);
	io.KeysDown[KEYCODE_RIGHT] = Input::GetKeyDown(KEYCODE_RIGHT);

	//Set active object by clicking on any entity
	if (SceneManager::GetActiveScene() && Input::GetButtonDown(BUTTONCODE_LEFT)) {
		GetInstance()->SetCurrentSelectedEntityByPosition(SceneManager::GetActiveScene(), Input::GetMousePosition() + SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition());
	}

	if (GetInstance()->bHoldingEntity && GetInstance()->currentSelectedEntity) {
		Vec2 mousePos = Input::GetMousePosition() + SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition();
		GetInstance()->currentSelectedEntity->localPosition = mousePos - GetInstance()->currentSelectedEntity->GetScale() / 2;
	}

	if (Input::GetButtonUp(BUTTONCODE_LEFT)) {
		GetInstance()->bHoldingEntity = false;
	}

	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File")) {
		if (ImGui::MenuItem("New")) { 
			if (SceneManager::GetActiveScene()) {
				delete SceneManager::GetActiveScene();
			}
			
			SceneManager::SetActiveScene(new Scene());
			SceneManager::GetActiveScene()->SetActiveCamera(new Camera());
		}
		if (ImGui::MenuItem("Load")) { GetInstance()->loadMenuActive = true; }
		if (ImGui::MenuItem("Save")) { GetInstance()->saveMenuActive = true; }
		if (ImGui::MenuItem("Exit")) { editorActive = false; }
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("View")) {
		if (ImGui::MenuItem("Hierarchy")) { GetInstance()->hierarchyActive = true; }
		if (ImGui::MenuItem("Inspector")) { GetInstance()->inspectorActive = true; }
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	//Handle menus
	GetInstance()->HandleFileMenus();
	GetInstance()->HandleViewMenus();

	//Update gizmos
	if (GetInstance()->currentSelectedEntity) {
		GetInstance()->gizmos.Update(Vec2(500, 200), 128);
	}
}