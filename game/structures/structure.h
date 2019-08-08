// Header file for structure class, this file contains the base class for structures + derived classes
// Version: 7/8/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, August 2019

//Include entity.h
#include "../../engine/entity.h"

#define STRUCTURE_DEFAULT_CAPACITY 3;

class Structure : public Entity {
private:
	int health; /***< The health of the structure, if health is 0 structure will be deleted*/
	int maxCapacity; /***< The amount of capacity the Structure can hold*/
	int capacity; /***< The capacity of the structure */
protected:
	/**
	* Master update method, should be called from within Update
	*/
	void MasterUpdate();
public:
	//Properties
	int cost[3]; /***< The cost of the structure to be build (wood, stones, materials)*/

	/**
	* Constructor
	*/
	Structure();

	/**
	* Update method
	*/
	virtual void Update() override;

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

	/**
	* Stores amount in the capacity, will return true if success
	* @param int amount
	* @return bool
	*/
	bool Store(int amount);

	/**
	* withdraws amount from the capacity, returns true if success
	* @param int amount
	* @return bool
	*/
	bool Withdraw(int amount);
};

//Define costs so it can be easily modified
#define WOODCUTTERHUT_COST_WOOD 100 
#define WOODCUTTERHUT_COST_STONES 50
#define WOODCUTTERHUT_COST_MATERIALS 0

//Structures
class WoodCutterHut : public Structure {
public:
	/**
	* Constructor
	*/
	WoodCutterHut();
};