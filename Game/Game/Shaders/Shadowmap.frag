#version 330 core

// ���C�g
struct Light
{
	vec3 direction; // ���C�g�̕���(�f�B���N�V���i�����C�g)
	vec3 ambient;   // ���C�g�A���r�G���g
	vec3 diffuse;   // ���C�g�f�B�t���[�Y
	vec3 specular;  // ���C�g�X�y�L�����[
};

uniform Light     light;
uniform vec3      viewPos;     // ���_
uniform sampler2D diffuseMap;  // �f�B�t���[�Y�e�N�X�`��
uniform sampler2D specularMap; // �X�y�L�����[�e�N�X�`��
uniform samler2D  depthMap;    // �V���h�E�}�b�v

in  vec3 Normal;            // �t���O�����g�ʒu�̖@���x�N�g��
in  vec3 FragPos;           // �t���O�����g�ʒu�̃��[���h���W
in  vec2 TexCoords;         // �e�N�X�`�����W
in  vec4 FragPosLightSpace; // ���C�g��Ԃł̃t���O�����g���W
out vec4 FragColor;         // ���̃t���O�����g�̏o��

float ShadowCaluculation(vec4 fragPosLightSpace)
{
	// �p�[�X�y�N�e�B�u���Z
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords      = projCoords * 0.5 + 0.5;

	// �V���h�E�}�b�v��胉�C�g�ɍł��߂��t���O�����g�̐[�x�l�𓾂�
	float closestDepth = texture(depthMap,projCoords.xy).r;
	// ���ݕ`�悵�悤�Ƃ��Ă���t���O�����g�̐[�x�l
	float currentDepth = projCoords.z;
	// �V���h�E����(1.0:�V���h�E  0.0:�V���h�E�O)
	float bias = max(0.0005 * (1.0 - dot(normalize(Normal),light.direction)),0.00005);
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	return shadow;
}

void main()
{
	// �A���r�G���g
	vec3 ambient = light.ambient * vec3(texture(diffuseMap,TexCoords));

	// �f�B�t���[�Y
	vec3  norm    = normalize(Normal);
	float diff    = max(dot(norm,-light.direction),0.0);
	vec3  diffuse = light.diffuse * diff * vec3(texture(diffuseMap,TexCoords));

	//�X�y�L�����[
	vec3  viewDir    = normalize(viewPos - FragPos);
	vec3  reflectDir = reflect(light.direction,norm);
	float spec       = pow(max(dot(viewDir,reflectDir),0.0)32);
	vec3  specular   = light.specular * spec * vec3(texture(dpecularMap,TexCoords));

	float shadow = ShadowCaluculation(FragPosLightSpace);
	vec3  result = ambient * (1.0 - shadow) * (diffuse + specular);

	FragColor   = vec4(result,1.0);
}