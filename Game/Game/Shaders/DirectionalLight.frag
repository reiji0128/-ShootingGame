#version 330 core

layout(location = 0) out vec4 FragBuffer;          // HDRバッファに出力
layout(location = 1) out vec4 BrightBuffer;        // 高輝度バッファに出力

// ディレクショナルライト設定
struct DirectionalLight
{
	vec3 direction;
	vec3 ambientColor;
	vec3 color;
	vec3 specular;
	float intensity;
};

// ディレクショナルライト
uniform DirectionalLight uLight;

// G-Bufferワールド位置
uniform sampler2D gPosition;
// G-Bufferワールド法線
uniform sampler2D gNormal;
// G-Bufferアルベド & スペキュラ
uniform sampler2D gAlbedoSpec;

uniform sampler2D uDepthMap;
// 視点
uniform vec3 uViewPos;

// 輝度
uniform float luminance;

// ライト空間行列
uniform mat4 uLightSpaceMatrix;

// 入力uv座標
in vec2 fragTexCoord;

float ShadowCaluculation(vec4 fragPosLightSpace,vec3 Normal,vec3 LightDir)
{
	// パースペクティブ除算
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// [0,1]の範囲に変換
	projCoords = projCoords * 0.5 + 0.5;
	// シャドウマップよりライトに最も近いフラグメントの深度値を得る
	float closestDepth = texture(uDepthMap,projCoords.xy).r;
	// 描画しようとしているフラグメントの深度値
	float currentDepth = projCoords.z;
	// シャドウ判定(1.0 : シャドウ  0.0 : シャドウ外)
	float bias = max(0.0005 * (1.0 - dot(normalize(Normal),LightDir)),0.00005);
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	return shadow;
}

void main()
{
	// gBufferよりWorldPos,Normal,アルベドを取得
	vec3 Position   = texture(gPosition,fragTexCoord).xyz;
	vec3 Normal     = texture(gNormal,fragTexCoord).xyz;
	vec4 albedoSpec = texture(gAlbedoSpec,fragTexCoord);
	vec3 Albedo     = albedoSpec.rgb;
	float Spec_p    = albedoSpec.a;

	vec3 ambient    = uLight.ambientColor * Albedo;
	vec3 LightDir   = normalize(-uLight.direction);
	Normal          = normalize(Normal);
	float diff      = max(dot(Normal,LightDir),0.0f);

	// ディフューズ
	vec3 diffuse    = uLight.color * uLight.intensity * Albedo * diff;

	// スペキュラー
	vec3 reflectDir = reflect(LightDir,Normal);
	vec3 viewDir    = normalize(uViewPos - Position);
	vec3 halfVec    = normalize(LightDir + viewDir);
	float spec      = pow(max(dot(Normal,halfVec),0.0),32);
	vec3 specular   = uLight.specular * uLight.intensity * spec * Spec_p;

	// ライト空間行列の作成
	vec4 fragPosLightSpace;
	fragPosLightSpace = vec4(Position,1.0) * uLightSpaceMatrix;

	// シャドウの計算
	float shadow = ShadowCaluculation(fragPosLightSpace,Normal,LightDir);



	vec3 result     = (1.0 - shadow) * (diffuse + specular + ambient) * luminance;

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