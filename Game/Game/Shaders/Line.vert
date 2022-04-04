#version 330

// ビュー射影用のUniform変数
uniform mat4 uViewProj;

// Attribute 0 位置
layout(location = 0) in vec3 inPosition;

void main()
{
	// 座標を同時座標に変換する
	vec4 pos = vec4(inPosition, 1.0);

	// 空間に変換
	gl_Position = pos * uViewProj;

}
