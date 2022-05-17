#include "ShaderTag.h"

// 前置インクリメント
ShaderTag& operator++(ShaderTag& rhs)
{
	if (rhs == ShaderTag::End)
	{
		throw std::out_of_range("for outof range ShaderTag& operator ++ (ShaderTag&)");
	}
	rhs = ShaderTag(static_cast<std::underlying_type<ShaderTag>::type>(rhs)> +1);

	return rhs;
}