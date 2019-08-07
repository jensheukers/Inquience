// Header file for structure class, this file contains the base class for structures + derived classes
// Version: 7/8/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, August 2019

//Include entity.h
#include "../../engine/entity.h"

class Structure : public Entity {
private:
	//Properties
	int cost[3]; /***< The cost of the structure to be build (wood, stones, materials)*/

	int health; /***< The health of the structure, if health is 0 structure will be deleted*/
protected:
	/**
	* Master update method, should be called from within Update
	*/
	void MasterUpdate();
public:
	/**
	* Constructor
	*/
	Structure();

	/**
	* Update method
	*/
	void Update() override;

	/**
	* Damages the structure, taking amount from health
	* @param int amount
	*/
	void Damage(int amount);

	/**
	* Heals the structure, adding amount to health
	* @param int amount
	*/
	void Heal(int amount);
};