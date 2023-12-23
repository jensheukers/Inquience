#include "wb_editorwindows.h"
#include <imgui.h>

#include <wb_editor.h>
#include <math/vec2.h>

#include <debug.h>
#include <input.h>
#include <core.h>
#include <component_register.h>
#include <parser.h>

WB_EditorWindow::WB_EditorWindow(const char* title, bool allowMove, Vec2 sizeDiv, Vec2 posDiv) {
	this->isFocused = false;
	this->active = true; // Always set to active
	this->title = title;
	this->allowMove = allowMove;
	this->sizeDiv = sizeDiv;
	this->posDiv = posDiv;
}

void WB_EditorWindow::Update(WB_Editor* editor) {
	ImGui::Begin(title, &this->active);

	//determine if focused
	if (ImGui::IsWindowFocused()) this->isFocused = true;

	Vec2 screenSize = Core::GetRenderer()->GetResolution();

	ImGui::SetWindowSize(ImVec2((float)screenSize.x / sizeDiv.x, (float)screenSize.y / sizeDiv.y));
	if (!allowMove) {
		float normalizedPositionX = Vec2::Lerp(Vec2(0,0), screenSize, posDiv.x).x;
		float normalizedPositionY = Vec2::Lerp(Vec2(0,0), screenSize, (posDiv.y + WB_EditorWindowToolBarSpace)).y;

		ImGui::SetWindowPos(ImVec2(normalizedPositionX, normalizedPositionY));
	}

	this->Handle(editor);
	ImGui::End();
}

void WB_EditorInputWindow::Handle(WB_Editor* editor) {
	ImGui::InputText("", this->buffer, sizeof(this->buffer));
	ImGui::SameLine();
	if (ImGui::Button("Apply")) {
		this->onApply.Execute();
		this->active = false;
	}
}

void WB_EditorTwoInputWindow::Handle(WB_Editor* editor) {
	ImGui::InputText("##0", this->buffer, sizeof(this->buffer));
	ImGui::InputText("##1", this->buffer2, sizeof(this->buffer2));
	if (ImGui::Button("Apply")) {
		this->onApply.Execute();
		this->active = false;
	}
	ImGui::End();
}

void WB_PainterWindow::Handle(WB_Editor* editor) {
	if (ImGui::BeginTabBar("#paintertabs")) {
		if (ImGui::BeginTabItem("Paint")) {
			if (ImGui::Button("Open File")) {
				try {
					sprite = new Sprite();
					sprite->SetTexture(TextureLoader::LoadTarga(editor->OpenFileName("Supported Files(*.tga)\0*.tga;\0").c_str()));
				}
				catch (const std::exception&) {
					sprite = nullptr;
				}
			}
			
			ImGui::InputInt("Pixels per tile: ", &this->pixelsPerTile);

			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0);
			if (sprite != nullptr && sprite->GetTexture()) {
				int index = 0;
				for (size_t y = 0; y < sprite->GetTexture()->textureData->height; y += pixelsPerTile) {
					for (size_t x = 0; x < sprite->GetTexture()->textureData->width; x += pixelsPerTile) {
						UV u = sprite->Split(pixelsPerTile, index);

						ImGui::PushID(index);
						if (ImGui::ImageButton((ImTextureID)sprite->GetTexture()->_glTexture, ImVec2(WB_PAINTERWINDOW_IMGUI_TILESIZE, WB_PAINTERWINDOW_IMGUI_TILESIZE), ImVec2(u.leftDown.x, u.leftDown.y), ImVec2(u.rightUp.x, u.rightUp.y))) {
							if (curUvData != nullptr) delete curUvData;
							curUvData = new UV(u);
						}
						ImGui::PopID();

						if (x + pixelsPerTile < sprite->GetTexture()->textureData->width) ImGui::SameLine();
						index++;
					}
				}

			}
			ImGui::PopStyleVar();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	if (!ImGui::IsWindowFocused()) {
		//Handle paint events
		if (Input::GetButton(BUTTONCODE_LEFT)) {
			if (this->curUvData != nullptr && this->sprite != nullptr && Core::GetSceneManager() && Core::GetSceneManager()->GetActiveScene()) {
				//Get tile on global mouse pos
				GridTile* tile = editor->grid->GetGridTile(Core::GetSceneManager()->GetActiveScene()->GetActiveCamera()->position + Input::GetMousePosition());

				//Check if there is no entity on tile already
				if (tile != nullptr) {
					Entity* top = Core::GetSceneManager()->GetActiveScene();
					if (editor->GetSelectedEntity()) top = editor->GetSelectedEntity();

					if (Entity* e = editor->GetEntityOnTile(tile, top)) {
						e->GetParent()->RemoveChild(e);
						delete e;
					}

					Entity* e = new Entity();
					e->scale = Vec2(this->pixelsPerTile, this->pixelsPerTile);
					e->position = tile->position;

					Sprite* s = e->AddComponent<Sprite>();
					s->SetTexture(this->sprite->GetTexture());
					s->uv = UV(*this->curUvData);

					if (editor->GetSelectedEntity()) {
						editor->GetSelectedEntity()->AddChild(e);
					}
					else {
						Core::GetSceneManager()->GetActiveScene()->AddChild(e);
					}
				}
			}
		}

		if (Input::GetButton(BUTTONCODE_RIGHT)) {
			if (Core::GetSceneManager() && Core::GetSceneManager()->GetActiveScene()) {
				GridTile* tile = editor->grid->GetGridTile(Core::GetSceneManager()->GetActiveScene()->GetActiveCamera()->position + Input::GetMousePosition());

				//Check if there is no entity on tile already
				if (tile != nullptr) {
					Entity* top = Core::GetSceneManager()->GetActiveScene();
					if (editor->GetSelectedEntity()) top = editor->GetSelectedEntity();
					if (Entity* e = editor->GetEntityOnTile(tile, top)) {
						if (e == editor->GetSelectedEntity()) editor->SetSelectedEntity(nullptr);

						e->GetParent()->RemoveChild(e);
						delete e;
					}
				}
			}
		}
	}
}

void WB_EditorGridSettingsWindow::Handle(WB_Editor* editor) {
	static float gridSizeValues[2] = {editor->grid->size.x, editor->grid->size.y};
	static float tileSizeValues[2] = {editor->grid->tileSize.x, editor->grid->tileSize.y};

	ImGui::Checkbox("Show grid", &editor->grid->active);
	ImGui::InputFloat2("Grid Size", gridSizeValues);
	ImGui::InputFloat2("Tile Size", tileSizeValues);

	if (ImGui::Button("Construct")) {
		delete editor->grid;
		editor->grid = new Grid();
		editor->grid->Construct(Vec2(gridSizeValues[0], gridSizeValues[1]), Vec2(tileSizeValues[0], tileSizeValues[1]));
		this->active = false;
	}
}


void WB_EditorHierarchy::ConstructTreenode(WB_Editor* editor, Entity* entity) {
	if (ImGui::TreeNode((std::to_string(entity->uniqueId) + " " + entity->tag).c_str())) {
		if (Core::GetSceneManager()->GetActiveScene()) {
			if (ImGui::BeginPopupContextItem()) {
				if (entity != Core::GetSceneManager()->GetActiveScene()) {
					if (ImGui::MenuItem("Select")) {
						editor->SetSelectedEntity(entity);
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
					if (entity != editor->GetSelectedEntity()) {
						editor->GetSelectedEntity()->GetParent()->RemoveChild(editor->GetSelectedEntity());
						entity->AddChild(editor->GetSelectedEntity());
					}
				}

				if (ImGui::MenuItem("Rename")) {
					editor->SetSelectedEntity(entity);

					WB_EditorInputWindow* window = new WB_EditorInputWindow("Rename entity");
					window->onApply.AddLambda([=]() {
						editor->GetSelectedEntity()->tag = std::string(window->GetBuffer());
						});

					editor->AddEditorWindow(window);
				}

				if (entity != Core::GetSceneManager()->GetActiveScene() && ImGui::MenuItem("Properties")) {
					editor->SetSelectedEntity(entity);

					if (!editor->AnyWindowOfTypeActive<WB_EditorInspector>()) editor->AddEditorWindow(new WB_EditorInspector());
				}

				if (ImGui::MenuItem("Export as prefab")) {
					editor->SetSelectedEntity(entity);

					WB_EditorInputWindow* window = new WB_EditorInputWindow("Export as prefab");
					window->onApply.AddLambda([=]() {
						Parser* parser = new Parser(std::string(window->GetBuffer()), false);
						editor->GetSelectedEntity()->isPrefab = true;
						editor->GetSelectedEntity()->prefabFilePath = window->GetBuffer();

						parser->WritePrefabToFile(editor->GetSelectedEntity());
						delete parser;
						});

					editor->AddEditorWindow(window);
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


void WB_EditorHierarchy::Handle(WB_Editor* editor) {
	if (Core::GetSceneManager()->GetActiveScene()) {
		ConstructTreenode(editor, Core::GetSceneManager()->GetActiveScene());
	}
}

void WB_EditorViewport::Handle(WB_Editor* editor) {
	ImGui::Image((ImTextureID)Core::GetRenderer()->GetFrameBufferTexture(), ImGui::GetWindowSize(), ImVec2(0, 1), ImVec2(1, 0));
}

void WB_EditorInspector::Handle(WB_Editor* editor) {
	if (editor->GetSelectedEntity()) {
		ImGui::Text(("Tag: " + editor->GetSelectedEntity()->tag).c_str());
		ImGui::Spacing();
		ImGui::Text("Local Transformations:");

		static float position[2];
		position[0] = editor->GetSelectedEntity()->position.x;
		position[1] = editor->GetSelectedEntity()->position.y;
		ImGui::InputFloat2("Position", position);

		static float scale[2];
		scale[0] = editor->GetSelectedEntity()->scale.x;
		scale[1] = editor->GetSelectedEntity()->scale.y;
		ImGui::InputFloat2("Scale", scale);

		//Set
		editor->GetSelectedEntity()->position = Vec2(position[0], position[1]);
		editor->GetSelectedEntity()->scale = Vec2(scale[0], scale[1]);

		//Spawn components
		ImGui::Spacing();
		ImGui::Text("Components:"); ImGui::SameLine();
		if (ImGui::Button("Add")) { addComponentActive = true; };
		ImGui::BeginChild("Components");
		for (size_t i = 0; i < editor->GetSelectedEntity()->GetComponents().size(); i++) {
			Component* component = editor->GetSelectedEntity()->GetComponents()[i];
			ImGui::Text(component->GetName().c_str()); ImGui::SameLine();
			if (ImGui::Button(std::string("Properties" + std::string("##") + std::to_string(i)).c_str())) {
				component->bShowComponentProperties = true;
			} ImGui::SameLine();

			if (ImGui::Button(std::string("Remove" + std::string("##") + std::to_string(i)).c_str())) {
				component->bShowComponentProperties = false;

				Core::LateFrameRemoveComponent(component);
			}

			if (component->bShowComponentProperties) {
				ImGui::Begin((editor->GetSelectedEntity()->tag + " - Component: " + component->GetName() + " - Properties").c_str(), &component->bShowComponentProperties);
				component->ShowComponentProperties();
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
						if (Component* component = Component_Register::GetNewComponentInstance(Component_Register::GetAvailableKeys()[i])) {
							if (!editor->GetSelectedEntity()->HasComponent(component)) {
								editor->GetSelectedEntity()->AddExistingComponentInstance(component);
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

		ImGui::Text(std::string("Is Prefab" + std::to_string(editor->GetSelectedEntity()->isPrefab)).c_str());
	}
	else {
		ImGui::Text("No entity selected");
	}
}

void WB_EditorResourceBrowser::Handle(WB_Editor* editor) {
	
}