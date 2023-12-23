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
	float zoomScale;
public:
	Vec2 position;

	Camera();

	void Update();

	void SetTarget(class Entity* target);

	/**
	* @brief Sets the zoomscale
	* @param float zoomscale
	*/
	void SetZoomScale(float zoomScale) { this->zoomScale = zoomScale; }

	/**
	* @brief Returns the zoom scale
	* @return float zoomscale
	*/
	float GetZoomScale() { return this->zoomScale; }
};

#endif // !CAMERA_H