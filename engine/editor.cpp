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

#include "component_register.h"

#include "math/physics.h"

#include "components/sprite.h"

void Gizmos::Update(Vec2 origin, float lenght) {
	/* Implement */
}

void Grid::Construct(Vec2 size, Vec2 tileSize) {
	for (int x = 0; x < (int)size.x; x += (int)tileSize.x) {
		for (int y = 0; y < (int)size.y; y += (int)tileSize.y) {
			GridTile* tile = new GridTile();
			tile->position = Vec2(x, y);
			tile->bounds = Vec2(x + tileSize.x, y + tileSize.y);

			gridTiles.push_back(tile);
		}
	}
}

GridTile* Grid::GetGridTile(Vec2 position) {
	for (size_t i = 0; i < gridTiles.size(); i++) {
		if (Physics::InBounds(position, gridTiles[i]->position, gridTiles[i]->bounds)) {
			return gridTiles[i];
		}
	}
	return nullptr;
}

void Grid::Clear() {
	for (size_t i = 0; i < gridTiles.size(); i++) {
		delete gridTiles[i];
		gridTiles.erase(gridTiles.begin() + i);
	}
}

Editor* Editor::instance;

Editor::Editor() {
	//Todo: Fetch grid settings
	grid.Construct(Vec2(512), Vec2(32));
}

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
			ImGui::Text("Components:"); ImGui::SameLine();
			if (ImGui::Button("Add")) { addComponentActive = true; };
			ImGui::BeginChild("Components");
			for (size_t i = 0; i < currentSelectedEntity->GetComponents().size(); i++) {
				Component* component = currentSelectedEntity->GetComponents()[i];
				ImGui::Text(component->GetName().c_str()); ImGui::SameLine();
				if (ImGui::Button(std::string("Properties" + std::string("##") + std::to_string(i)).c_str())) {
					component->bShowComponentProperties = true;
				} ImGui::SameLine();

				if (ImGui::Button(std::string("Remove" + std::string("##") + std::to_string(i)).c_str())) {
					component->bShowComponentProperties = false;
					currentSelectedEntity->RemoveComponent(component);
				}

				if (component->bShowComponentProperties) {
					ImGui::Begin((currentSelectedEntity->tag + " - Component: " + component->GetName() + " - Properties").c_str(), &component->bShowComponentProperties);
					component->OnComponentPropertiesEditor();
					ImGui::End();
				}
			}
			ImGui::EndChild();

			//Activate Menus
			if (addComponentActive) {
				ImGui::Begin("Add Component", &addComponentActive);
				ImGui::Text("Control + Left Click to add component to current entity");
				ImGui::ListBoxHeader("Registered Components");
				for (size_t i = 0; i < Component_Register::GetAvailableKeys().size(); i++) {
					std::string& item_name = Component_Register::GetAvailableKeys()[i];
					if (ImGui::Selectable(Component_Register::GetAvailableKeys()[i].c_str()) && Input::GetKey(KEYCODE_LEFT_CONTROL)) {
						if (Component* component = Component_Register::GetNewComponentInstance(Component_Register::GetAvailableKeys()[i])) {
							if (!currentSelectedEntity->HasComponent(component)) {
								currentSelectedEntity->AddExistingComponentInstance(component);
							}
							else {
								delete component;
							}
						}
					}
				}
				ImGui::ListBoxFooter();
				ImGui::End();
			}
		}
		else {
			ImGui::Text("No entity selected");
		}

		ImGui::End();
	}
}

void Editor::HandleEntityMenus() {
	if (createEntityActive) {
		ImGui::Begin("Create Entity", &createEntityActive);
		if (!SceneManager::GetActiveScene()) {
			ImGui::Text("No active scene");
			ImGui::End();
			return;
		}


		static float position[2];
		static float scale[2] = { 32, 32};
		static bool parentToSelected;

		ImGui::InputFloat2("Position", position);
		ImGui::InputFloat2("Scale", scale);
		ImGui::Checkbox("Parent to selected", &parentToSelected);

		if (ImGui::Button("Create")) {
			Entity* entity = new Entity();

			entity->localPosition = Vec2(position[0], position[1]);
			entity->localScale = Vec2(scale[0], scale[1]);

			if (parentToSelected && currentSelectedEntity) {
				currentSelectedEntity->AddChild(entity);
			}
			else {
				SceneManager::GetActiveScene()->AddChild(entity);
			}

			currentSelectedEntity = entity;
			createEntityActive = false;
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

	if (ImGui::BeginMenu("Entity")) {
		if (ImGui::MenuItem("New Entity")) { GetInstance()->createEntityActive = true; }

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	//Handle menus
	GetInstance()->HandleFileMenus();
	GetInstance()->HandleViewMenus();
	GetInstance()->HandleEntityMenus();

	//Update gizmos and draw visual
	if (GetInstance()->currentSelectedEntity) {
		GetInstance()->gizmos.Update(Vec2(500, 200), 128);

		//Draw around selected entity for visual
		Debug::DrawCube(GetInstance()->currentSelectedEntity->GetPosition(), GetInstance()->currentSelectedEntity->GetPosition() + GetInstance()->currentSelectedEntity->GetScale(), glm::vec3(1, 0, 0));
	}

	//Handle Input

	//Set active object by clicking on any entity
	if (SceneManager::GetActiveScene() && Input::GetButtonDown(BUTTONCODE_LEFT)) {
		GetInstance()->SetCurrentSelectedEntityByPosition(SceneManager::GetActiveScene(), Input::GetMousePosition() + SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition());
	}

	if (GetInstance()->bHoldingEntity && GetInstance()->currentSelectedEntity) {
		Vec2 mousePos = (Input::GetMousePosition() + SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition()) - GetInstance()->currentSelectedEntity->GetParent()->GetPosition();

		GetInstance()->bSnapToGrid = true;
		if (GetInstance()->bSnapToGrid && GetInstance()->grid.GetGridTile(mousePos)) {
			GetInstance()->currentSelectedEntity->localPosition = GetInstance()->grid.GetGridTile(mousePos)->position;
		}
		else {
			GetInstance()->currentSelectedEntity->localPosition = (mousePos - GetInstance()->currentSelectedEntity->GetScale() / 2);
		}
	}

	if (Input::GetButtonUp(BUTTONCODE_LEFT)) {
		GetInstance()->bHoldingEntity = false;
	}

	//Current entity specific
	if (GetInstance()->currentSelectedEntity) {
		//Delete
		if (Input::GetKeyDown(KEYCODE_DELETE)) {
			GetInstance()->currentSelectedEntity->GetParent()->RemoveChild(GetInstance()->currentSelectedEntity);
			delete GetInstance()->currentSelectedEntity;
		}

		//Copy current selected
		if (Input::GetKeyDown(KEYCODE_V)) {

		}

		//Enable disable snapping
		if (Input::GetKeyDown(KEYCODE_S)) {
			if (GetInstance()->bSnapToGrid) { 
				GetInstance()->bSnapToGrid = false;
			}
			else {
				GetInstance()->bSnapToGrid = true;
			}
		}
	}
}