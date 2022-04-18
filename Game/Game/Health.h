#pragma once
#include <algorithm>

class Health
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="max">�ő�l</param>
	/// <param name="min">�ŏ��l</param>
	/// <param name="pre">���ݒl</param>
	Health(float max = 0, float min = 0, float pre = 0);

// �Q�b�^�[ //
	float GetMax() { return mMax; }
	float GetMin() { return mMin; }
	float GetPre() { return mPre; }

// �Z�b�^�[ //
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
	float mMax;        // �ő�l
	float mMin;        // �ŏ��l
	float mPre;        // ���݂̒l
};