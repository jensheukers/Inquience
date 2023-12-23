// Main entry point.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers
#include "wb_editor.h"

//Includes
#include <core.h>
#include <camera.h>
#include <parser.h>
#include <imgui.h>
#include <Windows.h>

#include <input.h>
#include <math/physics.h>

Grid::Grid() {
	active = true;
}

void Grid::Construct(Vec2 size, Vec2 tileSize) {
	this->size = size;
	this->tileSize = tileSize;
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

Scene* WB_Editor::LoadScene(std::string path) {
	Scene* scene = Core::GetSceneManager()->ReadFromFileAndSwap(path);

	//Make sure to create a new camera as we are in the editor
	camera = new Camera();
	scene->SetActiveCamera(camera);

	return scene;
}

Scene* WB_Editor::NewScene() {
	currentScene = Core::GetSceneManager()->SetActiveScene(new Scene());
	//setup camera
	camera = new Camera();
	currentScene->SetActiveCamera(camera);

	return currentScene;
}

std::string WB_Editor::OpenFileName(char* filter, HWND owner) {
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrInitialDir = Core::GetExecutableDirectoryPath().c_str();
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";

	std::string fileNameStr = "";

	if (GetOpenFileName(&ofn))
		fileNameStr = fileName;

	return fileNameStr.erase(0, Core::GetExecutableDirectoryPath().length());
}

std::string WB_Editor::SaveFileName(char* filter, HWND owner) {
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrInitialDir = Core::GetExecutableDirectoryPath().c_str();
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";

	std::string fileNameStr = "";

	if (GetSaveFileName(&ofn)) fileNameStr = fileName;;

	return fileNameStr.erase(0, Core::GetExecutableDirectoryPath().length());
}


WB_EditorWindow* WB_Editor::AddEditorWindow(WB_EditorWindow* w) {
	activeEditorWindows.push_back(w);
	return w;
}

WB_EditorWindow* WB_Editor::RemoveEditorWindow(WB_EditorWindow* w) {
	for (size_t i = 0; i < activeEditorWindows.size(); i++) {
		if (activeEditorWindows[i] == w) {
			return RemoveEditorWindow(i);
		}
	}

	return nullptr;
}

WB_EditorWindow* WB_Editor::RemoveEditorWindow(int i) {
	WB_EditorWindow* w = activeEditorWindows[i];
	activeEditorWindows.erase(activeEditorWindows.begin() + i);
	return w;
}


WB_Editor::WB_Editor() {
	camera = new Camera();

	//Create new empty scene
	currentScene = NewScene();

	grid = new Grid();

	//Todo: Fetch grid settings
	grid->Construct(Vec2(2048), Vec2(32));

	selectedEntity = nullptr;

	//Setup key combo events
	KeyComboEvent copySelectedEvent = KeyComboEvent(KeyCombo{ KeyEvent(KEYCODE_LEFT_CONTROL, KeyEvent_Type::Get), KeyEvent(KEYCODE_V, KeyEvent_Type::GetDown) });
	copySelectedEvent.onActivate.AddLambda([=]() {
		if (!selectedEntity) return;
		Entity* copy = new Entity(*selectedEntity);
		selectedEntity->GetParent()->AddChild(copy);

		//Set position to mouse position
		copy->position = grid->GetGridTile(Input::GetMousePosition() + Core::GetSceneManager()->GetActiveScene()->GetActiveCamera()->position)->position;
		selectedEntity = copy;
	});
	combos.push_back(copySelectedEvent);

	KeyComboEvent deleteSelectedEvent = KeyComboEvent(KeyCombo{ KeyEvent(KEYCODE_DELETE, KeyEvent_Type::GetDown) });
	deleteSelectedEvent.onActivate.AddLambda([=]() {
		if (!selectedEntity) return;
		selectedEntity->GetParent()->RemoveChild(selectedEntity);
		delete selectedEntity;
		selectedEntity = nullptr;
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
		selectedEntity = nullptr;
	});
	combos.push_back(deselectSelected);

	this->scalemode = Editor_ScaleMode::Both;
}

void WB_Editor::HandleInput() {
	for (size_t i = 0; i < combos.size(); i++) {
		combos[i].Check();
	}

	Camera* camera = Core::GetSceneManager()->GetActiveScene()->GetActiveCamera();
	//Camera input
	if (Input::GetKey(KEYCODE_KP_2)) {
		camera->position = camera->position + Vec2(0, cameraMovementSpeed);
	}

	if (Input::GetKey(KEYCODE_KP_4)) {
		camera->position = camera->position + Vec2(-cameraMovementSpeed, 0);
	}

	if (Input::GetKey(KEYCODE_KP_6)) {
		camera->position = camera->position + Vec2(cameraMovementSpeed, 0);
	}

	if (Input::GetKey(KEYCODE_KP_8)) {
		camera->position = camera->position + Vec2(0, -cameraMovementSpeed);
	}

	if (Input::GetKey(KEYCODE_MINUS) && Core::GetSceneManager()->GetActiveScene()) {
		Core::GetSceneManager()->GetActiveScene()->GetActiveCamera()->SetZoomScale(
			Core::GetSceneManager()->GetActiveScene()->GetActiveCamera()->GetZoomScale() - 0.1f);
	}

	if (Input::GetKey(KEYCODE_EQUAL)) {
		Core::GetSceneManager()->GetActiveScene()->GetActiveCamera()->SetZoomScale(
			Core::GetSceneManager()->GetActiveScene()->GetActiveCamera()->GetZoomScale() + 0.1f);
	}

	//Mouse position
	Vec2 mousePos = Input::GetMousePosition() + Core::GetSceneManager()->GetActiveScene()->GetActiveCamera()->position;

	if (Input::GetButtonDown(BUTTONCODE_MIDDLE)) {
		selectedEntity = GetEntityOnTile(grid->GetGridTile(mousePos), Core::GetSceneManager()->GetActiveScene());
	}

	//Shortcuts
	if (Input::GetKeyDown(KEYCODE_G)) grid->active = !grid->active;

	if (AnyWindowFocused() || GetSelectedEntity() == nullptr)
		return;

	if (Input::GetButtonDown(BUTTONCODE_LEFT)) {
		GetSelectedEntity()->position = mousePos;
	}
}



void WB_Editor::Update() {
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File")) {
		if (ImGui::MenuItem("New Scene")) {
			NewScene();
		}

		if (ImGui::MenuItem("Load Scene")) {
			std::string path = OpenFileName();
			if (path != "") LoadScene(path);
		}

		if (ImGui::MenuItem("Save As")) {
			std::string path = SaveFileName();
			if (path != "") {
				Parser* parser = new Parser(path, false, true);
				parser->WriteSceneToFile(currentScene);
				delete parser;
			}
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("View")) {
		if (ImGui::MenuItem("Painter")) {
			if (!AnyWindowOfTypeActive<WB_PainterWindow>()) {
				AddEditorWindow(new WB_PainterWindow());
			}
		}
		
		if (ImGui::MenuItem("Hierarchy")) {
			if (!AnyWindowOfTypeActive<WB_EditorHierarchy>()) {
				AddEditorWindow(new WB_EditorHierarchy());
			}
		}

		if (ImGui::MenuItem("Resource Browser")) {
			if (!AnyWindowOfTypeActive<WB_EditorResourceBrowser>()) {
				AddEditorWindow(new WB_EditorResourceBrowser());
			}
		}

		if (ImGui::MenuItem("Viewport")) {
			if (!AnyWindowOfTypeActive<WB_EditorViewport>()) {
				AddEditorWindow(new WB_EditorViewport());
			}
		}

		if (ImGui::MenuItem("Inspector")) {
			if (!AnyWindowOfTypeActive<WB_EditorInspector>()) {
				AddEditorWindow(new WB_EditorInspector());
			}
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Settings")) {
		if (ImGui::MenuItem("Grid Settings"))  AddEditorWindow(new WB_EditorGridSettingsWindow()); 
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	//Update editor windows
	for (int i = this->activeEditorWindows.size() - 1; i >= 0; i--) {
		if (this->activeEditorWindows[i]->active && Core::GetSceneManager()->GetActiveScene()) {
			this->activeEditorWindows[i]->Update(this);
		}
		else {
			delete this->activeEditorWindows[i];
			this->activeEditorWindows.erase(this->activeEditorWindows.begin() + i);
		}
	}


	if (Core::GetSceneManager()->GetActiveScene()) {
		//Only execute when there is no window focused
		if (!ImGui::IsAnyWindowFocused()) {
			this->HandleInput();
		}
	}

	//Display mouse pos
	Vec2 worldMousePos = camera->position + Input::GetMousePosition();
	Debug::DrawTextLine("(" + std::to_string((int)worldMousePos.x) + ", " + std::to_string((int)worldMousePos.y) + ")", Vec2(0, 35), 0.25f);

	//Display selectedEntity name
	if (selectedEntity) { 
		Debug::DrawTextLine(selectedEntity->tag, Vec2(0, 55), 0.25f);
	}
	else {
		Debug::DrawTextLine("No selection", Vec2(0, 55), 0.25f);
	}

	//Draw grid lines if grid is active
	if (grid->active) {
		//Draw vertical lines
		for (size_t x = 0; x <= grid->size.x; x += grid->tileSize.x) {
			Debug::DrawLine(Vec2(x, 0), Vec2(x, grid->size.y));
		}

		//Draw horizontal lines
		for (size_t y = 0; y <= grid->size.y; y += grid->tileSize.y) {
			Debug::DrawLine(Vec2(0, y), Vec2(grid->size.x, y));
		}

	}

	//Draw around selected
	if (selectedEntity) {
		Debug::DrawCube(selectedEntity->GetGlobalPosition(), selectedEntity->GetGlobalPosition() + selectedEntity->scale, glm::vec3(0, 0, 255));
	}
}

Entity* WB_Editor::GetEntityOnTile(GridTile* tile, Entity* entity) {
	if (entity != Core::GetSceneManager()->GetActiveScene() && tile->position == entity->GetGlobalPosition()) return entity;

	for (size_t i = 0; i < entity->GetChildren().size(); i++) {
		if (Entity* child = this->GetEntityOnTile(tile, entity->GetChildren()[i])) {
			return child;
		}
	}

	return nullptr;
}

WB_Editor::~WB_Editor() {
	delete this->grid;

	//Delete editor windows instances
	for (size_t i = 0; i < this->activeEditorWindows.size(); i++) {
		delete this->activeEditorWindows[i];
		this->activeEditorWindows.erase(this->activeEditorWindows.begin() + i);
	}
}