#version 330

// ワールド変換処理用のUniforms変数
uniform mat4 uWorld;
// ビュー射影用のUniform変数
uniform mat4 uViewProj;

// Attribute 0 位置
layout(location = 0) in vec3 inPosition;

void main()
{
	// 同時座標に変換
	vec4 pos = vec4(inPosition, 1.0);

	// クリップ空間に変換
	gl_Position = pos * uWorld * uViewProj;

}
