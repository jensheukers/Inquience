// Header file for font rendering
//
// Version: 2/7/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, July 2019
#ifndef FONT_H
#define FONT_H

//Include freetype
#include <ft2build.h>
#include FT_FREETYPE_H  

//include string & map
#include <string>
#include <map>

//Include glm
#include <glm/glm.hpp>

struct Character {
	unsigned int textureID;  // ID handle of the glyph texture
	glm::ivec2 size;       // Size of glyph
	glm::ivec2 bearing;    // Offset from baseline to left/top of glyph
	unsigned int advance;    // Offset to advance to next glyph
};


class Font {
public:
	std::map<unsigned int, Character> characters;

	/**
	* Constructor requires path to .tff file, then loads all font faces
	* @param std::string
	*/
	Font(std::string ttf, int size = 48);
};

//Include ui.h for text
#include "../graphics/ui.h"

class Text : public UIElement {
private:
	Font* font; /***< The font used in the text*/
	std::string text; /***< The text to display*/
	glm::vec3 color; /***< The color of the text*/
public:
	/**
	* Constructor, takes in font, text and colour
	* @param Font*
	* @param std::string
	* @param glm::vec3
	*/
	Text(Font* font, std::string text = "", glm::vec3 color = glm::vec3(1,1,1));

	/**
	* Sets the text to display
	* @param std::string
	*/
	void SetText(std::string text);

	/**
	* Returns the text
	* @return std::string
	*/
	std::string GetText();

	/**
	* Sets the color of the text
	* @param glm::vec3
	*/
	void SetColor(glm::vec3 color);

	/**
	* Returns the color of the text
	* @return glm::vec3
	*/
	glm::vec3 GetColor();

	/**
	* Returns the font of the text
	* @return Font*
	*/
	Font* GetFont();
};

#endif // !FONT_H