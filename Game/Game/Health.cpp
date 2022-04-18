#include "Health.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="max">�ő�l</param>
/// <param name="min">�ŏ��l</param>
/// <param name="pre">���ݒl</param>
Health::Health(float max, float min, float pre)
    :mMax(max)
    ,mMin(min)
    ,mPre(pre)
{
}

/// <summary>
/// ���݂̒l�ɉ��Z
/// </summary>
/// <param name="value">���Z����l</param>
/// <returns>���݂̒l</returns>
float Health::addPre(float value)
{
    SetPre(GetPre() + value);
    return GetPre();
}

/// <summary>
/// �ő�l�ɉ��Z
/// </summary>
/// <param name="value">���Z����l</param>
/// <returns>�ő�l</returns>
float Health::addMax(float value)
{
    SetMax(GetMax() + value);
    return GetMax();
}

/// <summary>
/// �ŏ��l�ɉ��Z
/// </summary>
/// <param name="value">���Z����l</param>
/// <returns>�ŏ��l</returns>
float Health::addMin(float value)
{
    SetMin(GetMin() + value);
    return GetMin();
}

/// <summary>
/// ���݂̒l��0��
/// </summary>
/// <returns>
/// treu  : 0
/// false : 0���傫��
/// </returns>
bool Health::isEmpty()
{
    return (GetPre() <= GetMin());
}

/// <summary>
/// ���݂̒l���ő�l��
/// </summary>
/// <returns>
/// true  : �ő�l
/// false : �ő�l����
/// </returns>
bool Health::isFull()
{
    return (GetMax() <= GetPre());
}
