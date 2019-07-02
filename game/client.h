// Header file for client class, client handles / supervises all gameplay
// Client is a singleton instance, and should never be destroyed during gameplay
//
// Version: 2/7/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, July 2019
#ifndef CLIENT_H
#define CLIENT_H

class Scene; //Forward declare

//Singleton instance
class Client {
private:
	static Client* instance; /***< The singleton client instance */

	//Scenes
	Scene* mainMenu; /***< The main menu scene pointer*/

public:
	/***
	* Constructor, if instance pointer is already set then we destroy the newly created instance 
	*/
	Client();

	/**
	* Start is called right after the instance is declared, it should only be called once
	*/
	static void Start();

	/**
	* Update gets called by main every frame
	*/
	static void Update();
};


#endif // !CLIENT_H