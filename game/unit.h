// Header file for unit class.
//
// Version: 15/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#ifndef UNIT_H
#define UNIT_H
#include "../engine/entity.h"

//Unit extends from entity, a unit can move from a to b, and can be commanded by player
class Unit : public Entity {
private:
	float speed; /***< The movement speed of the unit*/
	Vec2 destination; /***< The destination vector of the unit*/

	//These members are set mostly every frame
	float amountTravelled; /***< Amount travelled in the current lerp, reset if arrived at destination*/
	Vec2 lastIdlePosition; /***< The last position when unit has reached its destination*/
public:
	/**
	* Constructor
	*/
	Unit();

	/**
	* Override void Update
	*/
	void Update() override;

	/**
	* Gets the destination of the unit
	* @return Vec2
	*/
	Vec2 GetDestination();

	/**
	* Sets the destination of the unit
	* @param Vec2
	* @return void
	*/
	void SetDestination(Vec2 destination);
};
#endif // !UNIT_H