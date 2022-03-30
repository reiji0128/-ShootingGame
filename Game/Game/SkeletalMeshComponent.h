#pragma once

#include<string>
#include "MeshComponent.h"
#include "MatrixPalette.h"

class SkeletalMeshComponent : public MeshComponent
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">コンポーネントを所有するActorのポインタ</param>
	SkeletalMeshComponent(class Actor* owner);

	/// <summary>
	/// スケルタルメッシュコンポーネントの描画処理
	/// </summary>
	/// <param name="shader"Shaderのポインタ></param>
	void Draw(class Shader* shader) override;

	/// <summary>
	/// スケルタルメッシュコンポーネントの更新処理
	/// </summary>
	/// <param name="deltaTime">1フレームの経過時間</param>
	void Update(float deltaTime) override;
	
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	/// <param name="anim">Animationのポインタ</param>
	/// <param name="playRate">再生割合</param>
	/// <returns>アニメーションの残り長さを返す</returns>
	float PlayAnimation(const class Animation* anim, float playRate = 1.0f);

	/// <summary>
	/// 現在再生中のアニメーションは再生中か？
	/// </summary>
	/// <returns>
	/// true  : 再生中
	/// false : 再生終了
	/// </returns>
	bool IsPlaying();

// セッター //
	/// <summary>
	/// スケルトンのセット
	/// </summary>
	/// <param name="sk">セットするスケルトンのポインタ</param>
	void SetSkeleton(const class Skeleton* sk) { mSkeleton = sk; }

	/// <summary>
	/// GetBoneIndexFromName関数の呼び出し
	/// </summary>
	/// <param name="BoneName">取得したいボーン名</param>
	/// <returns>ボーンのインデックスを取得</returns>
	int CallGetBoneIndexFromName(std::string BoneName);

// ゲッター //
	/// <summary>
	/// 現在再生中のアニメーションの取得
	/// </summary>
	/// <returns>現在再生中のアニメーション</returns>
	const class Animation* GetNowPlayingAnimation() { return mAnimation; }

	/// <summary>
	/// 再生時間の取得
	/// </summary>
	/// <returns>再生時間</returns>
	float GetPlayTime() { return mAnimTime; }

	/// <summary>
	/// インデックスからボーンの行列を取得
	/// </summary>
	/// <param name="BoneIndex">取得したいボーンのインデックス</param>
	/// <returns>ボーンの行列</returns>
	const Matrix4 GetBoneMatrix(int BoneIndex) const;

protected:
	/// <summary>
	/// 行列パレットの計算
	/// </summary>
	void ComputeMatrixPalette();

	// 行列パレット
	MatrixPalette mPalette;

	// Skeletonのポインタ
	const class Skeleton* mSkeleton;

	// Animationのポインタ
	const class Animation* mAnimation;

	// アニメーションの再生速度
	float mAnimPlayRate;

	// アニメーション時間
	float mAnimTime;
};
