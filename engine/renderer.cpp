// Source file for renderer class.
//
// Version: 26/3/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, March 2019
#include <glm/gtc/matrix_transform.hpp>
#include "renderer.h"
#include "debug.h"

int Renderer::Initialize(int width, int height, const char* title) {
	//Initialize GLFW
	if (!glfwInit()) {
		Debug::Log("GLFW Failed to Initialize");
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create glfw window
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	glfwMakeContextCurrent(window); // Make current context
	
	screenResolution = Vec2(width, height);

	//Initialize GLEW
	if (glewInit() != GLEW_OK) {
		Debug::Log("GLEW Failed to Initialize");
		return 1;
	}

	//Set opengl properties
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear color should be black
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);

	//Create default shader
	defaultShader = new Shader("shaders/default.vs","shaders/default.fs");

	//Setup quad VBO and VAO
	float vertices[] = {
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f,  1.0f, 0.0f, 0.0f, 1.0f
	};

	//Create quad VBO and VAO
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);

	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//Setup vertex attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Debug::Log("Renderer Initialized");
	return 0;
}

void Renderer::DrawSprite(Texture* texture, Vec2 position) {
	//Use default shader program
	glUseProgram(defaultShader->GetShaderProgram());

	//Transform coordinates to OpenGL Coordinate system



	//Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->_glTexture);

	//Draw
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::PollEvents() {
	glfwPollEvents();
}

void Renderer::SwapBuffers() {
	glfwSwapBuffers(window);
}

void Renderer::Clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

GLFWwindow* Renderer::GetWindow() {
	return window;
}