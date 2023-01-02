// Source file for debug class.
//
// Version: 19/9/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, September 2019
#include "debug.h"

#include <functional>

#include "imgui.h"

#include "input.h"
#include "core.h"


#include "graphics/font.h"

Debug* Debug::instance;

bool Debug::consoleActive = false;

Debug* Debug::GetInstance() {
	if (!instance) {
		instance = new Debug();
	}
	return instance;
}

void Debug::Log(std::string message) {
	std::string logMessage = LOG_PREFIX + message;
	std::cout << logMessage << std::endl;

	//Push to logs
	static int counter = 0;
	static std::string lastLogMessage;
	if (GetInstance()->logs.size() > 0 && lastLogMessage == logMessage) {
		GetInstance()->logs[GetInstance()->logs.size() - 1] = std::to_string(counter) + " - " + logMessage;
		counter++;
	}
	else {
		GetInstance()->logs.push_back(logMessage);
		lastLogMessage = logMessage;
		counter = 0;
	}
	
	if (GetInstance()->logs.size() > MAX_AMOUNT_LOGS_SAVED) {
		GetInstance()->logs.erase(GetInstance()->logs.begin());
	}
}

void Debug::DrawLine(Vec2 a, Vec2 b, glm::vec3 color) {
	Line line;
	line.a = a;
	line.b = b;
	line.color = color;
	GetInstance()->_lineDrawList.push_back(line);
}

void Debug::DrawCube(Vec2 a, Vec2 b, glm::vec3 color) {
	Line line0; line0.a = a; line0.b = Vec2(b.x, a.y); line0.color = color; GetInstance()->_lineDrawList.push_back(line0);
	Line line1; line1.a = Vec2(b.x, a.y); line1.b = b; line1.color = color; GetInstance()->_lineDrawList.push_back(line1);
	Line line2; line2.a = b; line2.b = Vec2(a.x, b.y); line2.color = color; GetInstance()->_lineDrawList.push_back(line2);
	Line line3; line3.a = Vec2(a.x, b.y); line3.b = a; line3.color = color; GetInstance()->_lineDrawList.push_back(line3);
}

void Debug::DrawTextLine(std::string text, Vec2 position, float size, glm::vec3 color) {
	static Font* font = FontLoader::LoadFont("fonts/consola.ttf");
	DebugText debugText;
	debugText.font = font;
	debugText.text = text;
	debugText.position = position;
	debugText.size = size;
	debugText.color = color;
	GetInstance()->_textDrawList.push_back(debugText);
}

void Debug::ConstructConsole() {
	//Handle events
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[KEYCODE_BACKSPACE] = Input::GetKey(KEYCODE_BACKSPACE);
	io.KeysDown[KEYCODE_ENTER] = Input::GetKeyDown(KEYCODE_ENTER);
	io.KeysDown[KEYCODE_DOWN] = Input::GetKeyDown(KEYCODE_DOWN);
	io.KeysDown[KEYCODE_UP] = Input::GetKeyDown(KEYCODE_UP);
	io.KeysDown[KEYCODE_LEFT] = Input::GetKeyDown(KEYCODE_LEFT);
	io.KeysDown[KEYCODE_RIGHT] = Input::GetKeyDown(KEYCODE_RIGHT);

	ImGui::Begin("Console");
		ImGui::SetWindowSize(ImVec2(512, 512));
		ImGui::BeginChild("Logs", ImVec2(512, 450));
			for (size_t i = 0; i < GetInstance()->logs.size(); i++){
				ImGui::Text(GetInstance()->logs[i].c_str());
			}
		ImGui::EndChild();

		static char buffer[128]; // Allocate buffer
		ImGui::InputText("", buffer, sizeof(buffer));

		std::function<void()> onExecute = [=](){
			GetInstance()->logs.push_back(buffer);

			//Run  on lua
			Core::GetLuaScript()->Run(buffer);
			
			GetInstance()->commands.push_back(buffer);

			//Clear buffer
			for (size_t i = 0; i < 128; i++) {
				buffer[i] = '\0';
			}
		};

		if (ImGui::IsItemActive() && Input::GetKeyDown(KEYCODE_ENTER)) {
			onExecute();
		}

		ImGui::SameLine();
		if (ImGui::Button("Execute")) {
			onExecute();
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear")) {
			for (size_t i = 0; i < GetInstance()->logs.size(); i++) {
				GetInstance()->logs.erase(GetInstance()->logs.begin() + i);
			}
		}

	ImGui::End();
}

void Debug::Clear() {
	for (size_t i = 0; i < GetInstance()->_lineDrawList.size(); i++) {
		GetInstance()->_lineDrawList.erase(GetInstance()->_lineDrawList.begin() + i);
	}

	for (size_t i = 0; i < GetInstance()->_textDrawList.size(); i++) {
		GetInstance()->_textDrawList.erase(GetInstance()->_textDrawList.begin() + i);
	}
}