// Source file for Texture struct and TextureLoader class.
//
// Version: 1/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, April 2019
#include "texture.h"
#include <iostream>
#include "debug.h"

void TextureLoader::BGR2RGB(Texture* texture) {
	for (unsigned i = 0; i < texture->imageSize; i += texture->bytesPerPixel) {
		int b = texture->imageData[i];
		int g = texture->imageData[i + 1];
		int r = texture->imageData[i + 2];

		texture->imageData[i] = r;
		texture->imageData[i + 1] = g;
		texture->imageData[i + 2] = b;
	}
}

void TextureLoader::UploadToGPU(Texture* texture) {
	if (texture->textureID) { // If uploaded before, delete from gpu
		glDeleteTextures(1, &texture->textureID);
	}

	glGenTextures(1, &texture->textureID);

	if (texture->type == GL_RGB) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->imageData);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->imageData);
	}

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture* TextureLoader::LoadTarga(const char* filePath) {
	//Read file
	FILE* file;
	fopen_s(&file, filePath, "rb");

	if (file == nullptr) {
		Debug::Log("Error reading targa file header: " + std::string(filePath)); // Log
		return nullptr; // Return nullptr
	}

	GLubyte header[12]; // File header to determine file type
	if (fread(&header, sizeof(header), 1, file) == 0) { //If failure to read file header
		Debug::Log("Error reading file header: " + std::string(filePath));
		return false; //Return false
	}

	//Load image data into texture
	Texture* texture = new Texture();

	if (fread(texture->header, sizeof(texture->header), 1, file) == 0) {
		Debug::Log("Error reading targa: " + std::string(filePath)); // Log
		return nullptr; // Return nullptr
	}

	texture->width = texture->header[1] * 256 + texture->header[0]; //Calculate width
	texture->height = texture->header[3] * 256 + texture->header[2]; //Calculate height
	texture->bitsPerPixel = texture->header[4];

	//Do some checks
	if (texture->width <= 0 || texture->height <= 0 || texture->bitsPerPixel != 24 && texture->bitsPerPixel != 32) {
		Debug::Log("Targa data is invalid: " + std::string(filePath));
		return nullptr;
	}

	//Determine texture type
	if(texture->bitsPerPixel == 24) {
		texture->type = GL_RGB;
	}
	else {
		texture->type = GL_RGBA;
	}

	texture->bytesPerPixel = (texture->bitsPerPixel / 8);
	texture->imageSize = (texture->bytesPerPixel * texture->width * texture->height);

	texture->imageData = (GLubyte*)malloc(texture->imageSize); //Allocate memory

	if (texture->imageData == NULL) {
		Debug::Log("Texture data was not allocated correctly: " + std::string(filePath));
		return nullptr;
	}

	if (fread(texture->imageData, 1, texture->imageSize, file) != texture->imageSize) {
		Debug::Log("Cant read image data: " + std::string(filePath));
		return nullptr;
	}

	//Convert from BGR to RGB and upload to GPU
	TextureLoader::BGR2RGB(texture);
	TextureLoader::UploadToGPU(texture);


	//Success
	Debug::Log("Generated Texture: " + std::string(filePath) + ", Bits per pixel: " + std::to_string(texture->bitsPerPixel));
	return texture;
}