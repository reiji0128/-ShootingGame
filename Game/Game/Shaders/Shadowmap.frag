#version 330 core

layout (location = 0) out vec4 FragBuffer;   // HDRバッファ
layout (location = 1) out vec4 BrightBuffer; // 高輝度バッファ

// ディレクショナルライト構造体
struct DirectionalLight
{
	// ライトの方向
	vec3 mDirection;
	// ディフーズ色
	vec3 mDiffuseColor;
	// スペキュラー色
	vec3 mSpecColor;
};

// テクスチャ座標
in  vec2 fragTexCoord;
// 法線
in vec3 fragNormal;
// 頂点位置
in vec3 fragWorldPos;
// ライト空間でのフラグメント座標
in vec4 fragPosLightSpace; 

// ライティング用変数 //
// カメラ位置
uniform vec3 uCameraPos;
// ポリゴン表面のスペキュラー強度
uniform float uSpecPower;
// ディレクショナルライト
uniform DirectionalLight uDirLight;
// アンビエントライト色
uniform vec3 uAmbientLight;

uniform sampler2D uTexture;
uniform sampler2D depthMap;

// 出力カラー
out vec4 outColor;

float ShadowCaluculation(vec4 fragPosLightSpace)
{
	// パースペクティブ除算
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// [0,1]の範囲に変換
	projCoords      = projCoords * 0.5 + 0.5;
	// シャドウマップよりライトに最も近いフラグメントの深度値を得る
	float closestDepth = texture(depthMap,projCoords.xy).r;
	// 現在描画しようとしているフラグメントの深度値
	float currentDepth = projCoords.z;
	// シャドウ判定(1.0:シャドウ  0.0:シャドウ外)
	float bias = max(0.0005 * (1.0 - dot(normalize(fragNormal),uDirLight.mDirection)),0.00005);
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	return shadow;
}

void main()
{
	// ポリゴン表面の法線（フラグメントシェーダー上で補間されている）
	vec3 N = normalize(fragNormal);
	// ポリゴン表面からライト方向へのベクトル
	vec3 L = normalize(-uDirLight.mDirection);
	// ポリゴン表面からカメラ方向
	vec3 V = normalize(uCameraPos - fragWorldPos);
	// -L ベクトルを 法線 N に対して反射したベクトルRを求める
	vec3 R = normalize(reflect(-L, N));

	// フォン反射計算
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);

	vec3 Diffuse;
	vec3 Specular;

	Diffuse = uDirLight.mDiffuseColor * max(NdotL,0.0f);
	Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);

	float shadow = ShadowCaluculation(fragPosLightSpace);

	vec3 texColor = texture(uTexture,fragTexCoord).rgb;
	vec3 diffuseColor = Diffuse * texColor;
	vec3 ambientColor = uAmbientLight* texColor;

	vec3 result = ambientColor + (1.0 - shadow) * (diffuseColor + Specular);

	float brightness = dot(result,vec3(0.2126,0.7152,0.0722));

	if(brightness > 0.8)
	{
		BrightBuffer = vec4(result,0.0f);
	}
	else
	{
		BrightBuffer = vec4(0.0f,0.0f,0.0f,0.0f);
	}

	FragBuffer = vec4(result,1.0);
}