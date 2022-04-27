#version 330 core

// ワールド空間行列
uniform mat4 uWorldTransform;

// 行列パレット
uniform mat4 uMatrixPalette[196];
uniform mat4 uLightSpaceMat;

// Attribute 0 位置    Attribute 1 法線  Attribute 2 ボーン 
// Attribute 3 重み    Attribute 4 テクスチャ座標

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in uvec4 inSkinBones;
layout (location = 3) in vec4 inSkinWeights;
layout (location = 4) in vec2 inTexCoord;

void main()
{
	// 同時座標系に変換
	vec4 pos = vec4(inPosition,1.0);

	//スキンの位置
	vec4 skinnedPos = (pos * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

	//ライト空間のフラグメント位置を出力
	gl_Position = skinnedPos * uWorldTransform * uLightSpaceMat;
}
