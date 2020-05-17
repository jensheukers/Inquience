// Source file for editor class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, April 2020
#include "editor.h"


#include "imgui.h"

#include "scenemanager.h"
#include "luascript.h"
#include "input.h"

#include "component_register.h"
#include "components/sprite.h"

#include "math/physics.h"

void Grid::Construct(Vec2 size, Vec2 tileSize) {
	for (int x = 0; x < (int)size.x; x += (int)tileSize.x) {
		for (int y = 0; y < (int)size.y; y += (int)tileSize.y) {
			GridTile* tile = new GridTile();
			tile->position = Vec2((float)x, (float)y);
			tile->bounds = Vec2((float)x + tileSize.x, (float)y + tileSize.y);

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

Grid::~Grid() {
	for (size_t i = 0; i < gridTiles.size(); i++) {
		delete gridTiles[i];
		gridTiles.erase(gridTiles.begin() + i);
	}
}

EditorWindow::EditorWindow() {
	this->active = true; // Always set to active
}

EditorInputWindow::EditorInputWindow(const char* title) {
	this->title = title;
}

void EditorInputWindow::Handle(Editor* editor) {
	ImGui::Begin(this->title, &this->active);
	ImGui::InputText("", this->buffer, sizeof(this->buffer));
	ImGui::SameLine();
	if (ImGui::Button("Apply")) {
		this->onApply.Execute();
		this->active = false;
	}
	ImGui::End();
}

void EditorTwoInputWindow::Handle(Editor* editor) {
	ImGui::Begin(this->title, &this->active);
	ImGui::InputText("##0", this->buffer, sizeof(this->buffer));
	ImGui::InputText("##1", this->buffer2, sizeof(this->buffer2));
	if (ImGui::Button("Apply")) {
		this->onApply.Execute();
		this->active = false;
	}
	ImGui::End();
}

void EditorHierarchy::ConstructTreenode(Editor* editor, Entity* entity) {
	if (ImGui::TreeNode((std::to_string(entity->uniqueId) + " " + entity->tag).c_str())) {
		if (SceneManager::GetActiveScene()) {
			if (ImGui::BeginPopupContextItem()) {
				if (entity != SceneManager::GetActiveScene()) {
					if (ImGui::MenuItem("Select")) {
						editor->currentSelectedEntity = entity;
					}

					if (ImGui::MenuItem("Copy")) {
						entity->GetParent()->AddChild(new Entity(*entity));
					}

					if (ImGui::MenuItem("Delete")) {
						entity->GetParent()->RemoveChild(entity);
						delete entity;
					}

					if (ImGui::MenuItem("Move Up")) {
						entity->GetParent()->MoveChildUp(entity);
					}

					if (ImGui::MenuItem("Move down")) {
						entity->GetParent()->MoveChildDown(entity);
					}
				}

				if (ImGui::MenuItem("Create Empty Child")) {
					entity->AddChild(new Entity());
				}

				if (ImGui::MenuItem("Parent selected to")) {
					if (entity != editor->currentSelectedEntity) {
						editor->currentSelectedEntity->GetParent()->RemoveChild(editor->currentSelectedEntity);
						entity->AddChild(editor->currentSelectedEntity);
					}
				}

				if (ImGui::MenuItem("Rename")) {
					editor->currentSelectedEntity = entity;

					EditorInputWindow* window = new EditorInputWindow();
					window->onApply.AddLambda([=]() {
						editor->currentSelectedEntity->tag = std::string(window->GetBuffer());
					});

					Editor::AddEditorWindow(window);
				}

				if (entity != SceneManager::GetActiveScene() && ImGui::MenuItem("Properties")) {
					editor->currentSelectedEntity = entity;
					
					if (!Editor::AnyWindowOfTypeActive<EditorInspector>()) Editor::AddEditorWindow(new EditorInspector());
				}

				if (ImGui::MenuItem("Export as .prefab")) {
					editor->currentSelectedEntity = entity;

					EditorInputWindow* window = new EditorInputWindow();
					window->onApply.AddLambda([=]() {
						editor->currentSelectedEntity->WriteToLuaFile(LuaScriptFile::LuaScriptFile(std::string(window->GetBuffer() + std::string(EDITOR_PREFAB_SUFFIX))), EDITOR_LUA_LOAD_FUNCNAME);
					});

					Editor::AddEditorWindow(window);
				}

				ImGui::EndPopup();
			}
		}

		for (size_t i = 0; i < entity->GetChildren().size(); i++) {
			ConstructTreenode(editor, entity->GetChildren()[i]);
		}

		ImGui::TreePop();
	}
}

void EditorHierarchy::Handle(Editor* editor) {
	ImGui::Begin("Hierarchy", &this->active);
	if (SceneManager::GetActiveScene()) {
		ConstructTreenode(editor, SceneManager::GetActiveScene());
	}

	ImGui::End();
}

void EditorInspector::Handle(Editor* editor) {
	ImGui::Begin("Inpector", &this->active);
	if (editor->currentSelectedEntity) {
		ImGui::Text(("Tag: " + editor->currentSelectedEntity->tag).c_str());
		ImGui::Spacing();
		ImGui::Text("Local Transformations:");

		static float position[2];
		position[0] = editor->currentSelectedEntity->localPosition.x;
		position[1] = editor->currentSelectedEntity->localPosition.y;
		ImGui::InputFloat2("Position", position);

		static float scale[2];
		scale[0] = editor->currentSelectedEntity->localScale.x;
		scale[1] = editor->currentSelectedEntity->localScale.y;
		ImGui::InputFloat2("Scale", scale);

		//Set
		editor->currentSelectedEntity->localPosition = Vec2(position[0], position[1]);
		editor->currentSelectedEntity->localScale = Vec2(scale[0], scale[1]);

		//Spawn components
		ImGui::Spacing();
		ImGui::Text("Components:"); ImGui::SameLine();
		if (ImGui::Button("Add")) { addComponentActive = true; };
		ImGui::BeginChild("Components");
		for (size_t i = 0; i < editor->currentSelectedEntity->GetComponents().size(); i++) {
			Component* component = editor->currentSelectedEntity->GetComponents()[i];
			ImGui::Text(component->GetName().c_str()); ImGui::SameLine();
			if (ImGui::Button(std::string("Properties" + std::string("##") + std::to_string(i)).c_str())) {
				component->bShowComponentProperties = true;
			} ImGui::SameLine();

			if (ImGui::Button(std::string("Remove" + std::string("##") + std::to_string(i)).c_str())) {
				component->bShowComponentProperties = false;
				editor->currentSelectedEntity->RemoveComponent(component);
			}

			if (component->bShowComponentProperties) {
				ImGui::Begin((editor->currentSelectedEntity->tag + " - Component: " + component->GetName() + " - Properties").c_str(), &component->bShowComponentProperties);
				component->OnComponentPropertiesEditor();
				ImGui::End();
			}
		}
		ImGui::EndChild();

		//Activate Menus
		if (addComponentActive) {
			ImGui::Begin("Add Component", &addComponentActive);
			ImGui::Text("Control + Left Click to add component to current entity");
			if (ImGui::ListBoxHeader("Registered Components")) {
				for (size_t i = 0; i < Component_Register::GetAvailableKeys().size(); i++) {
					std::string& item_name = Component_Register::GetAvailableKeys()[i];
					if (ImGui::Selectable(Component_Register::GetAvailableKeys()[i].c_str()) && Input::GetKey(KEYCODE_LEFT_CONTROL)) {
						if (Component * component = Component_Register::GetNewComponentInstance(Component_Register::GetAvailableKeys()[i])) {
							if (!editor->currentSelectedEntity->HasComponent(component)) {
								editor->currentSelectedEntity->AddExistingComponentInstance(component);
							}
							else {
								delete component;
							}
						}
					}
				}
				ImGui::ListBoxFooter();
			}
			ImGui::End();
		}
	}
	else {
		ImGui::Text("No entity selected");
	}

	ImGui::End();
}

void EditorCreateEntityWizard::Handle(Editor* editor) {
	ImGui::Begin("Create Entity", &this->active);
	if (!SceneManager::GetActiveScene()) {
		ImGui::Text("No active scene");
		ImGui::End();
		return;
	}

	static float position[2];
	static float scale[2] = { 32, 32 };
	static bool parentToSelected;

	ImGui::InputFloat2("Position", position);
	ImGui::InputFloat2("Scale", scale);
	ImGui::Checkbox("Parent to selected", &parentToSelected);

	if (ImGui::Button("Create")) {
		Entity* entity = new Entity();

		entity->localPosition = Vec2(position[0], position[1]);
		entity->localScale = Vec2(scale[0], scale[1]);

		if (parentToSelected && editor->currentSelectedEntity) {
			editor->currentSelectedEntity->AddChild(entity);
		}
		else {
			SceneManager::GetActiveScene()->AddChild(entity);
		}

		editor->currentSelectedEntity = entity;
		this->active = false;
	}

	ImGui::End();
}

void EditorKeyValuePairWizard::Handle(Editor* editor) {
	ImGui::Begin("Key Value Pairs", &this->active);

	if (!SceneManager::GetActiveScene()) {
		ImGui::Text("No active scene");
		ImGui::End();
		return;
	}

	std::vector<KeyValuePair> kvps = SceneManager::GetActiveScene()->GetKeyValuePairList();

	for (size_t i = 0; i < kvps.size(); i++) {
		ImGui::Text(("[Key: " + kvps[i].key + "] - [Value: " + kvps[i].value + "]").c_str());
		ImGui::SameLine();
		if (ImGui::Button("Delete")) {
			SceneManager::GetActiveScene()->RemoveKVP(kvps[i].key);
		}
	}

	// add a new KVP
	if (ImGui::Button("Add KVP")) {
		EditorTwoInputWindow* inputWindow = new EditorTwoInputWindow();
		inputWindow->onApply.AddLambda([=]() {
			SceneManager::GetActiveScene()->AddKVP(KeyValuePair(inputWindow->GetBuffer(), inputWindow->GetSecondBuffer()));
			});

		editor->AddEditorWindow(inputWindow);
	}
	ImGui::End();
}

void EditorGridSettings::Handle(Editor* editor) {
	ImGui::Begin("Grid Settings", &this->active);
	static float gridSizeValues[2];
	static float tileSizeValues[2];

	ImGui::InputFloat2("Grid Size", gridSizeValues);
	ImGui::InputFloat2("Tile Size", tileSizeValues);

	if (ImGui::Button("Construct")) {
		delete editor->grid;
		editor->grid = new Grid();
		editor->grid->Construct(Vec2(gridSizeValues[0], gridSizeValues[1]), Vec2(tileSizeValues[0], tileSizeValues[1]));
		this->active = false;
	}
	ImGui::End();
}

Editor* Editor::instance;

Editor::Editor() {
	grid = new Grid();

	//Todo: Fetch grid settings
	grid->Construct(Vec2(2048), Vec2(32));

	KeyComboEvent copySelectedEvent = KeyComboEvent(KeyCombo{ KeyEvent(KEYCODE_LEFT_CONTROL, KeyEvent_Type::Get), KeyEvent(KEYCODE_V, KeyEvent_Type::GetDown) });
	copySelectedEvent.onActivate.AddLambda([=]() {
		if (!currentSelectedEntity) return;
		Entity* copy = new Entity(*currentSelectedEntity);
		currentSelectedEntity->GetParent()->AddChild(copy);

		//Set position to mouse position
		copy->localPosition = Input::GetMousePosition() + SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition();
		currentSelectedEntity = copy;
	});
	combos.push_back(copySelectedEvent);

	KeyComboEvent deleteSelectedEvent = KeyComboEvent(KeyCombo{ KeyEvent(KEYCODE_DELETE, KeyEvent_Type::GetDown) });
	deleteSelectedEvent.onActivate.AddLambda([=]() {
		if (!currentSelectedEntity) return;
		currentSelectedEntity->GetParent()->RemoveChild(GetInstance()->currentSelectedEntity);
		delete currentSelectedEntity;
		currentSelectedEntity = nullptr;
	});
	combos.push_back(deleteSelectedEvent);

	KeyComboEvent setScalemodeEvent = KeyComboEvent(KeyCombo{ KeyEvent(KEYCODE_LEFT_CONTROL, KeyEvent_Type::Get), KeyEvent(KEYCODE_Q, KeyEvent_Type::GetDown) });
	setScalemodeEvent.onActivate.AddLambda([=]() {
		switch (this->scalemode) {
		case Editor_ScaleMode::Horizontal:
			this->scalemode = Editor_ScaleMode::Vertical;
			break;
		case Editor_ScaleMode::Vertical:
			this->scalemode = Editor_ScaleMode::Both;
			break;
		case Editor_ScaleMode::Both:
			this->scalemode = Editor_ScaleMode::Horizontal;
			break;
		default:
			this->scalemode = Editor_ScaleMode::Both;
			break;
		}
	});
	combos.push_back(setScalemodeEvent);

	KeyComboEvent deselectSelected = KeyComboEvent(KeyCombo{ KeyEvent(KEYCODE_ESCAPE, KeyEvent_Type::GetDown) });
	deselectSelected.onActivate.AddLambda([=]() {
		currentSelectedEntity = nullptr;
	});
	combos.push_back(deselectSelected);

	this->scalemode = Editor_ScaleMode::Both;
}

Editor* Editor::GetInstance() {
	if (!instance) {
		instance = new Editor();
	}
	return instance;
}

void Editor::HandleInput() {
	for (size_t i = 0; i < GetInstance()->combos.size(); i++) {
		GetInstance()->combos[i].Check();
	}

	//Mouse position
	Vec2 mousePos = Input::GetMousePosition() + SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition();

	//Mouse input
	if (Input::GetButtonDown(BUTTONCODE_LEFT)) {
		SetCurrentSelectedEntityByPosition(SceneManager::GetActiveScene(), mousePos);
	}

	if (currentSelectedEntity && bHoldingEntity && Input::GetButton(BUTTONCODE_RIGHT)) {
		currentSelectedEntity->localPosition = mousePos - (GetInstance()->currentSelectedEntity->GetScale() / 2);

		//Try to set on grid tile if possible
		if (GridTile* tile = grid->GetGridTile(currentSelectedEntity->localPosition)) {
			currentSelectedEntity->localPosition = tile->position;
		}

		std::string scaleModeString = "Scale Mode: ";

		//Scale the entity based on scrolling
		Vec2 extraScale;
		switch (this->scalemode) {
		case Editor_ScaleMode::Horizontal:
			extraScale = Vec2(Input::GetScrollOffset().y, 0);
			scaleModeString += "Horizontal";
			break;
		case Editor_ScaleMode::Vertical:
			extraScale = Vec2(0, Input::GetScrollOffset().y);
			scaleModeString += "Vertical";
			break;
		case Editor_ScaleMode::Both:
			extraScale = Vec2(Input::GetScrollOffset().y);
			scaleModeString += "Horizontal & Vertical";
			break;
		}

		currentSelectedEntity->localScale = currentSelectedEntity->localScale + extraScale;

		//Draw to screen
		Debug::DrawText(scaleModeString, Vec2(0, 100), 0.5f, glm::vec3(1, 0, 0));
		Debug::DrawText("Left Control + Q to change mode", Vec2(0, 150), 0.5f, glm::vec3(1, 0, 0));
	}
	else if (currentSelectedEntity) {
		Debug::DrawText("Right Click to modify position and scale", Vec2(0, 100), 0.5f, glm::vec3(1, 0, 0));
	}
	
	if (Input::GetButtonUp(BUTTONCODE_RIGHT)) {
		bHoldingEntity = false;
	}
}

bool Editor::editorActive = false;

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

void Editor::AddEditorWindow(EditorWindow* window) {
	GetInstance()->windows.push_back(window);
}

Entity* Editor::GetEntityOnTile(GridTile* tile, Entity* entity) {
	if (entity != SceneManager::GetActiveScene() && tile->position == entity->GetPosition()) return entity;

	for (size_t i = 0; i < entity->GetChildren().size(); i++) {
		if (Entity* child = this->GetEntityOnTile(tile, entity->GetChildren()[i])) {
			return child;
		}
	}

	return nullptr;
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
		if (ImGui::MenuItem("New")) { 
			if (SceneManager::GetActiveScene()) {
				delete SceneManager::GetActiveScene();
			}
			
			SceneManager::SetActiveScene(new Scene());
			SceneManager::GetActiveScene()->SetActiveCamera(new Camera());
		}
		if (ImGui::MenuItem("Load")) { 
			EditorInputWindow* window = new EditorInputWindow("Load");
			window->onApply.AddLambda([=]() {
				LuaScript::RunFunction(std::string(window->GetBuffer()) + EDITOR_SCENE_SUFFIX, EDITOR_LUA_LOAD_FUNCNAME);
			});

			GetInstance()->AddEditorWindow(window);
		}

		if (ImGui::MenuItem("Save")) { 
			EditorInputWindow* window = new EditorInputWindow("Save");
			window->onApply.AddLambda([=]() {
				SceneManager::GetActiveScene()->WriteToLuaFile(LuaScriptFile::LuaScriptFile(std::string(window->GetBuffer() + std::string(EDITOR_SCENE_SUFFIX))), EDITOR_LUA_LOAD_FUNCNAME);
			});

			GetInstance()->AddEditorWindow(window);
		}

		if (ImGui::MenuItem("Exit")) { editorActive = false; }
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("View")) {
		if (ImGui::MenuItem("Hierarchy")) { AddEditorWindow(new EditorHierarchy()); }
		if (ImGui::MenuItem("Inspector")) { AddEditorWindow(new EditorInspector()); }
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Scene")) {
		if (ImGui::MenuItem("New Entity")) { AddEditorWindow(new EditorCreateEntityWizard()); }
		if (ImGui::MenuItem("KVP Wizard")) { AddEditorWindow(new EditorKeyValuePairWizard()); }
		if (ImGui::MenuItem("New Prefab")) {
			EditorInputWindow* window = new EditorInputWindow();
			window->onApply.AddLambda([=]() {
				LuaScript::RunFunction(std::string(window->GetBuffer()) + EDITOR_PREFAB_SUFFIX, EDITOR_LUA_LOAD_FUNCNAME);
			});

			Editor::AddEditorWindow(window);
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Settings")) {
		if (ImGui::MenuItem("Grid Settings")) { AddEditorWindow(new EditorGridSettings()); }
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	//Handle windows
	for (int i = GetInstance()->windows.size() - 1; i >= 0; i--) {
		if (GetInstance()->windows[i]->active && SceneManager::GetActiveScene()) {
			GetInstance()->windows[i]->Handle(GetInstance());
		}
		else {
			delete GetInstance()->windows[i];
			GetInstance()->windows.erase(GetInstance()->windows.begin() + i);
		}
	}
	if (SceneManager::GetActiveScene()) {
		//Draw all colliders
		std::vector<Collider*> colliders;
		SceneManager::GetActiveScene()->GetAllComponentsOfTypeInChildren(colliders);
		for (size_t i = 0; i < colliders.size(); i++) {
			colliders[i]->Update();
		}

		//Draw around current selected entity
		if (GetInstance()->currentSelectedEntity) {
			Debug::DrawCube(GetInstance()->currentSelectedEntity->GetPosition(),
				GetInstance()->currentSelectedEntity->GetPosition() + GetInstance()->currentSelectedEntity->GetScale(),
				glm::vec3(1, 0, 1));
		}

		//Only execute when there is no window focused
		if (!ImGui::IsAnyWindowFocused()) {
			GetInstance()->HandleInput();
		}
	}
}

Editor::~Editor() {
	delete this->grid;
}