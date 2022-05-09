#version 330 core

layout (location = 0) in vec3 inPosition;     // 頂点位置
layout (location = 1) in vec3 inNormal;       // 法線
layout (location = 2) in vec2 inTexCoords;    // テクスチャ座標

uniform mat4 uWorldTransform;                 // ワールド変換行列
uniform mat4 uView;                           // ビュー行列
uniform mat4 uProjection;                     // プロジェクション行列
uniform mat4 uLightSpaceMatrix;               // ライト空間行列

out vec3 fragNormal;                          // フラグメントへの法線出力
out vec3 fragWorldPos;                        // フラグメントへの座標出力
out vec2 fragTexCoord;                        // テクスチャ座標
out vec4 fragPosLightSpace;                   // ライト空間でのフラグメント座標

void main()
{
	// 位置を同時座標系に変換し
	vec4 pos = vec4(inPosition,1.0);
	// 位置をワールド空間に変換
	pos =  pos *uWorldTransform;
	// ワールド空間の位置を保存
	fragWorldPos = pos.xyz;
	// クリップ空間に変換
	gl_Position = pos * uView * uProjection ;

	fragNormal = (vec4(inNormal,0.0f) * uWorldTransform).xyz;
	fragPosLightSpace = vec4(fragWorldPos,1.0) * uLightSpaceMatrix;
	
	// UV座標をフラグメントシェーダに渡す
	fragTexCoord   = inTexCoords;
}