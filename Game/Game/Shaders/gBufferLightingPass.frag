#version 330 core

// uv座標
in vec2 fragTexCoord;

// 出力色
out vec4 FragColor;

// ライト構造体
struct Light
{
	vec3 Position;
	vec3 Color;
};

// GBuffer 位置
uniform sampler2D gPosition;
// GBuffer 法線
uniform sampler2D gNormal;
// GBuffer アルベド + スペキュラ
uniform sampler2D gAlbedoSpec;

// ライトの最大数
const int MaxLightNum = 32;
// ライトの配列
uniform Light uLights[MaxLightNum];

// 視点(ワールド座標)
uniform vec3 uViewPos;

void main()
{
	// G-bufferからライティングに必要なデータを取得する
    // G-Bufferはスクリーンと同じ大きさなので、
    // 2Dスクリーン描画用のテクスチャ座標でG-Bufferを参照できる
	vec3  FragPos  = texture(gPosition, fragTexCoord).rgb;
	vec3  Normal   = texture(gNormal, fragTexCoord).rgb;
	vec3  Albedo   = texture(gAlbedoSpec, fragTexCoord).rgb;
	float Specular = texture(gAlbedoSpec, fragTexCoord).a;

	// 通常通りにライティング計算を行う
	vec3 lighting = Albedo * 0.1;
	vec3 viewDir = normalize(uViewPos - FragPos);

	// ライトの個数分ループ
	for(int i = 0; i<MaxLightNum; ++i)
	{
		// ディフューズライティング
		vec3 lightDir = normalize(uLights[i].Position - FragPos);
		vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * uLights[i].Color;
		
		// ライティング結果を加算
		lighting += diffuse;
	}
	// 最終カラー
	FragColor = vec4(lighting, 1.0);
}