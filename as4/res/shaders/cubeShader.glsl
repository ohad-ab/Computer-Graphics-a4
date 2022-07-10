#version 330

//varying vec3 color0;
in vec3 position0;
uniform samplerCube sampler;
uniform vec4 lightDirection;
uniform vec4 lightColor;

void main()
{
	gl_FragColor = texture(sampler,position0);
}
