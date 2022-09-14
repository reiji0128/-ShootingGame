#version 330 core

layout(location = 0) out vec4 FragBuffer;          // HDRバッファに出力
layout(location = 1) out vec4 BrightBuffer;        // 高輝度バッファに出力

// ポイントライト
struct PointLight
{
	// 光線位置
	vec3 position;
	// ライトアンビエント
	vec3 ambient;
	// ライトディフューズ
	vec3 diffuse;
	// ライトスペキュラー
	vec3 specular;

	// 定数
	float constant;
	// 線形項
	float linear;
	// 2乗項
	float quadratic;
};

uniform PointLight uLight;

// G-Bufferワールド位置
uniform sampler2D gPosition;
// G-Bufferワールド法線
uniform sampler2D gNormal;
// G-Bufferアルベド & スペキュラー強度
uniform sampler2D gAlbedoSpec;
// 視点
uniform vec3      uViewPos;

// 輝度
uniform float luminance;

// uv座標
in vec2 TexCoords;
// フラグメントの出力
out vec4 FragColor;

void main()
{
	// G-BufferからWorldPos,Normal,アルベドを取得
	vec3  Position    = texture(gPosition,TexCoords).xyz;
	vec3  Normal      = texture(gNormal,TexCoords).xyz;
	vec4  AlbedoSpec  = texture(gAlbedoSpec,TexCoords);
	vec3  Albedo      = AlbedoSpec.rgb;
	float Spec_p      = AlbedoSpec.a;

	// ライトとの距離
	float l_distance = length(uLight.position - Position);
	float attenuation = max(1.0f - l_distance / 100.0f, 0.0f);

	// ディフューズ
	vec3  norm     = normalize(Normal);
	vec3  lightDir = normalize(uLight.position - Position);
	float diff     = max(dot(norm,lightDir),0.0);
	vec3  diffuse  = uLight.diffuse * diff * Albedo;

	// スペキュラー
	vec3 viewDir   = normalize(uViewPos - Position);
	vec3 halfDir   = normalize(lightDir + viewDir);
	float spec     = pow(max(dot(norm,halfDir),0.0),32);
	vec3 specular  = uLight.specular * spec * Spec_p;

	// アンビエント
	vec3 ambient   = uLight.ambient * Albedo;

	ambient       *= attenuation;
	diffuse       *= attenuation;
	specular      *= attenuation;

	vec3 result   = (ambient + diffuse + specular) * luminance;
	
	// ピクセルの輝度を求める
	float brightness = dot(result,vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 0.8)
	{
		BrightBuffer = vec4(result, 0.0f);
	}
	else
	{
		BrightBuffer = vec4(0.0f,0.0f,0.0f,0.0f);
	}
	FragBuffer = vec4(result,1.0);
}