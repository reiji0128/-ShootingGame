#include "SkyBox.h"
#include "CubeMapComponent.h"
#include "Game.h"
#include "Renderer.h"

SkyBox::SkyBox()
	:Actor(Tag::SkyBox)
{
	// �L���[�u�}�b�v�R���|�[�l���g�𐶐�
	mCubeMapComp = new CubeMapComponent(this);

	// �����_���[�ɃL���[�u�}�b�v�R���|�[�l���g�̃|�C���^��n��
	RENDERER->SetActiveSkyBox(mCubeMapComp);

	// �g�p����e�N�X�`���𐶐�
	mCubeMapComp->CreateTexture("Assets/Skybox/");
}

SkyBox::~SkyBox()
{
}
