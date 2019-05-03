// Source file for Editor class.
//
// Version: 3/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#include "editor.h"

//Include core.h and scenemanager.h and input.h and physics
#include "core.h"
#include "scenemanager.h"
#include "input.h"
#include "math/physics.h"

//Include GLM
#include <glm/glm.hpp>

void Grid::Construct() {
	//Empty the gridtiles list, if already filled
	for (int i = 0; i < gridTiles.size(); i++) {
		gridTiles.erase(gridTiles.begin(), gridTiles.begin() + i);
	}

	for (int x = 0; x <= scale.x * tileScale.x; x += tileScale.x) {
		for (int y = 0; y <= scale.y * tileScale.y; y += tileScale.y) {
			GridTile tile;
			tile.position = Vec2(x, y);
			tile.occupied = false;
			gridTiles.push_back(tile);
		}
	}
}

Vec2 Grid::GetTilePosition(Vec2 position) {
	for (int i = 0; i < gridTiles.size(); i++) {
		if (Physics::InBounds(position, gridTiles[i].position, gridTiles[i].position + tileScale)) {
			return gridTiles[i].position;
		}
	}

	return Vec2(0,0);
}

Editor::Editor() {
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
	colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
	colors[ImGuiCol_TabActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

	style->ChildRounding = 4.0f;
	style->FrameBorderSize = 1.0f;
	style->FrameRounding = 2.0f;
	style->GrabMinSize = 7.0f;
	style->PopupRounding = 2.0f;
	style->ScrollbarRounding = 12.0f;
	style->ScrollbarSize = 13.0f;
	style->TabBorderSize = 1.0f;
	style->TabRounding = 0.0f;
	style->WindowRounding = 4.0f;

	//Initialize local members
	loadMenuActive = false;
	saveMenuActive = false;
	
	levelEditorActive = false;
	preferencesActive = false;

	selectedEntity = nullptr;
	
	//Setup cursor Entity, add sprite
	cursorEntity = new Entity();
	cursorEntity->AddComponent<Sprite>();

	//Create Grid
	grid = new Grid();

	//Set default values
	grid->scale = Vec2(10, 10);
	grid->tileScale = Vec2(32, 32);
	grid->Construct();
}

void Editor::Update() {
	//Static variables
	static bool active;

	//Handle events
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[KEYCODE_BACKSPACE] = Input::GetKey(KEYCODE_BACKSPACE);
	io.KeysDown[KEYCODE_ENTER] = Input::GetKeyDown(KEYCODE_ENTER);
	io.KeysDown[KEYCODE_DOWN] = Input::GetKeyDown(KEYCODE_DOWN);
	io.KeysDown[KEYCODE_UP] = Input::GetKeyDown(KEYCODE_UP);
	io.KeysDown[KEYCODE_LEFT] = Input::GetKeyDown(KEYCODE_LEFT);
	io.KeysDown[KEYCODE_RIGHT] = Input::GetKeyDown(KEYCODE_RIGHT);

	//Add Cursor Entity if not a child
	if (SceneManager::GetActiveScene()) {
		if (!SceneManager::GetActiveScene()->HasChild(cursorEntity)) {
			SceneManager::GetActiveScene()->AddChild(cursorEntity);
		}
	}

	if (levelEditorActive) {
		//Set position of cursor entity
		Vec2 position = grid->GetTilePosition(Input::GetMousePosition());
		cursorEntity->localPosition = position;
	}

	//Menu bar
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New Scene")) {
			//If we have a currently loaded scene, delete it from memory + delete camera instance from memory
			if (SceneManager::GetActiveScene()) {
				if (SceneManager::GetActiveScene()->GetActiveCamera()) {
					delete SceneManager::GetActiveScene()->GetActiveCamera();
				}
				delete SceneManager::GetActiveScene();
			}

			//Create new empty scene, and set active, also create a default camera
			SceneManager::SetActiveScene(new Scene());
			SceneManager::GetActiveScene()->SetActiveCamera(new Camera());

			Debug::Log("Editor - New scene created!");
		}

		if (ImGui::MenuItem("Load Scene")) {
			loadMenuActive = true;
		}

		if (ImGui::MenuItem("Save Scene")) {
			saveMenuActive = true;
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Edit")) {
		if (ImGui::MenuItem("Hierarchy")) {

		}

		if (ImGui::MenuItem("Entity info")) {

		}

		if (ImGui::MenuItem("Level editor")) {
			levelEditorActive = true;
		}

		if (ImGui::MenuItem("Preferences")) {
			preferencesActive = true;
		}


		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	if (loadMenuActive) {
		ImGui::Begin("Load", &loadMenuActive);
		ImGui::End();
	}

	if (saveMenuActive) {
		ImGui::Begin("Save", &saveMenuActive);
		ImGui::End();
	}

	if (levelEditorActive) {
		ImGui::Begin("Level Editor", &levelEditorActive);

		ImGui::BeginTabBar("LevelEditorTabBar");

		if (ImGui::BeginTabItem("Tile Edit")) {
			
			static Texture* tileMapTexture;
			static int tileSize = 32;
			static int tileIndex = 0;

			static char buffer[128]; // Allocate buffer
			ImGui::InputText("Tilemap texture path", buffer, sizeof(buffer));
			ImGui::SameLine();
			if (ImGui::Button("Load")) {
				tileMapTexture = TextureLoader::LoadTarga(buffer);
			}

			ImGui::InputInt("Tile Size", &tileSize);
			ImGui::InputInt("Tile Index", &tileIndex);

			ImVec2 texSize = ImVec2(256, 256);
			unsigned glTexId = 0;

			if (tileMapTexture != nullptr) {
				texSize = ImVec2(tileMapTexture->textureData->width, tileMapTexture->textureData->height);
				glTexId = tileMapTexture->_glTexture;

				//Set cursorEntity sprite texture
				cursorEntity->GetComponent<Sprite>()->SetTexture(tileMapTexture);
				cursorEntity->GetComponent<Sprite>()->uvCoordinates = SpriteUV(cursorEntity->GetComponent<Sprite>(), tileSize, tileIndex);
			}
			ImGui::Text("Selected Tile:");
			ImGui::SameLine();

			ImGui::Image((void*)(intptr_t)glTexId, ImVec2(tileSize, tileSize), ImVec2(cursorEntity->GetComponent<Sprite>()->uvCoordinates.leftUp.x,
				cursorEntity->GetComponent<Sprite>()->uvCoordinates.leftUp.y), ImVec2(cursorEntity->GetComponent<Sprite>()->uvCoordinates.rightDown.x,
					cursorEntity->GetComponent<Sprite>()->uvCoordinates.rightDown.y));

			ImGui::Text("Tilemap:");
			ImGui::SameLine();
			ImGui::Image((void*)(intptr_t)glTexId, texSize, ImVec2(1, 1), ImVec2(0, 0));

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("World Edit")) {
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();

		ImGui::End();
	}

	if (preferencesActive) {
		ImGui::Begin("Preferences", &preferencesActive);
		ImGui::BeginTabBar("LevelEditorTabBar");
		if (ImGui::BeginTabItem("Grid")) {

			static int gridScale[2] = { 10, 10 };
			ImGui::InputInt2("Grid Size", gridScale);

			static int tileScale[2] = { 32, 32 };
			ImGui::InputInt2("Tile Size", tileScale);

			grid->scale = Vec2(gridScale[0], gridScale[1]);
			grid->tileScale = Vec2(tileScale[0], tileScale[1]);


			if (ImGui::Button("Apply")) {
				grid->Construct();
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
		ImGui::End();
	}
}