#version 330 core

layout (location = 0) in vec3 aPos;          // 頂点位置
layout (location = 1) in vec3 aNormal;       // 法線
layout (location = 2) in vec2 aTexCoords;    // テクスチャ座標

//uniform mat4 model;                  // モデル行列
uniform mat4 uWorldTransform;          // ワールド変換行列
uniform mat4 view;                     // ビュー行列
uniform mat4 projection;               // プロジェクション行列
uniform mat4 lightSpaceMatrix;         // ライト空間行列

out vec3 Normal;             // フラグメントへの法線出力
out vec3 FragPos;           // フラグメントへの座標出力
out vec2 TexCoords;         // テクスチャ座標
out vec4 FragPosLightSpace; // ライト空間でのフラグメント座標

void main()
{
	// 位置を同時座標系に変換し
	vec4 pos = vec4(aPos,1.0);
	// 位置をワールド空間に変換
	pos =  pos *uWorldTransform;
	// ワールド空間の位置を保存
	FragPos = pos.xyz;
	// クリップ空間に変換
	gl_Position = pos * view * projection ;

	Normal = (vec4(aNormal,0.0f) * uWorldTransform).xyz;
	FragPosLightSpace = vec4(FragPos,1.0) * lightSpaceMatrix;
	
	// UV座標をフラグメントシェーダに渡す
	TexCoords   = aTexCoords;
}