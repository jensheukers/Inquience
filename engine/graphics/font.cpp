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
#include "../debug.h"

//Include glew.h
#include <GL/glew.h>

Font::Font(std::string ttf, int size) {
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		Debug::Log("ERROR::FREETYPE: Could not init FreeType Library");
	}

	FT_Face face;
	if (FT_New_Face(ft, ttf.c_str(), 0, &face)) {
		Debug::Log("ERROR::FREETYPE: Failed to load font");
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	FT_Set_Pixel_Sizes(face, 0, size);

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
		characters.insert(std::pair<GLchar, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
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