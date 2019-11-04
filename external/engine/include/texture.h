// Header file Texture class.
//
// Version: 4/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019

#ifndef TEXTURE_H
#define TEXTURE_H

//Include map so we can store pointers to loaded textures
#include <map>
#include <string>

//Include glm and glew
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
	std::string path; /***< Path to the texture*/
} Texture;

class TextureLoader {
private:
	static TextureLoader* instance; /***< The instance of the textureloader*/
	std::map<std::string, Texture*> loadedTextures; /***< map of already loaded textures*/

	/**
	* Returns the instance of the TextureLoader
	* @return TextureLoader*
	*/
	static TextureLoader* GetInstance();

	/**
	* Converts BGR to RGB
	*/
	static void BGR2RGB(Texture* texture);

	/**
	* Uploads the texture to the GPU, and sets the _glTexture pointer
	*/
	static void UploadToGPU(Texture* texture);

	/**
	* Returns the loadedTextures map
	* @return std::map<std::string, Texture*>*
	*/
	static std::map<std::string, Texture*>& GetLoadedTextures();
public:
	/**
	* Load a Targa File.
	* @return Texture*
	*/
	static Texture* LoadTarga(char* filepath);

	/**
	* Terminates the TextureLoader
	* @return void
	*/
	static void Terminate();
};

#endif // !TEXTURE_H