// Header file for scene class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019

//Include entity since scene is a entity
#include "entity.h"
#include "camera.h"

//Unique types for KeyValuePairs
#include "unique_types.h"
#include <vector>

//Include json
#include "json.hpp"

class Scene : public Entity {
private:
	Camera * activeCamera; /***< The currently active Camera*/
	std::vector<KeyValuePair> keyValuePairs; /***< All key value pairs stored in scene*/
	
	/**
	* Writes a entity to a json array
	*/
	void BufferEntityToJsonArray(Entity* e, nlohmann::json& _array);

	/**
	* Reads a entity entry in a json file and creates a instance
	*/
	void ReadEntityFromJsonData(nlohmann::json& jsonData, Entity* parent);

public:
	/**
	* Constructor
	*/
	Scene();

	/**
	* Updates all the scene children
	*/
	void UpdateScene(bool gamePaused);

	/**
	* Sets the active camera
	* @param Camera*
	*/
	void SetActiveCamera(Camera* camera);
	/**
	* Returns the active camera
	* @return Camera*
	*/
	Camera* GetActiveCamera();

	/**
	* Adds a KVP to the scene KVP List
	*/
	void AddKVP(KeyValuePair kvp);

	/**
	* Removes a KVP from the scene KVP List by key
	*/
	void RemoveKVP(std::string key);

	/**
	* Returns the key value pairs list
	*/
	std::vector<KeyValuePair> GetKeyValuePairList() { return this->keyValuePairs; }

	/**
	* Gets KVP Value from kvp list
	*/
	std::string GetKeyValue(std::string key);

	/*
	* Writes scene data & children data to a JSON file
	* @param path
	*/
	void WriteToJsonFile(std::string destination);

	/**
	* Reads scene data & children data from a JSON file
	* @param path
	*/
	void ReadFromJsonFile(std::string path);

	/**
	* Writes a json prefab
	*/
	void WriteJsonPrefab(std::string destination, Entity* parent);

	/**
	* Reads a prefab from a jsonfile (basicly a encapsulated entity tree), and initializes it in the world
	*/
	void ReadJsonPrefab(std::string path);

	/**
	* Destructor
	*/
	~Scene();
};
