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
	return Core::GetSceneManager()->ReadFromFileAndSwap(path);
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

	if (GetSaveFileName(&ofn)) fileNameStr = fileName;

	return fileNameStr.erase(0, Core::GetExecutableDirectoryPath().length() - 2);
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

	//Mouse position
	Vec2 mousePos = Input::GetMousePosition() + Core::GetSceneManager()->GetActiveScene()->GetActiveCamera()->position;
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
			AddEditorWindow((WB_EditorWindow*)new WB_PainterWindow());
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
			this->activeEditorWindows[i]->Handle(this);
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

	//Draw grid lines
	//Draw vertical lines
	for (size_t x = 0; x <= grid->size.x; x += grid->tileSize.x) {
		Debug::DrawLine(Vec2(x, 0), Vec2(x, grid->size.y));
	}

	//Draw horizontal lines
	for (size_t y = 0; y <= grid->size.y; y += grid->tileSize.y) {
		Debug::DrawLine(Vec2(0, y), Vec2(grid->size.x, y));
	}

	Vec2 worldMousePos = camera->position + Input::GetMousePosition();
	Debug::DrawTextLine("(" + std::to_string((int)worldMousePos.x) + ", " + std::to_string((int)worldMousePos.y) + ")", Vec2(0, 50), 0.5f);
}

WB_Editor::~WB_Editor() {
	delete this->grid;
}