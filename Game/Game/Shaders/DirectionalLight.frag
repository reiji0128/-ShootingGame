#version 330 core

// �f�B���N�V���i�����C�g�ݒ�
struct DirectionalLight
{
	vec3 direction;
	vec3 ambientColor;
	vec3 color;
	vec3 specular;
	float intensity;
};

// �f�B���N�V���i�����C�g
uniform DirectionalLight uLight;

// G-Buffer���[���h�ʒu
uniform sampler2D gPosition;
// G-Buffer���[���h�@��
uniform sampler2D gNormal;
// G-Buffer�A���x�h & �X�y�L����
uniform sampler2D gAlbedoSpec;
// ���_
uniform vec3 uViewPos;

// ����uv���W
in vec2 fragTexCoord;
// �o�̓J���[
out vec4 FragOut;

void main()
{
	// gBuffer���WorldPos,Normal,�A���x�h���擾
	vec3 Position   = texture(gPosition,fragTexCoord).xyz;
	vec3 Normal     = texture(gNormal,fragTexCoord).xyz;
	vec4 albedoSpec = texture(gAlbedoSpec,fragTexCoord);
	vec3 Albedo     = albedoSpec.rgb;
	float Spec_p    = albedoSpec.a;

	vec3 ambient    = uLight.ambientColor * Albedo;
	vec3 LightDir   = normalize(-uLight.direction);
	Normal          = normalize(Normal);
	float diff      = max(dot(Normal,LightDir),0.0f);

	// �f�B�t���[�Y
	vec3 diffuse    = uLight.color * uLight.intensity * Albedo * diff;

	// �X�y�L�����[
	vec3 reflectDir = reflect(LightDir,Normal);
	vec3 viewDir    = normalize(uViewPos - Position);
	vec3 halfVec    = normalize(LightDir + viewDir);
	float spec      = pow(max(dot(Normal,halfVec),0.0),32);
	vec3 specular   = uLight.specular * uLight.intensity * spec * Spec_p;

	vec3 result     = diffuse + specular + ambient;
	FragOut         = vec4(result,1.0);
}