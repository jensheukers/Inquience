// Header file for debug class.
//
// Version: 19/9/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, September 2019
#ifndef DEBUG_H
#define DEBUG_H

//Include iostream
#include <iostream>
#include <string>
#include <vector>

#include "math/vec2.h"
#include "glm/glm.hpp"

#define LOG_PREFIX "Inquience: "
#define MAX_AMOUNT_LOGS_SAVED 100

struct Line {
	Vec2 a;
	Vec2 b;
	glm::vec3 color;
};

//Basicly a holder for text settings
struct DebugText {
	struct Font* font;
	std::string text;
	Vec2 position;
	float size;
	glm::vec3 color;
};

class Debug {
private:
	static Debug* instance;
	std::vector<Line> _lineDrawList;
	std::vector<DebugText> _textDrawList; // Note normally text is a component, for debug though we make a exception
	std::vector<std::string> logs;
	std::vector<std::string> commands;
public:
	static bool consoleActive;

	/**
	* Returns the instance
	*/
	static Debug* GetInstance();

	/**
	* Logs a message to the console
	* @return void
	*/
	static void Log(std::string message);

	/**
	* Adds a line to the linedrawlist for drawing, note that this list gets cleared every frame
	*/
	static void DrawLine(Vec2 a, Vec2 b, glm::vec3 color = glm::vec3(1, 1, 1));

	/**
	* Draws 4 lines to create a cube
	*/
	static void DrawCube(Vec2 a, Vec2 b, glm::vec3 color = glm::vec3(1, 1, 1));

	/**
	* Draws text to the screen
	*/
	static void DrawTextLine(std::string text, Vec2 position, float size, glm::vec3 color = glm::vec3(1, 1, 1));

	/**
	* Returns the line draw list
	*/
	static const std::vector<Line> GetLineDrawList() { return GetInstance()->_lineDrawList; };

	/**
	* Returns the text draw list
	*/
	static const std::vector<DebugText> GetTextDrawList() { return GetInstance()->_textDrawList; };

	/**
	* Constructs the console using ImGui
	*/
	static void ConstructConsole();

	/**
	* Debug clear operation
	*/
	static void Clear();
};


#endif // !DEBUG_H