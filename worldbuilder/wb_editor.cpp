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

#include <component_register.h>
#include <input.h>

EditorWindow::EditorWindow() {
	this->active = true; // Always set to active
}

EditorInputWindow::EditorInputWindow(const char* title) {
	this->title = title;
}

void EditorInputWindow::Handle(WB_Editor* editor) {
	ImGui::Begin(this->title, &this->active);
	ImGui::InputText("", this->buffer, sizeof(this->buffer));
	ImGui::SameLine();
	if (ImGui::Button("Apply")) {
		this->onApply.Execute();
		this->active = false;
	}
	ImGui::End();
}

void EditorTwoInputWindow::Handle(WB_Editor* editor) {
	ImGui::Begin(this->title, &this->active);
	ImGui::InputText("##0", this->buffer, sizeof(this->buffer));
	ImGui::InputText("##1", this->buffer2, sizeof(this->buffer2));
	if (ImGui::Button("Apply")) {
		this->onApply.Execute();
		this->active = false;
	}
	ImGui::End();
}

void EditorHierarchy::ConstructTreenode(WB_Editor* editor, Entity* entity) {
	if (ImGui::TreeNode((std::to_string(entity->uniqueId) + " " + entity->tag).c_str())) {
		if (Core::GetSceneManager()->GetActiveScene()) {
			if (ImGui::BeginPopupContextItem()) {
				if (entity != Core::GetSceneManager()->GetActiveScene()) {
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

					editor->AddEditorWindow(window);
				}

				if (entity != Core::GetSceneManager()->GetActiveScene() && ImGui::MenuItem("Properties")) {
					editor->currentSelectedEntity = entity;

					if (!editor->AnyWindowOfTypeActive<EditorInspector>()) editor->AddEditorWindow(new EditorInspector());
				}

				if (ImGui::MenuItem("Export as prefab")) {
					editor->currentSelectedEntity = entity;

					EditorInputWindow* window = new EditorInputWindow();
					window->onApply.AddLambda([=]() {
						Parser* parser = new Parser(std::string(window->GetBuffer()), false);
						editor->currentSelectedEntity->isPrefab = true;
						editor->currentSelectedEntity->prefabFilePath = window->GetBuffer();

						parser->WritePrefabToFile(editor->currentSelectedEntity);
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

void EditorHierarchy::Handle(WB_Editor* editor) {
	ImGui::Begin("Hierarchy", &this->active);
	if (Core::GetSceneManager()->GetActiveScene()) {
		ConstructTreenode(editor, Core::GetSceneManager()->GetActiveScene());
	}

	ImGui::End();
}

void EditorInspector::Handle(WB_Editor* editor) {
	ImGui::Begin("Inpector", &this->active);
	if (editor->currentSelectedEntity) {
		ImGui::Text(("Tag: " + editor->currentSelectedEntity->tag).c_str());
		ImGui::Spacing();
		ImGui::Text("Local Transformations:");

		static float position[2];
		position[0] = editor->currentSelectedEntity->position.x;
		position[1] = editor->currentSelectedEntity->position.y;
		ImGui::InputFloat2("Position", position);

		static float scale[2];
		scale[0] = editor->currentSelectedEntity->scale.x;
		scale[1] = editor->currentSelectedEntity->scale.y;
		ImGui::InputFloat2("Scale", scale);

		//Set
		editor->currentSelectedEntity->position = Vec2(position[0], position[1]);
		editor->currentSelectedEntity->scale = Vec2(scale[0], scale[1]);

		//Spawn components
		ImGui::Spacing();
		ImGui::Text("Components:"); ImGui::SameLine();
		if (ImGui::Button("Add")) { addComponentActive = true; };
		ImGui::BeginChild("Components");
		for (size_t i = 0; i < editor->currentSelectedEntity->GetComponents().size(); i++) {
			Component* component = editor->currentSelectedEntity->GetComponents()[i];
			ImGui::Text(component->GetName().c_str()); ImGui::SameLine();
			//if (ImGui::Button(std::string("Properties" + std::string("##") + std::to_string(i)).c_str())) {
			//	component->bShowComponentProperties = true;
			//} ImGui::SameLine();

			if (ImGui::Button(std::string("Remove" + std::string("##") + std::to_string(i)).c_str())) {
				//component->bShowComponentProperties = false;
				editor->currentSelectedEntity->RemoveComponent(component);
			}

			//if (component->bShowComponentProperties) {
			//	ImGui::Begin((editor->currentSelectedEntity->tag + " - Component: " + component->GetName() + " - Properties").c_str(), &component->bShowComponentProperties);
			//	component->OnComponentPropertiesEditor();
			//	ImGui::End();
			//}
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

		ImGui::Text(std::string("Is Prefab" + std::to_string(editor->currentSelectedEntity->isPrefab)).c_str());
	}
	else {
		ImGui::Text("No entity selected");
	}

	ImGui::End();
}

void EditorCreateEntityWizard::Handle(WB_Editor* editor) {
	ImGui::Begin("Create Entity", &this->active);
	if (!Core::GetSceneManager()->GetActiveScene()) {
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

		entity->position = Vec2(position[0], position[1]);
		entity->scale = Vec2(scale[0], scale[1]);

		if (parentToSelected && editor->currentSelectedEntity) {
			editor->currentSelectedEntity->AddChild(entity);
		}
		else {
			Core::GetSceneManager()->GetActiveScene()->AddChild(entity);
		}

		editor->currentSelectedEntity = entity;
		this->active = false;
	}

	ImGui::End();
}

void EditorKeyValuePairWizard::Handle(WB_Editor* editor) {
	ImGui::Begin("Key Value Pairs", &this->active);

	if (!Core::GetSceneManager()->GetActiveScene()) {
		ImGui::Text("No active scene");
		ImGui::End();
		return;
	}

	std::vector<KeyValuePair> kvps = Core::GetSceneManager()->GetActiveScene()->GetKeyValuePairList();
	
	for (size_t i = 0; i < kvps.size(); i++) {
		ImGui::Text(("[Key: " + kvps[i].key + "] - [Value: " + kvps[i].value + "]").c_str());
		ImGui::SameLine();
		if (ImGui::Button("Delete")) {
			Core::GetSceneManager()->GetActiveScene()->RemoveKVP(kvps[i].key);
		}
	}

	// add a new KVP
	if (ImGui::Button("Add KVP")) {
		EditorTwoInputWindow* inputWindow = new EditorTwoInputWindow();
		inputWindow->onApply.AddLambda([=]() {
			Core::GetSceneManager()->GetActiveScene()->AddKVP(KeyValuePair(inputWindow->GetBuffer(), inputWindow->GetSecondBuffer()));
			});

		editor->AddEditorWindow(inputWindow);
	}
	ImGui::End();
}

EditorWindow* WB_Editor::AddEditorWindow(EditorWindow* w) {
	activeEditorWindows.push_back(w);
	return w;
}

EditorWindow* WB_Editor::RemoveEditorWindow(EditorWindow* w) {
	for (size_t i = 0; i < activeEditorWindows.size(); i++) {
		if (activeEditorWindows[i] == w) {
			return RemoveEditorWindow(i);
		}
	}

	return nullptr;
}

EditorWindow* WB_Editor::RemoveEditorWindow(int i) {
	EditorWindow* w = activeEditorWindows[i];
	activeEditorWindows.erase(activeEditorWindows.begin() + i);
	return w;
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


WB_Editor::WB_Editor() {
	camera = new Camera();

	//Create new empty scene
	currentScene = NewScene();

	KeyComboEvent copySelectedEvent = KeyComboEvent(KeyCombo{ KeyEvent(KEYCODE_LEFT_CONTROL, KeyEvent_Type::Get), KeyEvent(KEYCODE_V, KeyEvent_Type::GetDown) });
	copySelectedEvent.onActivate.AddLambda([=]() {
		if (!currentSelectedEntity) return;
		Entity* copy = new Entity(*currentSelectedEntity);
		currentSelectedEntity->GetParent()->AddChild(copy);

		//Set position to mouse position
		copy->position = Input::GetMousePosition() + Core::GetSceneManager()->GetActiveScene()->GetActiveCamera()->position;
		currentSelectedEntity = copy;
		});
	combos.push_back(copySelectedEvent);

	KeyComboEvent deleteSelectedEvent = KeyComboEvent(KeyCombo{ KeyEvent(KEYCODE_DELETE, KeyEvent_Type::GetDown) });
	deleteSelectedEvent.onActivate.AddLambda([=]() {
		if (!currentSelectedEntity) return;
		currentSelectedEntity->GetParent()->RemoveChild(currentSelectedEntity);
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

void WB_Editor::HandleInput() {
	for (size_t i = 0; i < combos.size(); i++) {
		combos[i].Check();
	}

	Camera* camera = Core::GetSceneManager()->GetActiveScene()->GetActiveCamera();
	//Camera input
	if (Input::GetKey(KEYCODE_KP_2)) {
		camera->position = camera->position + Vec2(0, 1);
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

	//Mouse input
	if (Input::GetButtonDown(BUTTONCODE_LEFT)) {
		SetCurrentSelectedEntityByPosition(Core::GetSceneManager()->GetActiveScene(), mousePos);
	}

	if (currentSelectedEntity && bHoldingEntity && Input::GetButton(BUTTONCODE_RIGHT)) {
		currentSelectedEntity->position = mousePos - (currentSelectedEntity->scale / 2);

		//Try to set on grid tile if possible
		//if (GridTile* tile = grid->GetGridTile(currentSelectedEntity->position)) {
		//	currentSelectedEntity->position = tile->position;
		//}

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

		currentSelectedEntity->scale = currentSelectedEntity->scale + extraScale;

		//Draw to screen
		Debug::DrawTextLine(scaleModeString, Vec2(0, 100), 0.5f, glm::vec3(1, 0, 0));
		Debug::DrawTextLine("Left Control + Q to change mode", Vec2(0, 150), 0.5f, glm::vec3(1, 0, 0));
	}
	else if (currentSelectedEntity) {
		Debug::DrawTextLine("Left Click to modify position and scale", Vec2(0, 100), 0.5f, glm::vec3(1, 0, 0));
	}

	if (Input::GetButtonUp(BUTTONCODE_RIGHT)) {
		bHoldingEntity = false;
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
		if (ImGui::MenuItem("Hierarchy")) {
			this->AddEditorWindow(new EditorHierarchy());
		}

		if (ImGui::MenuItem("Inspector")) {
			this->AddEditorWindow(new EditorInspector);
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Scene")) {
		if (ImGui::MenuItem("New Entity")) { AddEditorWindow(new EditorCreateEntityWizard()); }
		if (ImGui::MenuItem("KVP Wizard")) { AddEditorWindow(new EditorKeyValuePairWizard()); }
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Settings")) {
		if (ImGui::MenuItem("Grid Settings")) {

		}
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
		//Draw all colliders
		//std::vector<Collider*> colliders;
		//SceneManager::GetActiveScene()->GetAllComponentsOfTypeInChildren(colliders);
		//for (size_t i = 0; i < colliders.size(); i++) {
		//	colliders[i]->Update();
		//}

		//Draw around current selected entity
		if (this->currentSelectedEntity) {
			Debug::DrawCube(this->currentSelectedEntity->GetGlobalPosition(),
				this->currentSelectedEntity->GetGlobalPosition() + this->currentSelectedEntity->GetGlobalScale(),
				glm::vec3(1, 0, 1));
		}

		//Only execute when there is no window focused
		if (!ImGui::IsAnyWindowFocused()) {
			this->HandleInput();
		}
	}
}

void WB_Editor::SetCurrentSelectedEntityByPosition(Entity* parent, Vec2 pos) {
	for (size_t i = 0; i < parent->GetChildren().size(); i++) {
		Entity* child = parent->GetChildren()[i];
		SetCurrentSelectedEntityByPosition(child, pos);
	}

	if (Physics::InBounds(pos, parent->GetGlobalPosition(), parent->GetGlobalPosition() + parent->GetGlobalScale()) && parent != Core::GetSceneManager()->GetActiveScene()) {
		currentSelectedEntity = parent;
		bHoldingEntity = true;
	}
}