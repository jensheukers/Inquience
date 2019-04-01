// Header file for Texture struct and TextureLoader class.
//
// Version: 1/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, April 2019
#ifndef TEXTURE_H
#define TEXTURE_H
#include "GL/glew.h"

//Texture structure, contains all data about the texture
struct Texture {
	unsigned char* imageData; /***< Holds all the color values of the Texture */
	unsigned char header[6]; /***< Holds the first 6 usefull bytes of the image*/
	unsigned int imageSize; /***< The size of the image in memory*/
	unsigned int bitsPerPixel; /***< The amount of bits per pixel in the Texture*/
	unsigned int bytesPerPixel; /***< The amount of bytes per pixel in the Texture*/
	unsigned int width; /***< The width of the texture*/
	unsigned int height; /***< The height of the texture*/
	unsigned int textureID; /***< The texture id for use with the glBindTexture method*/
	unsigned int type; /***< The type of the texture (i.e GL_RGBA)*/
};

//Texture Loader only has static methods, these return the generated Texture instances
//Or modify them
class TextureLoader {
private:
	/**
	* Converts texture from BGR to RGB
	*/
	static void BGR2RGB(Texture* texture);

	/**
	* Uploads a texture to the GPU
	*/
	static void UploadToGPU(Texture* texture);
public:
	/**
	* Loads a targa texture instance into memory, then returns a pointer
	* @param const char* filePath
	* @return Texture*
	*/
	static Texture* LoadTarga(const char* filePath);
};

#endif // !TEXTURE_H