#version 330

// ワールド変換処理に使うUniform変数
uniform mat4 uWorldTransform;
// ビュー射影変換に使うUniform変数
uniform mat4 uViewProj;

// Attribute 0 座標, Attribute 1 法線, Attribute 2 テクスチャ座標
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// 任意の頂点を出力
out vec2 fragTexCoord;

void main()
{
	// 座標を同次座標に変換する
	vec4 pos = vec4(inPosition,1.0);
	// ワールド空間に変換してから、クリップ空間に変換
	gl_Position = pos * uWorldTransform * uViewProj;

	// テクスチャ座標をフラグメントシェーダーに渡す
	fragTexCoord = inTexCoord;
}