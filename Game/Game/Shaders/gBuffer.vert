#version 330 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

uniform mat4 uWorldTransform; // ワールド変換行列
uniform mat4 uViewProj;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	// 位置を同時座標系に変換し
	vec4 pos = vec4(inPosition,1.0);
	// 位置をワールド空間に変換
	pos =  pos *uWorldTransform;
	// ワールド空間の位置を保存
	FragPos = pos.xyz;

	Normal = (vec4(inNormal,0.0f) * uWorldTransform).xyz;
	
	// UV座標をフラグメントシェーダに渡す
	TexCoords   = inTexCoords;

	gl_Position = pos * uViewProj;
}

