// Header file for Renderer class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, October 2019
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

class Renderer {
private:
	GLFWwindow* window; /***< Pointer to the GLFWWindow instance */

	//Shaders
	Shader* defaultShader; /***< The default shader used to draw sprites*/
	Shader* textShader; /***< The shader used to draw texts*/
	Shader* lineShader; /***< The shader used to draw lines*/
	Shader* screenShader; /**Shader used for drawing framebuffer to screen*/

	//Local variables
	Vec2 resolution; /***< The resolution of the window*/

	glm::mat4 projection; /***< The projection matrix*/

	unsigned int vbo; /***< The vertex buffer object used to draw sprites*/
	unsigned int vao; /***< The vertex array object*/

	unsigned int textVbo; /***< The vertex buffer object used to draw text*/
	unsigned int textVao; /***< The text vertex array object*/

	//Framebuffer rendering
	unsigned int screenVbo; /**The screen quad vertex buffer object*/
	unsigned int screenVao;

	unsigned int fbo; /** The framebuffer object*/
	unsigned int fboTexture; /**the texture for the framebuffer to write to*/

	unsigned int rbo; /** Renderbuffer object*/

	/**
	* Renders a entity to the screen
	*/
	void RenderEntity(class Entity* entity, class Camera* camera);
public:
	/**
	* Draws text to the screen
	* @param Font* the pointer to the font used in the text
	* @param std::string the content of the text that is to be drawn
	* @param Vec2 position
	* @param float scale
	* @param glm::vec3 color
	*/
	void RenderText(Font* font, std::string text, Vec2 position, float size, glm::vec3 color);

	/**
	* Constructor
	* @param Vec2 resolution
	* @param Vec2 scale of the window
	* @param const char* titleName
	*/
	Renderer(Vec2 resolution, const char* title, bool fullscreen = false);

	/**
	* Renders the scene
	*/
	void RenderScene(class Scene* scene, class Camera* camera);

	/**
	* Renders all ImGui related elements
	*/
	void RenderImGui();

	/**
	* Draws a line from point a to point b, when camera is provided camera position will be included in calculation
	*/
	void DrawLine(Vec2 a, Vec2 b, glm::vec3 color, class Camera* camera);

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

	/*
	* @brief returns the framebuffer texture
	* @return unsigned int - framebuffer texture data object
	*/
	unsigned int GetFrameBufferTexture() { return fboTexture; }

	/**
	* Destructor
	*/
	~Renderer();
};

#endif // !RENDERER_H