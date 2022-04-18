#pragma once
#include <algorithm>

class Health
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="max">最大値</param>
	/// <param name="min">最小値</param>
	/// <param name="pre">現在値</param>
	Health(float max = 0, float min = 0, float pre = 0);

// ゲッター //
	float GetMax() { return mMax; }
	float GetMin() { return mMin; }
	float GetPre() { return mPre; }

// セッター //
	void SetMax(float value)
	{
		mMax = (std::max)(mMin, value);
		if (mMax < GetPre())
		{
			SetPre(mMax);
		}
	}

	void SetMin(float value)
	{
		mMin = (std::min)(value, mMax);
		if (GetPre() < mMin)
		{
			SetPre(mMin);
		}
	}

	void SetPre(float value)
	{
		value = (std::min)(value, mMax);
		value = (std::max)(value, mMin);
		mPre = value;
	}


	float addPre(float value);
	float addMax(float value);
	float addMin(float value);

	bool isEmpty();
	bool isFull();
private:
	float mMax;        // 最大値
	float mMin;        // 最小値
	float mPre;        // 現在の値
};