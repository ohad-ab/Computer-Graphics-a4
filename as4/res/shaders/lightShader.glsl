#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform vec4 lightColor;
uniform sampler2D sampler;
uniform vec4 lightDirection;


void main()
{
	vec3 ka = color0;
	vec3 Ia =vec3(0.5, 0.8, 0.8);
	vec3 kd = ka;
	vec3 L = normalize(position0);
	vec3 N = normal0;
	vec3 ks =vec3(0.7,0.7,0.7);
	vec3 v = normalize(position0);
	vec3 Ri = normalize(-L -2*dot(-position0,normal0)*normal0);
	int n =10;
	vec3 I = vec3(0.8, 0.5, 0.7);
	vec3 phong = ka*Ia+ (kd*(dot(N,L))+ks*pow(dot(v,Ri),n))*I;
	gl_FragColor =/*vec4(clamp(normal0, 0.0 ,1.0), 1.0);*/vec4(clamp(phong,0.0,1.0),0.0); //texture2D(sampler, texCoord0)*vec4(clamp(phong,0.0,1.0),1.0); //you must have gl_FragColor
}

