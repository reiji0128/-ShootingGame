#include "AttachMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Math.h"
#include "Mesh.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Shader.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="owner">オーナーへのポインタ</param>
/// <param name="skelMeshComp">アタッチ先のアクターのスケルタルメッシュのポインタ</param>
/// <param name="boneName">アタッチ先のボーンの名前</param>
/// <param name="shaderTag">適用するシェーダーのタグ</param>
AttachMeshComponent::AttachMeshComponent(class Actor* owner, class SkeletalMeshComponent* skelMeshComp,
	                                     const char* boneName, ShaderTag shaderTag)
	:MeshComponent(owner,shaderTag,false)
	,mBoneIndex(0)
	,mBoneName(boneName)
	,mSkelMesh(skelMeshComp)
{
}

/// <summary>
/// デストラクタ
/// </summary>
AttachMeshComponent::~AttachMeshComponent()
{
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="shader">シェーダーのポインタ</param>
void AttachMeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		Matrix4 AnimMatrix, resultMatrix;
		//ボーンインデックスの取得
		mBoneIndex = mSkelMesh->CallGetBoneIndexFromName(mBoneName);
		//インデックスから行列を取得
		AnimMatrix = mSkelMesh->GetBoneMatrix(mBoneIndex);
		resultMatrix = mRotationMat * mPositionMat * AnimMatrix;

		// ワールド変換をセット
		shader->SetMatrixUniform("uWorldTransform", resultMatrix);
		// スペキュラ強度セット
		shader->SetFloatUniform("uSpecPower", 100);
		// アクティブテクスチャセット
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		// 頂点配列をアクティブに
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		// 描画する
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

/// <summary>
/// アタッチさせるものの位置をセット
/// </summary>
/// <param name="pos"></param>
void AttachMeshComponent::SetOffsetPosition(const Vector3& pos)
{
	mPositionMat = Matrix4::CreateTranslation(pos);
}

/// <summary>
/// アタッチさせるものの角度をセット
/// </summary>
/// <param name="rot"></param>
void AttachMeshComponent::SetOffsetRotation(const Vector3& rot)
{
	Vector3 radian;
	radian.x = rot.x * (Math::Pi / 180);
	radian.y = rot.y * (Math::Pi / 180);
	radian.z = rot.z * (Math::Pi / 180);


	mRotationMat = Matrix4::CreateRotationX(radian.x)*
				   Matrix4::CreateRotationY(radian.y)*
				   Matrix4::CreateRotationZ(radian.z);
}
