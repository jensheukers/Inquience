// Header file for Physics class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#include "physics.h"

bool Physics::InBounds(Vec2 position, Vec2 a, Vec2 b) {
	if (position.x >= a.x && position.x <= b.x) {
		if (position.y >= a.y && position.y <= b.y) {
			return true;
		}
	}
	return false;
}