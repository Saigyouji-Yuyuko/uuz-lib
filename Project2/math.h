#pragma once
namespace uuz
{
	template<typename T>
	constexpr T abs(const T& p)noexcept
	{
		return p < 0 ? -p : p;
	}

}