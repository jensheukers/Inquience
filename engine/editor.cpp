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
		SetCurrentSelectedEntityByPosition(child, pos);
	}

	if (Physics::InBounds(pos, parent->GetPosition(), parent->GetPosition() + parent->GetScale()) && parent != SceneManager::GetActiveScene()) {
		currentSelectedEntity = parent;
		bHoldingEntity = true;
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
					if (entity != currentSelectedEntity) {
						currentSelectedEntity->GetParent()->RemoveChild(currentSelectedEntity);
						entity->AddChild(currentSelectedEntity);
					}
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
			ImGui::Text(("Tag: " + currentSelectedEntity->tag).c_str());
			ImGui::Spacing();
			ImGui::Text("Local Transformations:");

			static float position[2];
			position[0] = currentSelectedEntity->localPosition.x;
			position[1] = currentSelectedEntity->localPosition.y;
			ImGui::InputFloat2("Position", position);

			static float scale[2];
			scale[0] = currentSelectedEntity->localScale.x;
			scale[1] = currentSelectedEntity->localScale.y;
			ImGui::InputFloat2("Scale", scale);

			//Set
			currentSelectedEntity->localPosition = Vec2(position[0], position[1]);
			currentSelectedEntity->localScale = Vec2(scale[0], scale[1]);

			//Spawn components
			ImGui::Spacing();
			ImGui::Text("Components:");
			ImGui::BeginChild("Components");

			for (size_t i = 0; i < currentSelectedEntity->GetComponents().size(); i++) {
				Component* component = currentSelectedEntity->GetComponents()[i];
				ImGui::Text(component->GetName().c_str()); ImGui::SameLine();
				if (ImGui::Button(std::string("Properties" + std::string("##") + std::to_string(i)).c_str())) {
					component->bShowComponentProperties = true;
				} ImGui::SameLine();

				if (ImGui::Button("Remove")) {
					currentSelectedEntity->RemoveComponent(component);
				}

				if (component->bShowComponentProperties) {
					ImGui::Begin((currentSelectedEntity->tag + " - Component: " + component->GetName() + " - Properties").c_str(), &component->bShowComponentProperties);
					component->OnComponentPropertiesEditor();
					ImGui::End();
				}
			}

			ImGui::EndChild();
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
		GetInstance()->currentSelectedEntity->localPosition = ((mousePos - GetInstance()->currentSelectedEntity->GetParent()->GetPosition()) - GetInstance()->currentSelectedEntity->GetScale() / 2);
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