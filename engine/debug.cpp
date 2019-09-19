// Source file for debug class.
//
// Version: 19/9/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, September 2019
#include "debug.h"

void Debug::Log(std::string message) {
	std::cout << LOG_PREFIX << message << std::endl;
}