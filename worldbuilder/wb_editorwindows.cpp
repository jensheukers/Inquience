#include "wb_editorwindows.h"

#include <imgui.h>

#include <wb_editor.h>
#include <math/vec2.h>

#include <debug.h>

WB_EditorWindow::WB_EditorWindow() {
	this->active = true; // Always set to active
}

void WB_PainterWindow::Handle(WB_Editor* editor) {
	ImGui::Begin("Painter");

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
						if (ImGui::ImageButton((ImTextureID)sprite->GetTexture()->_glTexture, ImVec2(WB_PAINTERWINDOW_IMGUI_TILESIZE, WB_PAINTERWINDOW_IMGUI_TILESIZE), ImVec2(u.leftDown.x, u.leftDown.y), ImVec2(u.rightUp.x, u.rightUp.y))) {
							if (curUvData != nullptr) delete curUvData;
							curUvData = new UV(u);
						}

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

	ImGui::End();
}

void WB_EditorGridSettingsWindow::Handle(WB_Editor* editor) {
	ImGui::Begin("Grid Settings", &this->active);
	static float gridSizeValues[2] = {editor->grid->size.x, editor->grid->size.y};
	static float tileSizeValues[2] = {editor->grid->tileSize.x, editor->grid->tileSize.y};

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
