// Source file for Editor class.
//
// Version: 6/8/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, August 2019
#include "editor.h"

//Include core.h and scenemanager.h and input.h and physics
#include "core.h"
#include "scenemanager.h"
#include "input.h"

//Include GLM
#include <glm/glm.hpp>

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

	//Initialize bools
	loadMenuActive = false;
	saveMenuActive = false;
	levelEditorActive = false;

	//Set grid pointer
	grid = Core::GetGrid();

	//Initialize reference
	selectionEntity = new Entity();
	selectionEntity->AddComponent<Sprite>(); // Add sprite component

	//Camera
	this->editorCamera = new Camera();
}

#include <iostream>

void Editor::Update() {
	//Set active camera to editor camera
	if (SceneManager::GetActiveScene()->GetActiveCamera() != this->editorCamera) {
		SceneManager::GetActiveScene()->SetActiveCamera(this->editorCamera);
	}

	//Camera controlls
	if (Input::GetKey(KEYCODE_KP_4)) {
		this->editorCamera->SetPosition(this->editorCamera->GetPosition() + Vec2(-Core::GetDeltaTime() * 100, 0));
	}

	if (Input::GetKey(KEYCODE_KP_6)) {
		this->editorCamera->SetPosition(this->editorCamera->GetPosition() + Vec2(Core::GetDeltaTime() * 100, 0));
	}

	if (Input::GetKey(KEYCODE_KP_8)) {
		this->editorCamera->SetPosition(this->editorCamera->GetPosition() + Vec2(0, -Core::GetDeltaTime() * 100));
	}

	if (Input::GetKey(KEYCODE_KP_2)) {
		this->editorCamera->SetPosition(this->editorCamera->GetPosition() + Vec2(0, Core::GetDeltaTime() * 100));
	}

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
			//Create a new scene and unload current scene from memory, also add a camera 
			delete SceneManager::GetActiveScene();
			SceneManager::SetActiveScene(new Scene());
			SceneManager::GetActiveScene()->SetActiveCamera(new Camera());

			//Clear the grid
			Core::GetGrid()->Clear();
		}
		if (ImGui::MenuItem("Load")) { loadMenuActive = true; }
		if (ImGui::MenuItem("Save")) { saveMenuActive = true; }
		ImGui::EndMenu();
	}

	if (loadMenuActive) {
		ImGui::Begin("Load", &loadMenuActive);
		static char buffer[128]; // Allocate buffer
		ImGui::InputText("Path", buffer, sizeof(buffer));
		ImGui::SameLine();
		if (ImGui::Button("Load")) {
			//Create a new scene and unload current scene from memory, also add a camera then load data
			delete SceneManager::GetActiveScene();
			SceneManager::SetActiveScene(new Scene());
			SceneManager::GetActiveScene()->SetActiveCamera(new Camera());
			SceneManager::GetActiveScene()->Load(buffer);
		}
		ImGui::End();
	}

	if (saveMenuActive) {
		ImGui::Begin("Save", &saveMenuActive);
		static char buffer[128]; // Allocate buffer
		ImGui::InputText("Path", buffer, sizeof(buffer));
		ImGui::SameLine();
		if (ImGui::Button("Save")) {
			//Save
			SceneManager::GetActiveScene()->Save(buffer);
		}
		ImGui::End();
	}

	if (ImGui::BeginMenu("Edit")) {
		if (ImGui::MenuItem("Level Editor")) { levelEditorActive = true; }
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	if (levelEditorActive) {
		ImGui::Begin("Level Editor", &levelEditorActive);
		ImGui::BeginTabBar("");
		if (ImGui::BeginTabItem("Tile Edit")) {
			//Texture pointer
			static Texture* texture;

			//UvCoordinate Holder
			static UV uv;
			
			//Variables
			static float imageSize = DEFAULT_TILEMAP_SIZE; // Initialize at 256 as expected image size
			static int tileSize = DEFAULT_TILE_SIZE; // Initialize at DEFAULT_TILE_SIZE

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
					imageSize = texture->textureData->width;
				}
			}

			ImGui::InputInt("Tile Size", &tileSize);

			int tiles = imageSize / tileSize;

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

			//Input selection
			if (ImGui::IsItemHovered()) {
				if (Input::GetButtonDown(BUTTONCODE_LEFT)) {
					Vec2 relMousePos = Vec2(ImGui::GetMousePos().x - tilesetImagePos.x, ImGui::GetMousePos().y - tilesetImagePos.y);

					//Get the right uv coordinates.
					
					int _i = 0; // Tile index
					//Iterate through image
					for (int y = imageSize - tileSize; y > -1; y -= tileSize) {
						for (int x = 0; x < imageSize; x += tileSize) {
							//Iterate through every pixel
							for (int py = 0; py <= tileSize; py++) {
								for (int px = 0; px <= tileSize; px++) {
									if ((x + px) == relMousePos.x && (y + py) == relMousePos.y) {
										selectionEntity->GetComponent<Sprite>()->SetTexture(TextureLoader::LoadTarga("res/terrain_tiles.tga"));
										selectionEntity->GetComponent<Sprite>()->Split(tileSize, _i);
									}
								}
							}
							_i++;
						}
					}
				}
			}

			//Placement
			if (Input::GetKey(KEYCODE_LEFT_CONTROL) && Input::GetButton(BUTTONCODE_LEFT) && selectionEntity->GetComponent<Sprite>()->GetTexture()) {
				GridTile* tile = grid->GetTilePosition(Input::GetMousePosition() + SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition());
				if (tile != nullptr && !tile->occupied) {
					Sprite* sprite = selectionEntity->GetComponent<Sprite>();

					//Create instance
					Entity* entity = new Entity();

					//Set position
					entity->localPosition = tile->position;

					//Sprite component
					entity->AddComponent<Sprite>()->SetTexture(sprite->GetTexture());
					entity->GetComponent<Sprite>()->Split(sprite->GetTexture()->textureData->width / sprite->slices, sprite->tileMapIndex);

					//Occupy and set tileEntity
					tile->occupied = true;
					tile->tileEntity = entity;

					//Add to scene
					SceneManager::GetActiveScene()->AddChild(entity);
				}
			}

			//Tile removal
			if (Input::GetKey(KEYCODE_LEFT_CONTROL) && Input::GetButton(BUTTONCODE_RIGHT)) {
				GridTile* tile = grid->GetTilePosition(Input::GetMousePosition() + SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition());

				if (tile != nullptr && tile->occupied) {
					Entity* entity = tile->tileEntity;

					//Delete entity
					delete SceneManager::GetActiveScene()->RemoveChild(entity);

					//Set tile properties
					tile->occupied = false;
					tile->tileEntity = nullptr;
				}
			}

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
		ImGui::End();
	}
}