// Header file for scene class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019

//Include entity since scene is a entity
#include "entity.h"
#include "camera.h"

class Scene : public Entity {
private:
	Camera * activeCamera; /***< The currently active Camera*/

	/**
	* Returns a vector of LuaParsableLines, containing all child's properties and components
	*/
	std::vector<struct LuaParsableLine> GetChildLines(Entity* child, unsigned tabs = 0);
public:
	/**
	* Constructor
	*/
	Scene();

	/**
	* Updates all the scene children
	*/
	void Update() override;

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
	* Writes the scene to a luascript file instance, including children components and properties
	* Returns 0 if sucessfull
	*/
	int WriteToLuaFile(struct LuaScriptFile& file, std::string funcName);

	/**
	* Destructor
	*/
	~Scene();
};
