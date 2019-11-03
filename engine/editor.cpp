// Source file for editor class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
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
		if (ImGui::Button("Save as .asset")) {
			EditorInputWindow* window = new EditorInputWindow("Save Asset");
			window->onApply.AddLambda([=]() {
				editor->currentSelectedEntity->WriteToLuaFile(LuaScriptFile(std::string(window->GetBuffer()) + EDITOR_ENTITY_SUFFIX), EDITOR_LUA_LOAD_FUNCNAME);
			});

			editor->AddEditorWindow(window);
		}

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

void EditorTileEdit::Handle(Editor* editor) {
	//Enable snapToGrid
	editor->bSnapToGrid = true;

	ImGui::Begin("Tile Edit", &this->active);
	//Texture pointer
	static Texture* texture;

	//UvCoordinate Holder
	static UV uv;

	//Variables
	static float imageSize = 256; // Initialize at 256 as expected image size
	static int tileSize = 32; // Initialize at DEFAULT_TILE_SIZE
	static int objectScale = 32;
	static bool childToCurrentSelection = false;

	static char buffer[128]; // Allocate buffer
	ImGui::InputText("Path", buffer, sizeof(buffer));
	ImGui::SameLine();
	if (ImGui::Button("Load")) {
		Texture* t = TextureLoader::LoadTarga(buffer);

		if (t->textureData->width != t->textureData->height) {
			Debug::Log("Error: cannot load tilemap as width != height");
		}
		else {
			texture = t;
			imageSize = (float)texture->textureData->width;
		}
	}

	ImGui::InputInt("Tile Size", &tileSize);
	ImGui::InputInt("Object Spawn Scale", &objectScale);
	ImGui::Checkbox("Child to currently selected Entity", &childToCurrentSelection);

	int tiles = (int)imageSize / tileSize;

	//tileSetImagePosition
	ImVec2 tilesetImagePos = ImGui::GetCursorScreenPos();

	if (texture) {
		unsigned glTexId = texture->_glTexture;
		ImGui::Image((void*)(intptr_t)glTexId, ImVec2(imageSize, imageSize), ImVec2(0, 1), ImVec2(1, 0));
	}

	// draw grid
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	// draw horizontal lines
	for (int x = 0; x < tiles + 1; ++x) {
		draw_list->AddLine(ImVec2(tilesetImagePos.x + x * tileSize, tilesetImagePos.y),
			ImVec2(tilesetImagePos.x + x * tileSize, tilesetImagePos.y + tiles * tileSize),
			ImColor(255, 255, 255));
	}

	// draw vertical lines
	for (int y = 0; y < tiles + 1; ++y) {
		draw_list->AddLine(ImVec2(tilesetImagePos.x, tilesetImagePos.y + y * tileSize),
			ImVec2(tilesetImagePos.x + tiles * tileSize, tilesetImagePos.y + y * tileSize),
			ImColor(255, 255, 255));
	}

	static int tileMapIndex = 0;

	if (!editor->referenceEntity->HasComponent<Sprite>()) {
		editor->referenceEntity->AddComponent<Sprite>();
	}

	//Input selection
	if (ImGui::IsItemHovered()) {
		if (Input::GetButtonDown(BUTTONCODE_LEFT)) {
			Vec2 relMousePos = Vec2(ImGui::GetMousePos().x - tilesetImagePos.x, ImGui::GetMousePos().y - tilesetImagePos.y);

			//Get the right uv coordinates.

			int _i = 0; // Tile index
			//Iterate through image
			for (int y = (int)imageSize - tileSize; y > -1; y -= tileSize) {
				for (int x = 0; x < imageSize; x += tileSize) {
					//Iterate through every pixel
					for (int py = 0; py <= tileSize; py++) {
						for (int px = 0; px <= tileSize; px++) {
							if ((x + px) == relMousePos.x && (y + py) == relMousePos.y) {
								editor->referenceEntity->GetComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga(buffer));
								editor->referenceEntity->GetComponent<Sprite>()->Split(tileSize, _i);
								tileMapIndex = _i;
							}
						}
					}
					_i++;
				}
			}
		}

		std::string tileMapIndexText = "Tile Index: " + std::to_string(tileMapIndex);
		ImGui::Text((char*)tileMapIndexText.c_str());
	}

	Vec2 mousePos = (Input::GetMousePosition() + SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition());
	if (Input::GetButton(BUTTONCODE_LEFT) && editor->bSnapToGrid) {
		if (editor->grid.GetGridTile(mousePos) && !editor->GetEntityOnTile(editor->grid.GetGridTile(mousePos), SceneManager::GetActiveScene()) && !ImGui::IsWindowFocused()) {
			Entity* entity = new Entity();
			entity->AddComponent<Sprite>();
			entity->GetComponent<Sprite>()->SetTexture(editor->referenceEntity->GetComponent<Sprite>()->GetTexture());
			entity->GetComponent<Sprite>()->uv = editor->referenceEntity->GetComponent<Sprite>()->uv;

			entity->localScale = Vec2(objectScale);

			entity->localPosition = editor->grid.GetGridTile(mousePos)->position;

			if (childToCurrentSelection && editor->currentSelectedEntity) {
				editor->currentSelectedEntity->AddChild(entity);
			}
			else {
				SceneManager::GetActiveScene()->AddChild(entity);
			}
		}
	}

	if (Input::GetButton(BUTTONCODE_RIGHT) && editor->bSnapToGrid) {
		if (GridTile* tile = editor->grid.GetGridTile(mousePos)) {
			if (Entity* entity = editor->GetEntityOnTile(tile, SceneManager::GetActiveScene())) {
				if (entity != nullptr) { delete entity->GetParent()->RemoveChild(entity); }
			}
		}
	}

	ImGui::End();

}

Editor* Editor::instance;

Editor::Editor() {
	//Todo: Fetch grid settings
	grid.Construct(Vec2(512), Vec2(32));

	//Create a reference entity
	this->referenceEntity = new Entity();

	//Input delegates
	KeyComboEvent setGridSnappingEvent = KeyComboEvent(KeyCombo{ KeyEvent(KEYCODE_LEFT_CONTROL, KeyEvent_Type::Get), KeyEvent(KEYCODE_S, KeyEvent_Type::GetDown)});
	setGridSnappingEvent.onActivate.AddLambda([=]() {
		if (this->bSnapToGrid) {
			this->bSnapToGrid = false;
		}
		else {
			this->bSnapToGrid = true;
		}
	});
	combos.push_back(setGridSnappingEvent);

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

	KeyComboEvent deleteSelectedEvent = KeyComboEvent(KeyCombo{ KeyEvent(KEYCODE_LEFT_CONTROL, KeyEvent_Type::Get), KeyEvent(KEYCODE_DELETE, KeyEvent_Type::GetDown) });
	deleteSelectedEvent.onActivate.AddLambda([=]() {
		if (!currentSelectedEntity) return;
		currentSelectedEntity->GetParent()->RemoveChild(GetInstance()->currentSelectedEntity);
		delete currentSelectedEntity;
	});
	combos.push_back(deleteSelectedEvent);
}

Editor* Editor::GetInstance() {
	if (!instance) {
		instance = new Editor();
	}
	return instance;
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

	if (ImGui::BeginMenu("Entity")) {
		if (ImGui::MenuItem("New Entity")) { AddEditorWindow(new EditorCreateEntityWizard()); }
		if (ImGui::MenuItem("Load Entity Asset")) {
			EditorInputWindow* window = new EditorInputWindow("Load Asset");
			window->onApply.AddLambda([=]() {
				LuaScript::RunFunction(std::string(window->GetBuffer()) + EDITOR_ENTITY_SUFFIX, EDITOR_LUA_LOAD_FUNCNAME);
			});

			GetInstance()->AddEditorWindow(window);
		}
		if (ImGui::MenuItem("Tile Edit")) { AddEditorWindow(new EditorTileEdit()); }

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	//Handle windows
	bool anyWindowActive = false;
	for (int i = GetInstance()->windows.size() - 1; i >= 0; i--) {
		if (GetInstance()->windows[i]->active) {
			GetInstance()->windows[i]->Handle(GetInstance());
			anyWindowActive = true;
		}
		else {
			delete GetInstance()->windows[i];
			GetInstance()->windows.erase(GetInstance()->windows.begin() + i);
		}
	}

	//Draw around selected entity for visual
	if (GetInstance()->currentSelectedEntity) {
		Debug::DrawCube(GetInstance()->currentSelectedEntity->GetPosition(), GetInstance()->currentSelectedEntity->GetPosition() + GetInstance()->currentSelectedEntity->GetScale(), glm::vec3(1, 0, 1));
	}

	//Handle Input
	if (!anyWindowActive) {
		for (size_t i = 0; i < GetInstance()->combos.size(); i++) {
			GetInstance()->combos[i].Check();
		}

		//Set active object by clicking on any entity
		if (SceneManager::GetActiveScene() && Input::GetKey(KEYCODE_LEFT_CONTROL) && Input::GetButtonDown(BUTTONCODE_LEFT)) {
			GetInstance()->SetCurrentSelectedEntityByPosition(SceneManager::GetActiveScene(), Input::GetMousePosition() + SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition());
		}

		if (GetInstance()->bHoldingEntity && GetInstance()->currentSelectedEntity) {
			Vec2 mousePos = (Input::GetMousePosition() + SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition()) - GetInstance()->currentSelectedEntity->GetParent()->GetPosition();

			if (GetInstance()->bSnapToGrid && GetInstance()->grid.GetGridTile(mousePos)) {
				GetInstance()->currentSelectedEntity->localPosition = GetInstance()->grid.GetGridTile(mousePos)->position;
			}
			else {
				GetInstance()->currentSelectedEntity->localPosition = (mousePos - GetInstance()->currentSelectedEntity->GetScale() / 2);
			}
		}
	}

	if (Input::GetButtonUp(BUTTONCODE_LEFT)) {
		GetInstance()->bHoldingEntity = false;
	}
}

Editor::~Editor() {
	if (this->referenceEntity) {
		delete this->referenceEntity;
	}
}