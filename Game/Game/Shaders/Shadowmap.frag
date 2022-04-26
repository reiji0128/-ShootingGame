#version 330 core

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
in  vec2 TexCoords;
// 法線
in vec3 Normal;
// 頂点位置
in vec3 FragPos;
// ライト空間でのフラグメント座標
in vec4 FragPosLightSpace; 

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
	projCoords      = projCoords * 0.5 + 0.5;

	// シャドウマップよりライトに最も近いフラグメントの深度値を得る
	float closestDepth = texture(depthMap,projCoords.xy).r;
	// 現在描画しようとしているフラグメントの深度値
	float currentDepth = projCoords.z;
	// シャドウ判定(1.0:シャドウ  0.0:シャドウ外)
	float bias = max(0.0005 * (1.0 - dot(normalize(Normal),uDirLight.mDirection)),0.00005);
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	return shadow;
}

void main()
{
	// ポリゴン表面の法線（フラグメントシェーダー上で補間されている）
	vec3 N = normalize(Normal);
	// ポリゴン表面からライト方向へのベクトル
	vec3 L = normalize(-uDirLight.mDirection);
	// ポリゴン表面からカメラ方向
	vec3 V = normalize(uCameraPos - FragPos);
	// -L ベクトルを 法線 N に対して反射したベクトルRを求める
	vec3 R = normalize(reflect(-L, N));

	// フォン反射計算
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);

	vec3 Diffuse;
	vec3 Specular;

	Diffuse = uDirLight.mDiffuseColor * max(NdotL,0.0f);
	Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);

	float shadow = ShadowCaluculation(FragPosLightSpace);
	vec3  result = uAmbientLight + (1.0 - shadow) * (Diffuse + Specular);

	// 最終的な色はテクスチャの色xフォンの光 (alpha = 1)
	//outColor = texture(uTexture, TexCoords) * vec4((Diffuse + uAmbientLight),1.0f) + vec4(Specular,1.0f);
	outColor = texture(uTexture, TexCoords) * vec4(result,1.0);
}