// Header file for unit class.
//
// Version: 7/8/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, August 2019
#ifndef UNIT_H
#define UNIT_H
#include "../engine/entity.h"

//Forward declare
class Structure;

//Unit extends from entity, a unit can move from a to b, and can be commanded by player
class Unit : public Entity {
private:
	float speed; /***< The movement speed of the unit*/
	Vec2 destination; /***< The destination vector of the unit*/
protected:
	/**
	* This function should be called by update every frame
	*/
	void MasterUpdate();
public:
	bool atDestination; /***< True if unit is at its destination*/
	bool moveable; /***< If true the unit can be moved by the client (Note that this bool is pure for reference to the clients)*/

	/**
	* Constructor
	*/
	Unit();

	/**
	* Override void Update
	*/
	virtual void Update() override;

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

//Base class for workers
class Worker : public Unit {
protected:
	Structure* house; /***< The structure the worker lives in (and operates from)*/
	Vec2 workplace; /***< The place where the worker will execute its work function*/
public:
	/**
	* Constructor
	* @param Structure* house
	* @param Vec2 workplace, the position of the workplace
	*/
	Worker(Structure* house, Vec2 workplace);

	/**
	* Update gets called every frame
	*/
	virtual void Update() override;

	/**
	* Work gets called every frame when worker is at its workplace
	*/
	virtual void Work();

	/**
	* AtHome gets called every frame when worker is at its house
	*/
	virtual void AtHome();
};

#endif // !UNIT_H