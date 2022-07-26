#version 330

// ポイントライト
struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in VS_OUT
{
	PointLight Light;
	vec2 TexCoords;
	vec3 FragPos;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
}fs_in;

out vec4 FragColor;

// テクスチャマップ
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

// ポイントライト計算
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 viewDir);

void main()
{
	vec3 normal = texture(normalMap,fs_in.TexCoords).rgb;
	normal = normalize(normal * 2.0f - 1.0f);

	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec3 result;
	result = CalcPointLight(fs_in.Light,normal,viewDir);
	FragColor = vec4(result,0.0);
}

// ポイントライト計算
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 viewDir)
{
	vec3 color = vec3(texture(diffuseMap,fs_in.TexCoords));

	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);

	// ディフューズシェーディング
	float diff = max(dot(lightDir,normal),0.0);

	// スペキュラーシェーディング
	vec3 halfVec = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal,halfVec),0.0),32.0);

	// 結果の合成
	vec3 ambient = light.ambient * color;
	vec3 diffuse = light.diffuse * diff * color;
	vec3 specular = light.specular * spec;

	return (ambient + diffuse + specular);
}