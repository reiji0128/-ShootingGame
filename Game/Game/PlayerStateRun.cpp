#include "PlayerStateRun.h"
#include "Input.h"
#include "Bullet.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerStateRun::PlayerStateRun()
	:mRotationTargetDir(0.0f,0.0f,0.0f)
{
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStateRun::~PlayerStateRun()
{
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="owner">プレイヤーのポインタ</param>
/// <param name="deltaTime">1フレームの経過時間</param>
/// <returns></returns>
PlayerState PlayerStateRun::Update(PlayerActor* owner, float deltaTime)
{
	// コントローラ入力されたか
	Vector2 stickL = INPUT_INSTANCE.GetLStick();
	bool isContollerInputOff = !INPUT_INSTANCE.IsLStickMove();

	bool IsIdle = INPUT_INSTANCE.IsKeyOff(KEY_W) &
		          INPUT_INSTANCE.IsKeyOff(KEY_S) &
		          INPUT_INSTANCE.IsKeyOff(KEY_D) &
		          INPUT_INSTANCE.IsKeyOff(KEY_A) &
		          isContollerInputOff;

	// 移動入力がなかったら
	if (IsIdle)
	{
		return PlayerState::STATE_IDLE;
	}

	// 移動処理
	MoveCalc(owner,deltaTime);

	Fire(owner, deltaTime);

	return PlayerState::STATE_RUN;
}

/// <summary>
/// 状態が移行したときに1度だけ処理
/// </summary>
/// <param name="owner">プレイヤーのポインタ</param>
/// <param name="deltaTime">1フレームの経過時間</param>
void PlayerStateRun::Enter(PlayerActor* owner, float deltaTime)
{
    // アイドル状態のアニメーションを再生
    mSkelComp = owner->GetSkeltalMeshComp();
    mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_RUN));
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="owner">プレイヤーのポインタ</param>
/// <param name="deltaTime">1フレームの経過時間</param>
void PlayerStateRun::MoveCalc(PlayerActor* owner, float deltaTime)
{
	// スピード
	const float speed = 300.0f;
	// 現在のプレイヤーのスピード
	float charaSpeed = owner->GetSpeed();

    // カメラから見た前方ベクトルを取得
	Vector3 playerPos = GAMEINSTANCE.GetViewTarget();
	Vector3 cameraPos = GAMEINSTANCE.GetViewPos();
    Vector3 forwardVec = playerPos - cameraPos;
    forwardVec.z = 0.0f;
    forwardVec = Vector3::Normalize(forwardVec);
	
    // 前方ベクトルとz軸から右方向ベクトルを算出
    Vector3 rightVec = Vector3::Cross(Vector3::UnitZ, forwardVec);

	// 右方向から回転角を算出
	float forwardAngle = 0.0f;
	float angleSign;
	Vector3 tmpVec;
	forwardAngle = acosf(Vector3::Dot(Vector3::UnitX, rightVec));

	// 右回転か左回転か？
	tmpVec = Vector3::Cross(Vector3::UnitX, rightVec);
	angleSign = (tmpVec.z > 0.0) ? 1.0f : -1.0f;
	forwardAngle *= angleSign;

	// プレイヤーの前方ベクトル
	mCharaForwardVec = owner->GetForward();

    // キー入力
    Vector3 DirVec(0.0f, 0.0f, 0.0f);
	if (INPUT_INSTANCE.IsKeyPressed(KEY_W))
	{
		DirVec += forwardVec;
	}

	if (INPUT_INSTANCE.IsKeyPressed(KEY_S))
	{
		DirVec -= forwardVec;
	}

	if (INPUT_INSTANCE.IsKeyPressed(KEY_D))
	{
		DirVec += rightVec;
	}

	if (INPUT_INSTANCE.IsKeyPressed(KEY_A))
	{
		DirVec -= rightVec;
	}

	// ゲームパッド
	Vector2 stickL = INPUT_INSTANCE.GetLStick();
	Matrix3 rot = Matrix3::CreateRotation(forwardAngle);
	stickL = Vector2::Transform(stickL, rot);
	DirVec.x += stickL.x;
	DirVec.y += stickL.y;

	if (DirVec.Length() > 0.5)
	{
		
		DirVec.Normalize();

	    // 回転中か？
		if (mRotateNow)
		{
			RotateActor();
		}

	    // 入力とプレイヤーの前方ベクトルがほぼ同じなら
		if (isNearAngle(DirVec, mCharaForwardVec))
		{
			mCharaForwardVec = DirVec;
		}
		else
		{
			mRotateNow = true;
			mRotationTargetDir = DirVec;
		}
		
		// 現在のスピードを保存
		charaSpeed = speed * deltaTime;
	}

	// 移動処理
	playerPos += charaSpeed * DirVec;

	// プレイヤーの位置・スピード・
	// 前方ベクトル・変換行列の再計算の必要をセット
	owner->SetPosition(playerPos);
	owner->SetSpeed(charaSpeed);
	owner->SetDirection(mCharaForwardVec);
	owner->SetComputeWorldTransform();
}

/// <summary>
/// 回転処理
/// </summary>
void PlayerStateRun::RotateActor()
{
	// 前方ベクトルが目標方向に近ければ回転終了
	if (isNearAngle(mRotationTargetDir, mCharaForwardVec))
	{
		mCharaForwardVec = mRotationTargetDir;
		mRotateNow = false;
	}
	else
	{
		// 回転させる
		Vector3 calcVec;
		calcVec = zRotateToAimVec(mCharaForwardVec, mRotationTargetDir, 10.0f);

		// 前方ベクトルが目標方向を超えていないか
		Vector3 cross1, cross2;
		cross1 = Vector3::Cross(mCharaForwardVec, mRotationTargetDir);
		cross2 = Vector3::Cross(calcVec, mRotationTargetDir);

		// 目標方向を超えたら終了
		if (cross1.z * cross2.z < 0.0f)
		{
			calcVec = mRotationTargetDir;
			mRotateNow = false;
		}
		mCharaForwardVec = calcVec;
	}
}

void PlayerStateRun::Fire(class PlayerActor* owner, float deltaTime)
{
	//// 弾を発射
	//if (INPUT_INSTANCE.GetInput(KEY_SPACE) == KEY_STATE_PUSHDOWN)
	//{
	//	//発射位置を計算
	//	Vector3 firePos;
	//	firePos = owner->GetDirection() * 100.0f;
	//	firePos.z = 100.0f;

	//	//プレイヤー弾を生成
	//	BulletActor* ba = new BulletActor(firePos + owner->GetPosition(), owner->GetDirection(),
	//		Tag::PlayerBullet, 1000.0f, 10000.0f);
	//}
}
