// Header file for Physics class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#include "physics.h"

#include "../components/collider.h"

bool Physics::Raycast(Vec2 origin, Vec2 direction, float length, RaycastHit& out, std::vector<Collider*> colliders, std::vector<Collider*> ignore) {
	Vec2 rayPoint = origin;
	
	for (size_t i = 0; i < length; i++) {
		rayPoint = rayPoint + direction;

		for (size_t ii = 0; ii < colliders.size(); ii++) {
			bool _ignoreCur = false;
			for (size_t iii = 0; iii < ignore.size(); iii++) {
				if (colliders[ii] == ignore[iii]) {
					_ignoreCur = true;
				}
			}

			if (!_ignoreCur && colliders[ii]->IsCollidingWithPoint(rayPoint)) {
				out.collider = colliders[ii];

				//Return out of function because we have hit
				return true;
			}
		}
	}
	return false;
}

bool Physics::InBounds(Vec2 position, Vec2 a, Vec2 b) {
	if (position.x >= a.x && position.x <= b.x) {
		if (position.y >= a.y && position.y <= b.y) {
			return true;
		}
	}
	return false;
}