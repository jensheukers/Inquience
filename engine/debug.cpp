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

const char* Debug::lastLog;

void Debug::Log(const char* message) {
	if (message != lastLog) {
		std::cout << __TIME__ << " - " << message << std::endl;
		lastLog = message;
	}
}

void Debug::Log(std::string message) {
	if (message != lastLog) {
		std::cout << __TIME__ << " - " << message << std::endl;
		lastLog = message.c_str();
	}
}

void Debug::Log(int message) {
	if (std::to_string(message) != lastLog) {
		std::cout << __TIME__ << " - " << std::to_string(message) << std::endl;
		lastLog = std::to_string(message).c_str();
	}
}