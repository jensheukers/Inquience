// Color Header file .
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, March 2021


/**
* A color struct with values between 0 and 255
*/
struct ColorRGBA {
	unsigned int _r = 255;
	unsigned int _g = 255;
	unsigned int _b = 255;
	unsigned int _a = 255;

	//Constructor
	ColorRGBA() {
		this->_r = 255;
		this->_g = 255;
		this->_b = 255;
		this->_a = 255;
	}

	//Constructor
	ColorRGBA(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
		this->_r = r;
		this->_g = g;
		this->_b = b;
		this->_a = a;
	}
};

#define RED ColorRGBA(255, 0, 0, 255)
#define BLUE ColorRGBA(0, 0, 255, 255)
#define GREEN ColorRGBA(0, 255, 0, 255)
#define MAGENTA ColorRGBA(255, 0,   255, 255)
#define WHITE ColorRGBA(255, 255 , 255, 255)
#define BLACK ColorRGBA(0, 0, 0, 255)