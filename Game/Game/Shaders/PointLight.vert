#version 330 core

layout (location = 0) in vec3 inPos;

uniform mat4 model;
uniform mat4 uView;
uniform mat4 uProjection;

out vec2 TexCoords;

void main()
{
	//vec4 pos = uProjection * uView * model * vec4(inPos, 1.0);
	//gl_Position = pos ;

	vec4 pos = vec4(inPos,1.0);
	pos = pos * model;
	pos = pos * uView * uProjection;
	gl_Position = pos;
	
	// 正規化デバイス座標より,G-Bufferのテクスチャ座標に変換
	pos.x = pos.x / pos.w * 0.5f + 0.5f;
	pos.y = pos.y / pos.w * 0.5f + 0.5f;
	TexCoords = pos.xy;
}