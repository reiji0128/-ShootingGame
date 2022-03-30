#pragma once
#include "Component.h"
#include "math.h"
#include <cstddef>
#include <Effekseer.h>


class EffectComponent : public Component
{
public:
	EffectComponent(class Actor* owner, bool moveOn = false, bool rotateOn = false, int updateOrder = 100);
	~EffectComponent();
	void LoadEffect(const char16_t* effkseerFilename);
	void Update(float deltaTime) override;

	void SetRelativePosition(const Vector3& relativePos) { mRelativePos = relativePos; }
	void SetRelativeRotate(const Matrix4& relativeRotMat) { mRelativeRot = relativeRotMat; }
	void SetMove(bool move) { mIsMove = move; }
	void SetRotate(bool rotate) { mIsRotate = rotate; }

private:
	class Actor* mOwner;                 // オーナーアクター
	Effekseer::Handle mHandle;           // エフェクトパラメータを管理するためのハンドル
	bool              mIsMove;           // オーナーアクターの移動に合わせる
	bool              mIsRotate;         // オーナーアクターの回転移動に合わせる
	Vector3           mRelativePos;      // オーナーアクターの相対座標
	Matrix4           mRelativeRot;      // 相対回転行列
};
