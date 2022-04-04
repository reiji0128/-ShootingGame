#version 330

// ���[���h�ϊ������Ɏg��Uniform�ϐ�
uniform mat4 uWorldTransform;
// �r���[�ˉe�ϊ��Ɏg��Uniform�ϐ�
uniform mat4 uViewProj;

// Attribute 0 ���W, Attribute 1 �@��, Attribute 2 �e�N�X�`�����W
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// �C�ӂ̒��_���o��
out vec2 fragTexCoord;

void main()
{
	// ���W�𓯎����W�ɕϊ�����
	vec4 pos = vec4(inPosition,1.0);
	// ���[���h��Ԃɕϊ����Ă���A�N���b�v��Ԃɕϊ�
	gl_Position = pos * uWorldTransform * uViewProj;

	// �e�N�X�`�����W���t���O�����g�V�F�[�_�[�ɓn��
	fragTexCoord = inTexCoord;
}