#version 330 core
layout (location = 0) in vec3 aPos;       // 頂点位置
layout (location = 1) in vec3 aNormal;    // 法線
layout (location = 2) in vec2 aTexCoords; // テクスチャ座標

uniform mat4 model;             // モデル行列
uniform mat4 view;              // ビュー行列
uniform mat4 projection;        // プロジェクション行列
uniform mat4 lightSpaceMatrix;  // ライト空間行列

out vec3 Normal;            // フラグメントへの法線出力
out vec3 FragPos;           // フラグメントへの座標出力
out vec2 TexCoords;         // テクスチャ座標
out vec4 FragPosLightSpace; // ライト空間でのフラグメント座標

void main()
{
	FragPos     = vec3(model * vec4(aPos,1.0));
	Normal      = mat3(transpose(inverse(model))) * aNormal;
	TexCoords   = aTexCoords;
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos,1.0);
	gl_Position = projection * view * vec4(FragPos,1.0); 
}