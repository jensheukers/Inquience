// Header file for vec2 class.
//
// Version: 28/3/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, March 2019
#ifndef VEC2_H
#define VEC2_H

//Include GLM if existant in project
#ifdef GLM_SETUP_INCLUDED
#include "glm/glm.hpp"
#endif // GLM_SETUP_INCLUDED

//Vec2 class has extra implemtations for glm if glm is defined
class Vec2 {
public:
	float x, y; /***< x and y components of the vector*/

	/**
	* Constructor
	*/
	Vec2() {
		this->x = 0;
		this->y = 0;
	}

	/**
	* Constructor with x component, y will be set to x
	* @param x
	*/
	Vec2(float x) {
		this->x = x;
		this->y = x;
	}

	/**
	* Constructor with x and y component
	* @param x
	* @param y
	*/
	Vec2(float x, float y) {
		this->x = x;
		this->y = y;
	}

#ifdef GLM_SETUP_INCLUDED
	/**
	* Constructor, taking glm::vec2 as parameter
	* @param glm::vec2
	*/
	Vec2(glm::vec2 other) {
		this->x = other.x;
		this->y = other.y;
	}
#endif // GLM_SETUP_INCLUDED

	//Operator overloaders
	Vec2 Vec2::operator +(Vec2 &v) {
		Vec2 res;
		
		res.x = x + v.x;
		res.y = y + v.y;
		return res;
	}

	Vec2 Vec2::operator -(const Vec2 &v) {
		Vec2 res;

		res.x = x - v.x;
		res.y = y - v.y;
		return res;
	}

	Vec2 Vec2::operator *(Vec2 &v) {
		Vec2 res;

		res.x = x * v.x;
		res.y = y * v.y;
		return res;
	}

	Vec2 Vec2::operator /(Vec2 &v) {
		Vec2 res;

		res.x = x / v.x;
		res.y = y / v.y;
		return res;
	}

	Vec2 Vec2::operator +(float t) {
		Vec2 res;

		res.x = x + t;
		res.y = y + t;
		return res;
	}

	Vec2 Vec2::operator -(float t) {
		Vec2 res;

		res.x = x - t;
		res.y = y - t;
		return res;
	}

	Vec2 Vec2::operator *(float t) {
		Vec2 res;

		res.x = x * t;
		res.y = y * t;
		return res;
	}

	Vec2 Vec2::operator /(float t) {
		Vec2 res;

		res.x = x / t;
		res.y = y / t;
		return res;
	}

//Overloaders if GLM is included
#ifdef GLM_SETUP_INCLUDED
	Vec2 Vec2::operator +(glm::vec2 &v) {
		Vec2 res;

		res.x = x + v.x;
		res.y = y + v.y;
		return res;
	}

	Vec2 Vec2::operator -(glm::vec2 &v) {
		Vec2 res;

		res.x = x - v.x;
		res.y = y - v.y;
		return res;
	}

	Vec2 Vec2::operator *(glm::vec2 &v) {
		Vec2 res;

		res.x = x * v.x;
		res.y = y * v.y;
		return res;
	}

	Vec2 Vec2::operator /(glm::vec2 &v) {
		Vec2 res;

		res.x = x / v.x;
		res.y = y / v.y;
		return res;
	}
#endif // GLM_SETUP_INCLUDED

};

#endif // !VEC2_H