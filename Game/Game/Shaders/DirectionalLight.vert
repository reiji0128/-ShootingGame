#version 330

// Attribute 0 座標, Attribute 1 uv座標
layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTexCoord;

// uv座標出力変数
out vec2 fragTexCoord;

void main()
{
	// テクスチャ座標をフラグメントシェーダーに渡す
	fragTexCoord = inTexCoord;
	gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0);
}