// Header file for debug class.
//
// Version: 26/3/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, March 2019
#ifndef DEBUG_H
#define DEBUG_H
#include <string>

//Debug class is a singleton instance

class Debug {
private:
	static Debug* instance; /***< The instance of the debug class*/
public:
	/**
	* Logs a message to the console
	* @return void
	*/
	static void Log(const char* message);

	/**
	* Logs a message to the console
	* @return void
	*/
	static void Log(std::string message);
};


#endif // !DEBUG_H