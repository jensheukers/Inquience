// Header file for renderer class.
//
// Version: 6/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, April 2019
#ifndef RENDERER_H
#define RENDERER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "graphics/shader.h"
#include "sprite.h"
#include "math/vec2.h"

//Renderer is not to be availble outside of core.
class Renderer {
private:
	GLFWwindow* window; /***< The glfw window to render to*/
	Shader* defaultShader; /***< The default shader program*/
	
	//Local members
	Vec2 screenResolution; /***< The resolution of the window*/

	//Default VBO and VAO
	unsigned int vbo; /***< The default Vertex Buffer Object*/
	unsigned int vao; /***< The default Vertex Array Object*/
public:
	/**
	* Initializes GLFW and GLEW, also creates basic shader programs + vbo's
	* returns 0 if succes
	* @return int
	*/
	int Initialize(int width, int height, const char* title);
	
	/**
	* Renders a sprite
	* @return void
	*/
	void DrawSprite(Texture* texture, Vec2 position, Vec2 scale, float rotation, int zIndex);

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
};

#endif // !RENDERER_H