#pragma once

#include "Math.h"

class BoneTransform
{
public:
	/// <summary>
	/// 行列の出力
	/// </summary>
	/// <returns>回転と平行移動から行列を返す</returns>
	Matrix4 ToMatrix() const;

	/// <summary>
	/// ボーンの補間
	/// </summary>
	/// <param name="a">回転a</param>
	/// <param name="b">回転b</param>
	/// <param name="f">割合の数値</param>
	/// <returns></returns>
	static BoneTransform Interpolate(const BoneTransform& a, const BoneTransform& b, float f);

	// 回転
	Quaternion mRotation;

	// 移動
	Vector3 mTranslation;
};