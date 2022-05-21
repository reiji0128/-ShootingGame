#version 330 core

// Attribute 1 ˆÊ’u
layout(location = 0) in vec3 inPos;

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

void main()
{
	vec4 pos = vec4(inPos,1.0);
	pos = pos * uWorldTransform;
	gl_Position = pos * uViewProj;
}