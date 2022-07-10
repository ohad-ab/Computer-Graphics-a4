#version 330

attribute vec3 position;
attribute vec3 color;
attribute vec3 normal;
attribute vec2 texCoords;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 Normal;

//out vec3 color0;
out vec2 texCoord0;
out vec3 normal0;
out vec3 color0;
out vec3 position0;

void main()
{
	position0 = position;
	//color0 = color;
	gl_Position = proj *view*Normal* vec4(position, 1.0);
}
