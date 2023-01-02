// Unique types for engine that allow foir things like delegates
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
#include <string>

#include <thread>

//KVP's are stored as std::string
struct KeyValuePair {
	std::string key;
	std::string value;

	KeyValuePair(std::string key, std::string value) {
		this->key = key;
		this->value = value;
	}
};

struct Delegate {
private:
	std::vector<std::function<void()>> executionList;
public:
	/**
	* Adds a lambda to the execution list
	*/
	void AddLambda(std::function<void()> func);

	/**
	* Executes the execution list
	*/
	void Execute();
};

struct ThreadContext {
private:
	std::thread worker; /**< The thread instance*/
public:
	/**
	* Spawns a thread
	* @param std::function<void()>
	*/
	void Spawn(std::function<void()> func);

	/**
	* Yield until frame is finished
	*/
	void WaitUntil();
	/**
	* Sleep for amount of seconds
	* @param int 
	*/
	void Sleep(int amount);

	/**
	* Kill the thread
	*/
	void Kill();
};

//Essential static functions, so we dont have to rewrite these every time
class Essentials {
public:
	/**
	* Splits a string and returns a vector
	*/
	static std::vector<std::string> Split(std::string string, char split);
};

#endif // !UNIQUE_TYPES_H