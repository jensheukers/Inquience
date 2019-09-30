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

struct Line {
	Vec2 a;
	Vec2 b;
	glm::vec3 color;
};

class Debug {
	static Debug* instance;
	std::vector<Line> _lineDrawList;
public:
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
	* Returns the line draw list
	*/
	static std::vector<Line> GetLineDrawList() { return GetInstance()->_lineDrawList; };

	/**
	* Debug clear operation
	*/
	static void Clear();
};


#endif // !DEBUG_H