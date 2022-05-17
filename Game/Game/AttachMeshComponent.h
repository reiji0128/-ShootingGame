#pragma once
#include "MeshComponent.h"
#include "Math.h"
#include <string>
#include "ShaderTag.h"

class AttachMeshComponent : public MeshComponent
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">オーナーへのポインタ</param>
	/// <param name="skelMeshComp">アタッチ先のアクターのスケルタルメッシュのポインタ</param>
	/// <param name="boneName">アタッチ先のボーンの名前</param>
	/// <param name="shaderTag">適用するシェーダーのタグ</param>
	AttachMeshComponent(class Actor* owner,class SkeletalMeshComponent* skelMeshComp,const char* boneName,ShaderTag shaderTag);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~AttachMeshComponent();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="shader">使用するシェーダークラスのポインタ</param>
	void Draw(class Shader* shader) override;

// セッター //
	//オフセット位置・角度

	/// <summary>
	/// 差分の平行移動行列を設定
	/// </summary>
	/// <param name="pos">位置</param>
	void SetOffsetPosition(const Vector3& pos);

	/// <summary>
	/// 差分の回転行列を設定
	/// </summary>
	/// <param name="rot">角度</param>
	void SetOffsetRotation(const Vector3& rot);

protected:
	//ボーン名から取得したインデックス
	int mBoneIndex;

	//アタッチ先のボーン名
	const char* mBoneName;

	//オフセット位置
	Matrix4 mPositionMat;

	//オフセット角度
	Matrix4 mRotationMat;

	// スケルトンメッシュのポインタ
	SkeletalMeshComponent* mSkelMesh;
};