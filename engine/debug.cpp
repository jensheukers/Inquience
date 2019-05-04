// Source file for debug class.
//
// Version: 26/3/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, March 2019
#include <iostream>
#include "debug.h"

void Debug::Log(const char* message) {
	std::cout << __TIME__ << " - " << message << std::endl;
}

void Debug::Log(std::string message) {
	std::cout << __TIME__ << " - " << message << std::endl;
}

void Debug::Log(int message) {
	std::cout << __TIME__ << " - " << std::to_string(message) << std::endl;
}