// Source file Texture class.
//
// Version: 4/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#include <iostream>
#include <string>
#include "texture.h"
#include "debug.h"

//Include Core.h for static GetApplicationBuildDirectory method
#include "core.h"

TextureLoader* TextureLoader::instance; // Declare instance

TextureLoader* TextureLoader::GetInstance() {
	if (!instance) {
		instance = new TextureLoader();
	}
	return instance;
}

void TextureLoader::BGR2RGB(Texture* texture) {
	int bufferSize = (texture->textureData->width * texture->textureData->height) * texture->textureData->bytesPerPixel;

	for (int i = 0; i < bufferSize; i += texture->textureData->bytesPerPixel) {
		int b = texture->textureData->imageData[i];
		int g = texture->textureData->imageData[i + 1];
		int r = texture->textureData->imageData[i + 2];

		texture->textureData->imageData[i] = r;
		texture->textureData->imageData[i + 1] = g;
		texture->textureData->imageData[i + 2] = b;
	}
}

void TextureLoader::UploadToGPU(Texture* texture) {
	if (texture->_glTexture) {
		glDeleteTextures(1, &texture->_glTexture); // Delete the texture if already uploaded before
	}

	glGenTextures(1, &texture->_glTexture); // Generate OpenGL Ready Textures

										 // Map the surface to the texture in video memory
	glBindTexture(GL_TEXTURE_2D, texture->_glTexture);

	if (texture->textureData->type == GL_RGB) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->textureData->width, texture->textureData->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->textureData->imageData);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->textureData->width, texture->textureData->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->textureData->imageData);
	}

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

std::map<std::string, Texture*>& TextureLoader::GetLoadedTextures() {
	return GetInstance()->loadedTextures;
}

Texture* TextureLoader::LoadTarga(char* filepath) {
	//Check if texture has been loaded before
	for (std::map<std::string, Texture*>::reverse_iterator it = GetLoadedTextures().rbegin(); it != GetLoadedTextures().rend(); ++it) {
		if (it->first == filepath) {
			Debug::Log("Returning already loaded texture: " + std::string(filepath));
			return it->second;
		}
	}

	FILE* fTGA; //Declare file pointer
	fopen_s(&fTGA, Core::GetExecutableDirectoryPath().append(filepath).c_str(), "rb"); //Open file for reading

	if (fTGA == NULL) { //If error was found..
		Debug::Log("Could not open: " + std::string(filepath));
		return nullptr; //Return false
	}

	//Texture instance
	Texture* texture = new Texture();

	// Header
	TGAHeader header;

	// file data.
	TGA targa;

	if (fread(&header, sizeof(header), 1, fTGA) == 0) { //If failure to read file header
		Debug::Log("Error reading file header: " + std::string(filepath));
		return nullptr; //Return false
	}

	TextureData* textureData = new TextureData();
	if (fread(targa.header, sizeof(targa.header), 1, fTGA) == 0) { //Attempt to read next 6 bytes
		Debug::Log("Error reading TGA: " + std::string(filepath));
		return nullptr; // Return false
	}

	textureData->width = targa.header[1] * 256 + targa.header[0]; //Calculate height
	textureData->height = targa.header[3] * 256 + targa.header[2]; //Calculate width
	textureData->bpp = targa.header[4];
	targa.width = textureData->width;
	targa.height = textureData->height;
	targa.bpp = textureData->bpp;

	if ((textureData->width <= 0) || (textureData->height <= 0) || ((textureData->bpp != 24) && (textureData->bpp != 32))) { 	// Make Sure All Information Is Valid
		Debug::Log("Targa data is invalid! : " + std::string(filepath));
		return nullptr;               // Return False
	}

	if (textureData->bpp == 32) { // if its a 32bpp image
		textureData->type = GL_RGBA; //Set type to RGBA
	}
	else { // else it must be 24bpp Image
		textureData->type = GL_RGB; //Set type to RGB
	}

	targa.bytesPerPixel = (targa.bpp / 8); // Calculate the BYTES per pixel
	targa.imageSize = (targa.bytesPerPixel * targa.width * targa.height); //Calculate the memory needed to store the image

	textureData->bytesPerPixel = targa.bytesPerPixel;

	textureData->imageData = (GLubyte*)malloc(targa.imageSize); //Allocate memory

	if (textureData->imageData == NULL) { // Check if imageData was allocated correctly
		Debug::Log("textureData was not allocated correctly : " + std::string(filepath));
		return nullptr; // If Not, Return False
	}

	if (fread(textureData->imageData, 1, targa.imageSize, fTGA) != targa.imageSize) { 	// Attempt To Read All The Image Data
		Debug::Log("cant read image data : " + std::string(filepath));
		return nullptr; //If we cant read the data return false
	}

	texture->textureData = textureData;
	texture->path = filepath;

	TextureLoader::BGR2RGB(texture); //Convert from BGR to RGB
	TextureLoader::UploadToGPU(texture);

	GetLoadedTextures()[std::string(filepath)] = texture;
	Debug::Log("Texture created succesfully! Texture bits per pixel = " + std::to_string(textureData->bpp) + " | Path = " + std::string(filepath));

	fclose(fTGA);                   // Close The File
	return texture;                    // Return Success
}

void TextureLoader::Terminate() {
	//Delete all textures
	for (std::map<std::string, Texture*>::reverse_iterator it = GetLoadedTextures().rbegin(); it != GetLoadedTextures().rend(); ++it) {
		delete it->second;
		Debug::Log("Unloaded texture: " + it->first);
	}
}