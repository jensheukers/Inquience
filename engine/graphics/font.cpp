// Source file for font rendering
//
// Version: 2/7/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, July 2019
#include "font.h"

//Include debug.h
#include "../core.h"
#include "../debug.h"

//Include glew.h
#include <GL/glew.h>

FontLoader* FontLoader::instance;

FontLoader* FontLoader::GetInstance() {
	if (!instance) {
		instance = new FontLoader();
	}
	return instance;
}

std::map<std::string, Font*>& FontLoader::GetLoadedFonts() {
	return GetInstance()->loadedFonts;
}

Font* FontLoader::LoadFont(std::string ttf, int size) {
	std::string path = Core::GetExecutableDirectoryPath().append(ttf);

	//Check if font has been loaded before
	for (std::map<std::string, Font*>::reverse_iterator it = GetLoadedFonts().rbegin(); it != GetLoadedFonts().rend(); ++it) {
		if (it->first == path) {
			Debug::Log("Returning already loaded font: " + std::string(path));
			return it->second;
		}
	}


	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		Debug::Log("ERROR::FREETYPE: Could not init FreeType Library");
		return nullptr;
	}

	FT_Face face;
	if (FT_New_Face(ft, path.c_str(), 0, &face)) {
		Debug::Log("ERROR::FREETYPE: Failed to load font");
		return nullptr;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	FT_Set_Pixel_Sizes(face, 0, size);

	Font* font = new Font();

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			Debug::Log("ERROR::FREETYTPE: Failed to load Glyph");
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		font->characters.insert(std::pair<GLchar, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	GetInstance()->loadedFonts[ttf] = font;

	return font;
}

Text::Text(Font* font, std::string text, glm::vec3 color) {
	this->font = font;
	this->text = text;
	this->color = color;
}

void Text::SetText(std::string text) {
	this->text = text;
}

std::string Text::GetText() {
	return this->text;
}

void Text::SetColor(glm::vec3 color) {
	this->color = color;
}

glm::vec3 Text::GetColor() {
	return this->color;
}

Font* Text::GetFont() {
	return this->font;
}