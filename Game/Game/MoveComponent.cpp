#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
	, mAngularSpeed(0.0f)
	, mForwardSpeed(0.0f)
{

}

void MoveComponent::Update(float deltaTime)
{
	// 回転スピードが0に近くなければ
	if (!Math::NearZero(mAngularSpeed))
	{
		Quaternion rot = mOwner->GetRotation();
		float angle = mAngularSpeed * deltaTime;
		//回転を増分するためのクォータニオンを作成
		//（上軸を中心に回転）
		Quaternion inc(Vector3::UnitZ, angle);
		//前回と新しいをクォータニオンを連結
		rot = Quaternion::Concatenate(rot, inc);
		mOwner->SetRotation(rot);
	}

	// 前進スピードまたはストレイフスピードが0に近くなければ
	if (!Math::NearZero(mForwardSpeed) || !Math::NearZero(mStrafeSpeed))
	{
		Vector3 pos = mOwner->GetPosition();
		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;

		// 右向きベクトルとストレイフ速度によって位置を更新
		pos += mOwner->GetRight() * mStrafeSpeed * deltaTime;
		mOwner->SetPosition(pos);
	}
}
