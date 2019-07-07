#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texCoords;

// Projection Matrix
uniform mat4 projection;

//Model Matrix
uniform mat4 model;

void main()
{
    gl_Position = projection * model * vec4(aPos.x, aPos.y, 0.0, 1.0);
	texCoords = aTexCoords;
}