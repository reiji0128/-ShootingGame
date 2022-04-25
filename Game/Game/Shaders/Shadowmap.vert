#version 330 core
layout (location = 0) in vec3 aPos;       // ���_�ʒu
layout (location = 1) in vec3 aNormal;    // �@��
layout (location = 2) in vec2 aTexCoords; // �e�N�X�`�����W

uniform mat4 model;             // ���f���s��
uniform mat4 view;              // �r���[�s��
uniform mat4 projection;        // �v���W�F�N�V�����s��
uniform mat4 lightSpaceMatrix;  // ���C�g��ԍs��

out vec3 Normal;            // �t���O�����g�ւ̖@���o��
out vec3 FragPos;           // �t���O�����g�ւ̍��W�o��
out vec2 TexCoords;         // �e�N�X�`�����W
out vec4 FragPosLightSpace; // ���C�g��Ԃł̃t���O�����g���W

void main()
{
	FragPos     = vec3(model * vec4(aPos,1.0));
	Normal      = mat3(transpose(inverse(model))) * aNormal;
	TexCoords   = aTexCoords;
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos,1.0);
	gl_Position = projection * view * vec4(FragPos,1.0); 
}