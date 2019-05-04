// Header file for vec2 class.
//
// Version: 2/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019

#ifndef VEC2_H
#define VEC2_H

//Include GLM if existant in project
#ifdef GLM_SETUP_INCLUDED
#include "glm/glm.hpp"
#endif // GLM_SETUP_INCLUDED

//Define PI
#define PI 3.14159265359

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

	/**
	* Transforms a Vec2 to a glm::vec2
	*/
	glm::vec2 ToGLM() {
		return glm::vec2(this->x, this->y);
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

	//Other methods

	/**
	* The distance between this and other vec2
	* @param Vec2
	* @return float
	*/
	float Distance(Vec2 other);

	/**
	* Get the direction between this and other vec2
	* @param Vec2
	* @return Vec2
	*/
	Vec2 Direction(Vec2 other);

	/**
	* Get the normalized direction between this and other vec2
	* @param Vec2
	* @return Vec2
	*/
	Vec2 DirectionNormalized(Vec2 other);

	/**
	* Get the angle between this and other vec2
	* @param Vec2
	* @return float
	*/
	float Angle(Vec2 other);

	/**
	* Returns the magnitude of this vector
	* @return float
	*/
	float Magnitude();

	/**
	* Returns the vector in a string
	* @return const char*
	*/
	const char* ToString();

	//Static methods

	/**
	* The distance between a and b vec2
	* @param Vec2
	* @param Vec2
	* @return float
	*/
	static float Distance(Vec2 a, Vec2 b);

	/**
	* Lerp between to points, where amount is distance, note that amount should be normalized
	* @param Vec2
	* @param Vec2
	* @param float
	* @return Vec2
	*/
	static Vec2 Lerp(Vec2 a, Vec2 b, float amount);

	/**
	* Returns value converted from radians to degrees
	* @param float
	* @return float
	*/
	static float RadToDeg(float radians);

	/**
	* Returns value converted from degrees to radians
	* @param float
	* @return float
	*/
	static float DegToRad(float degrees);
};

#endif // !VEC2_H