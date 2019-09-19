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
  
#define LOG_PREFIX "FFEngine: "

class Debug {
public:
	/**
	* Logs a message to the console
	* @return void
	*/
	static void Log(std::string message);
};


#endif // !DEBUG_H