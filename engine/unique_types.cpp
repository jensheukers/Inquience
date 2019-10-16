// Unique types for engine that allow foir things like delegates
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, September 2019
#include "unique_types.h"

//Include core.h for ThreadContext
#include "core.h"

#include <sstream>

//Delegate
void Delegate::AddLambda(std::function<void()> func) {
	executionList.push_back(func);
}

void Delegate::Execute() {
	for each (auto func in executionList) {
		func();
	}
}

//ThreadContext
void ThreadContext::Spawn(std::function<void()> func) {
	worker = std::thread(func);
}

void ThreadContext::Yield() {
	int nextFrame = (int)Core::GetCurrentFrame() + 1;
	while (nextFrame < (int)Core::GetCurrentFrame()) {
		continue;
	}
}
 
void ThreadContext::Sleep(int amount) {
	int sTime = (int)Core::GetTimeElapsed() + amount;
	while (sTime > (int)Core::GetTimeElapsed()) {
		continue;
	}
}

void ThreadContext::Kill() {
	worker.join();
}

std::vector<std::string> Essentials::Split(std::string string, char split) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(string);
	while (std::getline(tokenStream, token, split))
	{
		tokens.push_back(token);
	}
	return tokens;
}