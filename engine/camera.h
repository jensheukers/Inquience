// Header file for vec2 class.
//
// Version: 23/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#ifndef CAMERA_H
#define CAMERA_H
#include "math/vec2.h"

class Camera {
private:
	Vec2 position; /***< The position of the camera*/
public:
	/**
	* Set the position of the camera
	* @param Vec2
	*/
	void SetPosition(Vec2 position);

	/**
	* Return the position of the camera
	* @return Vec2
	*/
	Vec2 GetPosition();
};

#endif // !CAMERA_H