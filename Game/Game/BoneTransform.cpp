#include "BoneTransform.h"

/// <summary>
/// 行列の出力
/// </summary>
/// <returns>回転と平行移動から行列を返す</returns>
Matrix4 BoneTransform::ToMatrix() const
{
	// 回転と平行移動から、行列を返す
	Matrix4 rot = Matrix4::CreateFromQuaternion(mRotation);
	Matrix4 trans = Matrix4::CreateTranslation(mTranslation);

	return rot * trans;
}

/// <summary>
/// ボーンの補間
/// </summary>
/// <param name="a">回転a</param>
/// <param name="b">回転b</param>
/// <param name="f">割合の数値</param>
/// <returns></returns>
BoneTransform BoneTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, float f)
{
	BoneTransform retVal;

	// aからb に数値fで球面線形補間(slerp)する
	retVal.mRotation = Quaternion::Slerp(a.mRotation, b.mRotation, f);
	// 平行移動成分はf で線形補間(lerp)する
	retVal.mTranslation = Vector3::Lerp(a.mTranslation, b.mTranslation, f);
	return retVal;
}
