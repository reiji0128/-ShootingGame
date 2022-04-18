#include "Health.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="max">最大値</param>
/// <param name="min">最小値</param>
/// <param name="pre">現在値</param>
Health::Health(float max, float min, float pre)
    :mMax(max)
    ,mMin(min)
    ,mPre(pre)
{
}

/// <summary>
/// 現在の値に加算
/// </summary>
/// <param name="value">加算する値</param>
/// <returns>現在の値</returns>
float Health::addPre(float value)
{
    SetPre(GetPre() + value);
    return GetPre();
}

/// <summary>
/// 最大値に加算
/// </summary>
/// <param name="value">加算する値</param>
/// <returns>最大値</returns>
float Health::addMax(float value)
{
    SetMax(GetMax() + value);
    return GetMax();
}

/// <summary>
/// 最小値に加算
/// </summary>
/// <param name="value">加算する値</param>
/// <returns>最小値</returns>
float Health::addMin(float value)
{
    SetMin(GetMin() + value);
    return GetMin();
}

/// <summary>
/// 現在の値が0か
/// </summary>
/// <returns>
/// treu  : 0
/// false : 0より大きい
/// </returns>
bool Health::isEmpty()
{
    return (GetPre() <= GetMin());
}

/// <summary>
/// 現在の値が最大値か
/// </summary>
/// <returns>
/// true  : 最大値
/// false : 最大値未満
/// </returns>
bool Health::isFull()
{
    return (GetMax() <= GetPre());
}
