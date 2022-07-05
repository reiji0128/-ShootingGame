#version 330 core

layout (location = 0) in vec3 aPos;

// �o�͂���e�N�X�`�����W
out vec3 TexCoords;

// �v���W�F�N�V�����s��
uniform mat4 uProjection;

// �r���[�s��
uniform mat4 uView;

void main()
{
	vec4 pos = uProjection * uView * vec4(aPos,1.0)

	// �[�x�o�b�t�@��z���W��[�x�l�Ƃ��Ď󂯎�邽�߃X�J�C�{�b�N�X�̐[�x�l�����1�ɂ���
	gl_Position = vec4(pos.x,pos.y,pos.z,pos.z);

	// ���W�n���Ⴄ����z���𔽓]
	TexCoords = vec3(aPos.x,aPos.y,-aPos.z);
}