// Header file Texture class.
//
// Version: 2/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, April 2019

#ifndef TEXTURE_H
#define TEXTURE_H
#include "glm/glm.hpp"
#include "GL/glew.h"

typedef struct {
	/** Holds all the color values for the image*/
	GLubyte * imageData;

	/** Holds the number of bytes per pixel*/
	GLuint bytesPerPixel;

	/** Holds the number of bits per pixel*/
	GLuint bpp;

	/** The width of the entire image*/
	GLuint width;

	/** The height of the entire image*/
	GLuint height;

	/** Texture ID for use with glBindTexture() method*/
	GLuint texId;

	/** Data stored in *ImageData (GL_RGB OR GL_RGBA)*/
	GLuint type;
} TextureData;

typedef struct {
	/** File header to determine file type*/
	GLubyte header[12];
} TGAHeader;

typedef struct {
	/** Holds the first 6 usefull bytes of the file*/
	GLubyte header[6];

	/** Number of bytes per pixel*/
	GLuint bytesPerPixel;

	/** Amount of memory needed to hold the image*/
	GLuint imageSize;

	/** The type (GL_RGB OR GL_RGBA)*/
	GLuint type;

	/** The width of the image*/
	GLuint width;

	/** The height of the image*/
	GLuint height;

	/** Number of bits per pixel*/
	GLuint bpp;
} TGA;

typedef struct {
  TextureData* textureData; /***< Pointer to the texture data*/
  GLuint _glTexture; /***< The pointer to the gl texture on the GPU*/
} Texture;

class TextureLoader {
private:
	/**
	* Converts BGR to RGB
	*/
	static void BGR2RGB(Texture* texture);

	/**
	* Uploads the texture to the GPU, and sets the _glTexture pointer
	*/
	static void UploadToGPU(Texture* texture);

public:
	/**
	* Load a Targa File.
	*/
	static Texture* LoadTarga(char* filepath);
};

#endif // !TEXTURE_H