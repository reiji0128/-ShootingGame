#version 330

// ワールド変換処理用のUniform変数
uniform mat4 uWorldTransform;
// ビュー射影変換用のUniform変数
uniform mat4 uViewProj;

// Attribute 0 位置 ,Attribute 1 法線 ,Attribute 2 UV座標.
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// 任意の頂点出力
out vec2 fragTexCoord;
// 法線 (in world space)
out vec3 fragNormal;
// 位置 (in world space)
out vec3 fragWorldPos;

void main()
{
	// 位置を同時座標系に変換する
	vec4 pos = vec4(inPosition, 1.0);
	// 位置をワールド空間に変換する
	pos = pos * uWorldTransform;
	// ワールド空間の位置を保存
	fragWorldPos = pos.xyz;
	// クリップ空間に変換する
	gl_Position = pos * uViewProj;

	// 法線をワールド空間に変換 (w = 0)
	fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

	// UV座標をフラグメントシェーダーに渡す
	fragTexCoord = inTexCoord;
}
