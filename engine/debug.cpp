// Source file for debug class.
//
// Version: 19/9/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, September 2019
#include "debug.h"

Debug* Debug::instance;

Debug* Debug::GetInstance() {
	if (!instance) {
		instance = new Debug();
	}
	return instance;
}

void Debug::Log(std::string message) {
	std::cout << LOG_PREFIX << message << std::endl;
}

void Debug::DrawLine(Vec2 a, Vec2 b, glm::vec3 color) {
	Line line;
	line.a = a;
	line.b = b;
	line.color = color;
	GetInstance()->_lineDrawList.push_back(line);
}

void Debug::Clear() {
	for (size_t i = 0; i < GetInstance()->_lineDrawList.size(); i++) {
		GetInstance()->_lineDrawList.erase(GetInstance()->_lineDrawList.begin() + i);
	}
}