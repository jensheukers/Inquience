// Header file for renderer class.
//
// Version: 4/7/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, July 2019
#ifndef RENDERER_H
#define RENDERER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "graphics/shader.h"
#include "graphics/font.h"
#include "components/sprite.h"
#include "math/vec2.h"

class Entity; // Forward declartion of entity

//Renderer is not to be availble outside of core.
class Renderer {
private:
	GLFWwindow* window; /***< The glfw window to render to*/
	Shader* defaultShader; /***< The default shader program*/
	Shader* textShader; /***< The text shader program*/

	//Local members
	Vec2 screenResolution; /***< The resolution of the window*/

	//Default VBO and VAO
	unsigned int vbo; /***< The default Vertex Buffer Object*/
	unsigned int vao; /***< The default Vertex Array Object*/

	//Text VBO and VAO
	unsigned int textVbo; /***< The text vbo*/
	unsigned int textVao; /***< The text vao*/

	//Lists
	std::vector<Entity*> renderList; /***< List of entities to render*/
	std::vector<Text*> textList; /***< List of text to render*/

	/**
	* Renders a sprite
	* @return void
	*/
	void DrawSprite(Sprite* sprite, Vec2 position, Vec2 scale);

	/**
	* Draws text to the screen
	*/
	void DrawText(Font* font, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

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
	* Adds a text to the render list
	* @param Text*
	* @return void
	*/
	void RegisterText(Text* entity);

	/**
	* Removes a text from the render list
	* @param Text*
	* @return void
	*/
	void RemoveText(Text* entity);

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

	/**
	* Returns the window resolution as a Vec2
	* @return Vec2
	*/
	Vec2 GetResolution();

	/**
	* local window size callback method
	*/
	void WindowSizeCallback(GLFWwindow* window, int width, int height);

	/**
	* Destructor
	*/
	~Renderer();
};

#endif // !RENDERER_H