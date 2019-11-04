// Header file for Physics class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#ifndef PHYSICS_H
#define PHYSICS_H

//Include vec2.h
#include "vec2.h"

class Physics {
public:
	/**
	* Returns true if position is between a and b
	* @param Vec2
	* @param Vec2
	* @param Vec2
	* @return bool 
	*/
	static bool InBounds(Vec2 position, Vec2 a, Vec2 b);
};

#endif // !PHYSICS_H