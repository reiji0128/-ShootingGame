#version 330 core
layout (location = 0) out vec3 gPosition;   // G-Bufferの位置テクスチャに出力
layout (location = 1) out vec3 gNormal;     // G-Bufferの法線テクスチャに出力
layout (location = 2) out vec4 gAlbedoSpec; // G-BufferのRGBAテクスチャに出力

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D uDiffuse;  // オブジェクトのディフューズマップ
uniform sampler2D uSpecular; // オブジェクトのスペキュラマップ

void main()
{
	// フラグメントの位置をG-Bufferに保存
	gPosition = FragPos;
	// フラグメントごとの法線をG-Bufferに保存
	gNormal = normalize(Normal);
	// ディフューズカラーをアルベドのRGBに保存
	gAlbedoSpec.rgb = texture(uDiffuse,TexCoords).rgb;
	// スペキュラ強度をアルベドのa要素に保存
	//gAlbedoSpec.a = texture(uSpecular,TexCoords).r;
	gAlbedoSpec.a = 0.1;
}
