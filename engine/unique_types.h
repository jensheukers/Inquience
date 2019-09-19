// Unique types for engine that allow foir things like delegates
//
// Version: 19/9/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, September 2019

#ifndef UNIQUE_TYPES_H
#define UNIQUE_TYPES_H

//Include vector
#include <vector>
#include <functional>

struct Delegate {
private:
	std::vector<std::function<void()>> executionList;
public:
	/**
	* Adds a lambda to the execution list
	*/
	void AddLambda(std::function<void ()> func) {
		executionList.push_back(func);
	};

	/**
	* Executes the execution list
	*/
	void Execute() {
		for each (auto func in executionList) {
			func();
		}
	}
};

#endif // !UNIQUE_TYPES_H