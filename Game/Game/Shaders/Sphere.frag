#version 330

uniform float luminance;

out vec4 outColor;

void main()
{
	vec3 color = vec3(1.0,1.0,0.0);
	vec3 result = color * luminance;
	outColor = vec4(result,1.0);
}