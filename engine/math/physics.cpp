// Header file for Physics class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
#include "physics.h"

#include "../components/collider.h"
#include "../entity.h"

bool Physics::InBounds(Vec2 position, Vec2 a, Vec2 b) {
	if (position.x >= a.x && position.x <= b.x) {
		if (position.y >= a.y && position.y <= b.y) {
			return true;
		}
	}
	return false;
}

Vec2 Physics::Bounce(Vec2 velocity, PhysicsHitPlaneResponseEventType side) {
	switch (side) {
	case Horizontal:
		return Vec2(velocity.x, -velocity.y);
		break;
	case Vertical:
		return Vec2(-velocity.x, velocity.y);
		break;
	}
	//Else return fully inverted velocity
	return Vec2(-velocity.x, -velocity.y);
}

PhysicsHitPlaneResponseEventType Physics::CalculateHitPlaneEventType(Vec2 position, Collider* collider) {

	//Test if object is above horizontally
	if (position.x > collider->GetOwner()->GetPosition().x && position.x < collider->GetOwner()->GetPosition().x + collider->outer.x) {
		return PhysicsHitPlaneResponseEventType::Horizontal;
	}

	if (position.y > collider->GetOwner()->GetPosition().y && position.y < collider->GetOwner()->GetPosition().y + collider->outer.y) {
		return PhysicsHitPlaneResponseEventType::Vertical;
	}
	
	return PhysicsHitPlaneResponseEventType::Edge;
}