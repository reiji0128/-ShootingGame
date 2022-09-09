#version 330 core

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
// 視点
uniform vec3 uViewPos;

// 入力uv座標
in vec2 fragTexCoord;
// 出力カラー
out vec4 FragOut;

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

	vec3 result     = diffuse + specular + ambient;
	FragOut         = vec4(result,1.0);
}