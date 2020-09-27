#version 330

attribute vec3 position;
attribute vec3 color;
attribute vec3 normal;
attribute vec2 texCoords;

out vec3 position1;
out vec3 normal0;

uniform mat4 MVP;
uniform mat4 Normal;

void main()
{
	
	normal0 = (Normal * vec4(normal, 0.0)).xyz;
	position1 = vec3(Normal * vec4(position, 1.0));
	gl_Position = MVP *Normal* vec4(position, 1.0); //you must have gl_Position
}
