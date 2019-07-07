// Header file for Renderer class.
//
// Version: 7/7/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, July 2019
#ifndef RENDERER_H
#define RENDERER_H

//Include glew.h and glfw3.h
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Include vector
#include <vector>

//Include vec2.h
#include "math/vec2.h"

//Include graphics
#include "graphics/font.h"
#include "graphics/shader.h"

//Include sprite.h
#include "components/sprite.h"

//Pointer to 

class Renderer {
private:
	GLFWwindow* window; /***< Pointer to the GLFWWindow instance */

	//Shaders
	Shader* defaultShader; /***< The default shader used to draw sprites*/
	Shader* textShader; /***< The shader used to draw texts*/

	//Local variables
	Vec2 resolution; /***< The resolution where the game is displayed in*/
	Vec2 scale; /***< The scale of the window*/

	glm::mat4 projection; /***< The projection matrix*/

	unsigned int vbo; /***< The vertex buffer object used to draw sprites*/
	unsigned int vao; /***< The vertex array object*/

	unsigned int textVbo; /***< The vertex buffer object used to draw text*/
	unsigned int textVao; /***< The text vertex array object*/

	//Vectors
	std::vector<Sprite*> registeredSprites; /***< The sprites registered and ready for rendering*/
	std::vector<Text*> registeredTexts; /***< The texts registered and ready for rendering*/

	/**
	* Renders a sprite to the screen, modifies the existing vertex buffer object and reinserts correct vertex data.
	* @param Sprite* the sprite to render
	* @param Vec2 position
	* @param Vec2 scale
	*/
	void DrawSprite(Sprite* sprite, Vec2 position, Vec2 scale = Vec2(1, 1));

	/**
	* Draws text to the screen
	* @param Font* the pointer to the font used in the text
	* @param std::string the content of the text that is to be drawn
	* @param Vec2 position
	* @param float scale
	* @param glm::vec3 color
	*/
	void DrawText(Font* font, std::string text, Vec2 position, float scale, glm::vec3 color);
public:
	/**
	* Constructor
	* @param Vec2 resolution
	* @param Vec2 scale of the window
	* @param const char* titleName
	*/
	Renderer(Vec2 resolution, Vec2 scale, const char* title);

	/**
	* Adds a sprite to the registeredSprites vector
	* @param Sprite*
	*/
	void RegisterSprite(Sprite* sprite);

	/**
	* Removes a sprite from the registeredSprites vector
	* @param Sprite*
	*/
	void RemoveSprite(Sprite* sprite);

	/**
	* Adds a text to the registeredTexts vector
	* @param Text*
	*/
	void RegisterText(Text* text);

	/**
	* Removes a text to the registeredTexts vector
	* @param Text*
	*/
	void RemoveText(Text* text);

	/**
	* Renders the current frame, filters out all entities that are not in camera view and does a z-index test.
	*/
	void RenderFrame();

	/**
	* Polls all GLFW events
	*/
	void PollEvents();

	/**
	* Swaps the buffers
	*/
	void SwapBuffers();

	/**
	* Clears the current frame
	*/
	void Clear();

	/**
	* Returns the glfw window pointer
	* @return GLFWWindow*
	*/
	GLFWwindow* GetWindow();

	/**
	* Returns the window resolution as a Vec2
	* @return Vec2
	*/
	Vec2 GetResolution();

	/**
	* Returns the window scale as a Vec2
	* @return Vec2
	*/
	Vec2 GetScale();

	/**
	* Destructor
	*/
	~Renderer();
};

#endif // !RENDERER_H