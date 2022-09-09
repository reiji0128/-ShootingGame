#include "PlayerStateIdle.h"
#include "Input.h"
#include "Bullet.h"

PlayerStateIdle::PlayerStateIdle()
{
}

PlayerStateIdle::~PlayerStateIdle()
{
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="owner">プレイヤーのポインタ</param>
/// <param name="deltaTime">1フレームの経過時間</param>
/// <returns></returns>
PlayerState PlayerStateIdle::Update(PlayerActor* owner, float deltaTime)
{
    // コントローラ入力されたか
    bool isControllerInputoff = !(INPUT_INSTANCE.IsLStickMove());

    // 方向キーが入力されたか
    bool IsIdle = INPUT_INSTANCE.IsKeyOff(KEY_W) &
                  INPUT_INSTANCE.IsKeyOff(KEY_S) &
                  INPUT_INSTANCE.IsKeyOff(KEY_A) &
                  INPUT_INSTANCE.IsKeyOff(KEY_D) &
                  isControllerInputoff;

    // 移動入力がされたか
    if (!IsIdle)
    {
        return PlayerState::STATE_RUN;
    }

    Fire(owner, deltaTime);

    return PlayerState::STATE_IDLE;
}

/// <summary>
/// 状態が移行したときに1度だけ処理
/// </summary>
/// <param name="owner">プレイヤーのポインタ</param>
/// <param name="deltaTime">1フレームの経過時間</param>
void PlayerStateIdle::Enter(PlayerActor* owner, float deltaTime)
{
    // アイドル状態のアニメーションを再生
    mSkelComp = owner->GetSkeltalMeshComp();
    mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_IDLE));
}

void PlayerStateIdle::Fire(class PlayerActor* owner, float deltaTime)
{
    //// 弾を発射
    //if (INPUT_INSTANCE.GetInput(KEY_SPACE) == KEY_STATE_PUSHDOWN)
    //{
    //    //発射位置を計算
    //    Vector3 firePos;
    //    firePos = owner->GetDirection() * 100.0f;
    //    firePos.z = 100.0f;

    //    //プレイヤー弾を生成するときに Tag::PlayerBulletを指定したい。どうしたらいいだろう？
    //    BulletActor* ba = new BulletActor(firePos + owner->GetPosition(), owner->GetDirection(),
    //        Tag::PlayerBullet, 1000.0f, 10000.0f);
    //}
}
