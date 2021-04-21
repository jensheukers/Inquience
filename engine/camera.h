// Header file for vec2 class.
//
// Version: 23/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, 2021
#ifndef CAMERA_H
#define CAMERA_H
#include "math/vec2.h"


class Camera : Vec2{
private:
	class Entity* target; /***< Target camera can focus on*/
public:
	Vec2 position;

	Camera();

	void Update();

	void SetTarget(class Entity* target);
};

#endif // !CAMERA_H