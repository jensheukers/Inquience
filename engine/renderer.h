// Header file for renderer class.
//
// Version: 1/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#ifndef RENDERER_H
#define RENDERER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "graphics/shader.h"
#include "sprite.h"
#include "math/vec2.h"

class Entity; // Forward declartion of entity

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

	//Lists
	std::vector<Entity*> renderList; /***< List of entities to render*/

	/**
	* Renders a sprite
	* @return void
	*/
	void DrawSprite(Texture* texture, Vec2 position, Vec2 scale, float rotation, SpriteUV uvData = SpriteUV());

public:
	/**
	* Initializes GLFW and GLEW, also creates basic shader programs + vbo's
	* returns 0 if succes
	* @return int
	*/
	int Initialize(int width, int height, const char* title);

	/**
	* Adds a entity to the render list
	* @param Entity*
	* @return void
	*/
	void RegisterEntity(Entity* entity);

	/**
	* Removes a entity from the render list
	* @param Entity*
	* @return void
	*/
	void RemoveEntity(Entity* entity);

	/**
	* Renders the current frame, Filters out all entities that are not in camera view, and does a z-index test
	* @return void
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
};

#endif // !RENDERER_H