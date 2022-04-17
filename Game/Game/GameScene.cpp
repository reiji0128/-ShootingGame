#include "GameScene.h"
#include "Game.h"
#include <iostream>
#include "Math.h"
#include "Renderer.h"
#include "DebugGrid.h"
#include "AudioManager.h"
#include "BitmapText.h"
#include "Input.h"
#include "PhysicsWorld.h"
#include "Texture.h"
#include "StaticBGActor.h"
#include "BGCollisionSetter.h"
#include "PlayerActor.h"
#include "EnemyActor.h"
#include "ThirdPersonCameraActor.h";
#include "Gun.h"
#include "Altar.h"


GameScene::GameScene()
	:mFont(nullptr)
	,mTex (nullptr)
	,mGrid(nullptr)
{
	printf("-----------------GameScene-----------------\n");
	// �t�H���g������
	mFont = new BitmapText;
	mFont->SetFontImage(16, 6, "Assets/Font/font.png");
	mFont->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");

	// �s�񏉊���
	Matrix4 proj;
	proj = Matrix4::CreatePerspectiveFOV(Math::ToRadians(45.0f), GAMEINSTANCE.GetRenderer()->GetScreenWidth(), GAMEINSTANCE.GetRenderer()->GetScreenHeight(), 1.0, 10000.0f);
	GAMEINSTANCE.GetRenderer()->SetProjMatrix(proj);

	//�f�o�b�O�O���b�h
	Vector3 color(0, 1, 0);
	mGrid = new DebugGrid(2000, 20, color);

	// ���C�e�B���O�ݒ�
	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();
	dir.mDirection = Vector3(0.7f, 0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	mTex = new Texture;
	mTex = RENDERER->GetTexture("Assets/Player/Health.png");
	//mUI = new Texture;
	//mUI = RENDERER->GetTexture("Assets/UI/UI.png");

	// �v���C���[�̐���
	PlayerActor* player = new PlayerActor
	                          (Vector3(1040.0, 740.0, 30.0),          // ���W
		                       1.0f,                                  // �X�P�[��
		                       "Assets/Player/SpecialForces.gpmesh",  // gpMesh�̃t�@�C���p�X
		                       "Assets/Player/SpecialForces.gpskel"); // gpSkel�̃t�@�C���p�X
	
	// �e�̐���
	Gun* gun = new Gun(Vector3(90, -40, 140),                         // �I�t�Z�b�g�ʒu
		               Vector3(-15, 130, 0),                          // �I�t�Z�b�g�p�x(radian)
		               "Assets/Gun/SK_KA47.gpmesh",                   // gpMesh�̃t�@�C���p�X
		                player->GetSkeltalMeshComp(),                 // �A�^�b�`���SkeltalMeshComp�N���X�̃|�C���^
		               "LeftHandIndex4");                             // �A�^�b�`��̃{�[����

	// �G�̐���
	new EnemyActor(Vector3(2260.0, -117.0, 30.0), Vector3(1, 0, 0));
	new EnemyActor(Vector3(1095.0, -452.0, 30.0), Vector3(1, 0, 0));
	new EnemyActor(Vector3(13.0, -13.0, 30.0), Vector3(1, 0, 0));
	new EnemyActor(Vector3(99.0, 1468.0, 30.0), Vector3(1, 0, 0));
	new EnemyActor(Vector3(1110.0, 1644.0, 30.0), Vector3(1, 0, 0));
	new EnemyActor(Vector3(2011.0, 1735.0, 30.0), Vector3(1, 0, 0));
	new EnemyActor(Vector3(2002.0, 415.0, 30.0), Vector3(1, 0, 0));

    // �J�����̐���
	ThirdPersonCameraActor* camera = new ThirdPersonCameraActor(player);
	camera->SetCameraLength(800.0f);

	// �o�b�N�O���E���h�̐���
	new StaticBGActor(Vector3(1040.0, 740.0, 30.0), "Assets/BackGround/StaticMesh.gpmesh");
	
	new Altar(Vector3(1000.0, 740.0, 30.0), "Assets/BackGround/Altar.gpmesh");

	// �o�b�N�O���E���h�̓����蔻��̐���
	new BGCollisionSetter("Assets/BackGround/CollisionBox.json");

	
	// �Q�[���V�X�e���ɓ����蔻�胊�X�g��o�^����
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::Enemy, Tag::Player);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::BackGround, Tag::Player);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::BackGround, Tag::Enemy);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::PlayerBullet, Tag::Enemy);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::EnemyBullet, Tag::Player);
}

GameScene::~GameScene()
{
	delete mGrid;
}

SceneBase* GameScene::Update()
{
	static float time = 0;
	time += 0.01f;

	// �����蔻��\�����[�h�̐؂�ւ�
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_START))
	{
		GAMEINSTANCE.GetPhysics()->ToggleDebugMode();
	}
	RENDERER->GetEffekseerManager()->Update();
	
	return this;
}

void GameScene::Draw()
{
	glClearColor(0.2f, 0.5f, 0.9f, 1.0f);
	// ��ʃN���A
	GAMEINSTANCE.GetRenderer()->WindowClear();

	// �Q�[���V�X�e���֘A�̕`��i�R���|�[�l���g�n�̂��̂͂����ł��ׂĕ`�悳���)
	GAMEINSTANCE.GetRenderer()->Draw();

	// 2D�`��̊J�n����
	RENDERER->SpriteDrawBegin();

	// �̗̓Q�[�W�`��
	RENDERER->DrawHelthGauge(mTex, Vector2(10, 50), 0.3f, 0.03f, 1.0f);
	//RENDERER->DrawHelthGauge(mUI, Vector2(1500, 540),1.0f ,1.0f, 1.0f);

	// 2D�`��̏I������
	RENDERER->SpriteDrawEnd();

// �G�t�F�N�g�֘A�̏��� //
	// �G�t�F�N�g�`��̊J�n����
	RENDERER->GetEffekseerRenderer()->BeginRendering();
	RENDERER->GetEffekseerManager()->Draw();
	// �G�t�F�N�g�`��̏I������
	RENDERER->GetEffekseerRenderer()->EndRendering();

	// ��ʃt���b�v
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}
